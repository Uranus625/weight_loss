#include "bluetooth.h"
#include <QDebug>

BlueDevice::BlueDevice(QObject *parent)
    : QObject(parent)
{
    qDebug() << "初始化蓝牙设备...";

    // 创建蓝牙设备扫描对象
    discoveryAgent.reset(new QBluetoothDeviceDiscoveryAgent(this));
    connect(discoveryAgent.data(), &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BlueDevice::deviceDiscoveredSlot);
    connect(discoveryAgent.data(), &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BlueDevice::discoveryFinished);

    // 创建 RFCOMM 经典蓝牙套接字
    socket.reset(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this));
    connect(socket.data(), &QBluetoothSocket::connected, this, &BlueDevice::socketConnected);
    connect(socket.data(), &QBluetoothSocket::disconnected, this, &BlueDevice::socketDisconnected);
    connect(socket.data(), &QBluetoothSocket::readyRead, this, &BlueDevice::readSocketData);
    connect(socket.data(), &QBluetoothSocket::errorOccurred, this, &BlueDevice::handleSocketError);
}

BlueDevice::~BlueDevice()
{
    qDebug() << "销毁蓝牙设备...";
}

void BlueDevice::startDiscovery()
{
    qDebug() << "开始搜索蓝牙设备...";
    discoveryAgent->start();
}

void BlueDevice::deviceDiscoveredSlot(const QBluetoothDeviceInfo &device)
{
    qDebug() << "发现设备: " << device.name() << " 地址: " << device.address().toString();
        // 启动经典蓝牙服务发现
    if (!serviceDiscoveryAgent) {
        serviceDiscoveryAgent.reset(new QBluetoothServiceDiscoveryAgent(device.address(), this));
        connect(serviceDiscoveryAgent.data(), &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
                this, &BlueDevice::serviceDiscoveredClassic);
        connect(serviceDiscoveryAgent.data(), &QBluetoothServiceDiscoveryAgent::finished,
                this, &BlueDevice::discoveryFinished);
    }
    serviceDiscoveryAgent->start();
    emit deviceDiscovered(device);
}

void BlueDevice::discoveryFinished()
{
    qDebug() << "蓝牙设备搜索完成";
}

void BlueDevice::connectToDevice(const QBluetoothDeviceInfo &device)
{
    if (socket->state() != QBluetoothSocket::SocketState::ConnectedState) {
        socket->connectToService(device.address(), QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));
    }
}

void BlueDevice::serviceDiscoveredClassic(const QBluetoothServiceInfo &service)
{
    qDebug() << "发现经典蓝牙服务: " << service.serviceName() << " 地址: " << service.device().address().toString();
    // 选择合适的服务进行连接（例如 SerialPort）
    if (service.serviceUuid() == QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort)) {
        if (socket->state() != QBluetoothSocket::SocketState::ConnectedState) {
            socket->connectToService(service.device().address(), service.serviceUuid());
        }
    }
}

void BlueDevice::socketConnected()
{
    qDebug() << "经典蓝牙 (RFCOMM) 连接成功!";
    emit connectionEstablished();
}

void BlueDevice::socketDisconnected()
{
    qDebug() << "RFCOMM 连接已断开";
    emit connectionLost();
}

void BlueDevice::sendData(const QByteArray &data)
{
    if (socket->isOpen()) {
        socket->write(data);
        qDebug() << "发送 RFCOMM 数据: " << data;
    } else {
        qDebug() << "RFCOMM 套接字未打开!";
    }
}

void BlueDevice::readSocketData()
{
    QByteArray receivedData = socket->readAll();
    if (receivedData.length() == 8) {
        // 定义包尾数据
        QByteArray expectedTail = QByteArray::fromHex("0000807f");

        // 提取接收到数据的后 4 个字节作为包尾
        QByteArray actualTail = receivedData.right(4);

        if (actualTail == expectedTail) {
            // 提取前 4 个字节的数据
            QByteArray neededData = receivedData.left(4);

            //qDebug() << "收到有效的 RFCOMM 数据，前四个字节数据为: " << neededData.toHex();
            emit dataReceived(neededData); // 将提取的数据通过信号发送
        } else {
            qDebug() << "包尾不匹配，数据无效";
        }
    }
}

void BlueDevice::disconnectDevice()
{
    if (socket->isOpen()) {
        socket->close();
    }
}

void BlueDevice::handleSocketError(QBluetoothSocket::SocketError error)
{
    qDebug() << "Socket error: " << error;
    switch (error) {
    case QBluetoothSocket::SocketError::HostNotFoundError:
        //qDebug() << QString::fromLocal8Bit("主机未找到");
        break;
    case QBluetoothSocket::SocketError::ServiceNotFoundError:
        //qDebug() << "服务未找到";
        break;
    case QBluetoothSocket::SocketError::NoSocketError:
        //qDebug() << "网络错误";
        break;
    case QBluetoothSocket::SocketError::UnsupportedProtocolError:
        //qDebug() << "网络错误";
        break;
    case QBluetoothSocket::SocketError::OperationError:
        //qDebug() << "网络错误";
        break;
    case QBluetoothSocket::SocketError::RemoteHostClosedError:
        break;
    case QBluetoothSocket::SocketError::NetworkError:
        //qDebug() << "网络错误";
        break;
    case QBluetoothSocket::SocketError::UnknownSocketError:
    default:
        //qDebug() << "其他错误";
        break;
    }
    emit socketErrorOccurred(error); // 发送错误信号给主窗口
}

