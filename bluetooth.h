#pragma once

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothServiceInfo>
#include <QBluetoothUuid>
#include <QScopedPointer>

class BlueDevice : public QObject {
    Q_OBJECT
public:
    explicit BlueDevice(QObject *parent = nullptr);
    ~BlueDevice();

    void startDiscovery(); // 开始搜索蓝牙设备
    void connectToDevice(const QBluetoothDeviceInfo &device); // 连接设备
    void sendData(const QByteArray &data); // 发送数据
    void disconnectDevice(); // 断开连接
    QBluetoothDeviceDiscoveryAgent* getDiscoveryAgent() const { return discoveryAgent.data(); }


signals:
    void deviceDiscovered(const QBluetoothDeviceInfo &device); // 发现设备
    void connectionEstablished(); // 连接成功
    void connectionLost(); // 连接断开
    void dataReceived(const QByteArray &data); // 接收到数据
    void socketErrorOccurred(QBluetoothSocket::SocketError error); // RFCOMM 错误信号

private slots:
    void deviceDiscoveredSlot(const QBluetoothDeviceInfo &device); // 发现设备
    void discoveryFinished(); // 搜索完成
    void socketConnected(); // RFCOMM 连接成功
    void socketDisconnected(); // RFCOMM 连接断开
    void readSocketData(); // 读取 RFCOMM 数据
    void handleSocketError(QBluetoothSocket::SocketError error); // 处理 RFCOMM 错误
    void serviceDiscoveredClassic(const QBluetoothServiceInfo &service); // 发现经典蓝牙服务


private:
    QScopedPointer<QBluetoothDeviceDiscoveryAgent> discoveryAgent; // 使用智能指针管理
    QScopedPointer<QBluetoothSocket> socket; // RFCOMM 连接
    QScopedPointer<QBluetoothServiceDiscoveryAgent> serviceDiscoveryAgent; // 经典蓝牙服务发现
};

