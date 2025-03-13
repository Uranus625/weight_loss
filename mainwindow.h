#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

#include <QTimer>
#include <QDateTime>
#include <QLabel>
#include "bluetooth.h"
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& username = QString(), QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    BlueDevice *blueDevice; // 蓝牙设备处理对象
    QList<QBluetoothDeviceInfo> mydevice;
    double mTimeOffset;
    void setLED(QLabel* label, int color, int size);
    //void updateData();

    void setplot();
    void setbutton();
    void setspinbox();
    void Adaptive_screen();

    QString m_username;
    void loadUserData();

private slots:
    void onStartDiscoveryClicked(); // 开始设备发现
    void onConnectionEstablished(); // 连接成功
    void onConnectionLost(); // 连接断开
    void onDeviceDiscovered(const QBluetoothDeviceInfo &device); // 发现设备
    void connectdevice();
    void updateData(const QByteArray &data);
};
#endif // MAINWINDOW_H
