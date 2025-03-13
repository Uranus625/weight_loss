#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(const QString& username, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , blueDevice(new BlueDevice())
    , m_username(username)
{
    ui->setupUi(this);
    Adaptive_screen();

    setplot();   //设置绘图
    setbutton();  //设置按钮
    loadUserData(); // 先加载用户数据
    setspinbox(); //设置输入框
    setLED(ui->flag, 1, 16);
    ui->listWidget->setStyleSheet("font-size: 20pt;");

    connect(ui->search_button, &QPushButton::clicked, this, &MainWindow::onStartDiscoveryClicked);
    connect(ui->connect_button, &QPushButton::clicked, this, &MainWindow::connectdevice);

    connect(blueDevice, &BlueDevice::deviceDiscovered, this, &MainWindow::onDeviceDiscovered);
    connect(blueDevice, &BlueDevice::connectionEstablished, this, &MainWindow::onConnectionEstablished);
    connect(blueDevice, &BlueDevice::connectionLost, this, &MainWindow::onConnectionLost);
    connect(blueDevice, &BlueDevice::dataReceived, this, &MainWindow::updateData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadUserData()
{
    if (m_username.isEmpty()) {
        return;
    }

    // 获取用户最新的体重数据
    double latestWeight = DatabaseManager::instance().getLatestWeight(m_username, 60.0); // 默认值60kg
    ui->spinBox->setValue(static_cast<int>(latestWeight));

    // 获取用户最新的百分比数据
    double latestPercentage = DatabaseManager::instance().getLatestPercentage(m_username, 50.0); // 默认值50%
    ui->spinBox_2->setValue(static_cast<int>(latestPercentage*100));

    qDebug() << "已加载用户" << m_username << "的数据: 体重=" << latestWeight
             << "kg, 百分比=" << latestPercentage << "%";
}

void MainWindow::onStartDiscoveryClicked()
{
    ui->listWidget->clear();
    blueDevice->startDiscovery();
}

void MainWindow::onConnectionEstablished()
{
    setLED(ui->flag, 2, 16);
}

void MainWindow::onConnectionLost()
{
    setLED(ui->flag, 1, 16);
}

void MainWindow::onDeviceDiscovered(const QBluetoothDeviceInfo &device)
{
    ui->listWidget->addItem(device.name());
    mydevice.append(device);
}

void MainWindow::connectdevice()
{
    int row = ui->listWidget->currentRow();
    const auto deviceInfo = mydevice.at(row);
    blueDevice->connectToDevice(deviceInfo);
}

void MainWindow::setplot()
{
    ui->myCustomPlot->addGraph(); // 添加一条曲线
    ui->myCustomPlot->graph(0)->setPen(QPen(Qt::blue));

    ui->myCustomPlot->xAxis->setLabel("Time (s)"); // 横轴以时间为刻度
    ui->myCustomPlot->yAxis->setLabel("Value");

    double mLastTime = QDateTime::currentMSecsSinceEpoch() / 1000.0; // 初始化数据
    mTimeOffset = mLastTime;
}

QList<QByteArray> senddata = {"ss", "sf", "sb", "sl", "sr", "sr"};

void MainWindow::setbutton()
{
    // 按下前进按钮时发送 "forward"
    connect(ui->front_button, &QPushButton::pressed, this, [this](){
        qDebug() << "forward";
        blueDevice->sendData(senddata[1]);
    });
    // 松开前进按钮时发送 "stop"
    connect(ui->front_button, &QPushButton::released, this, [this](){
        qDebug() << "stop";
    });

    connect(ui->behind_button, &QPushButton::pressed, this, [this](){
        qDebug() << "behind";
    });
    connect(ui->behind_button, &QPushButton::released, this, [this](){
        qDebug() << "stop";
    });

    connect(ui->left_button, &QPushButton::pressed, this, [this](){
        qDebug() << "left";
    });
    connect(ui->left_button, &QPushButton::released, this, [this](){
        qDebug() << "stop";
    });

    connect(ui->right_button, &QPushButton::pressed, this, [this](){
        qDebug() << "right";
    });
    connect(ui->right_button, &QPushButton::released, this, [this](){
        qDebug() << "stop";
    });

    connect(ui->revolve_button, &QPushButton::pressed, this, [this](){
        qDebug() << "revolve";
    });
    connect(ui->revolve_button, &QPushButton::released, this, [this](){
        qDebug() << "stop";
    });
}

void MainWindow::setspinbox()
{
    connect(ui->spinBox, &QSpinBox::editingFinished, this, [=](){
        int weightValue = ui->spinBox->value();
        qDebug() << "体重输入值：" << weightValue;

        // 保存体重数据到数据库
        if (!m_username.isEmpty()) {
            DatabaseManager::instance().saveWeightData(m_username, weightValue);
        }
    });

    connect(ui->spinBox_2, &QSpinBox::editingFinished, this, [=](){
        int Value = ui->spinBox_2->value();
        qDebug() << "百分比输入值：" << Value;

        double percentageValue = Value*0.01;
        // 保存百分比数据到数据库
        if (!m_username.isEmpty()) {
            DatabaseManager::instance().savePercentageData(m_username, percentageValue);
        }
    });
}

float Byte_to_Float(const QByteArray &data) {
    float value;
    if(data.length() == 4)
    {
        memcpy(&value, data, 4);
        return value;
    }
    else
        return 0;
}

void MainWindow::updateData(const QByteArray &data)
{
    double currentTime = QDateTime::currentMSecsSinceEpoch() / 1000.0 - mTimeOffset;
    float value = Byte_to_Float(data); // 模拟数据，可改成真实采集值

    ui->myCustomPlot->graph(0)->addData(currentTime, value); // 向曲线追加数据

    ui->myCustomPlot->xAxis->setRange(currentTime, 10, Qt::AlignRight);  // 仅显示最近 10 秒的数据
    ui->myCustomPlot->graph(0)->rescaleValueAxis(true);

    ui->myCustomPlot->replot(); // 刷新图表
}

void MainWindow::setLED(QLabel* label, int color, int size)
{
    // 将label中的文字清空
    label->setText("");
    // 先设置矩形大小
    // 如果ui界面设置的label大小比最小宽度和高度小，矩形将被设置为最小宽度和最小高度；
    // 如果ui界面设置的label大小比最小宽度和高度大，矩形将被设置为最大宽度和最大高度；
    QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
    QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
    QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
    QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
    // 再设置边界形状及边框
    QString border_radius = QString("border-radius: %1px;").arg(size/2);    // 边框是圆角，半径为size/2
    QString border = QString("border:1px solid black;");                    // 边框为1px黑色
    // 最后设置背景颜色
    QString background = "background-color:";
    switch (color) {
    case 0:
        // 灰色
        background += "rgb(190,190,190)";
        break;
    case 1:
        // 红色
        background += "rgb(255,0,0)";
        break;
    case 2:
        // 绿色
        background += "rgb(0,255,0)";
        break;
    case 3:
        // 黄色
        background += "rgb(255,255,0)";
        break;
    default:
        break;
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    label->setStyleSheet(SheetStyle);
}

void MainWindow::Adaptive_screen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    resize(screenGeometry.width(), screenGeometry.height());

    int w = width();
    int h = height();

    // 计算字体缩放比例
    double fontRatio = qMin((double)w / 1023.0, (double)h / 703.0);

    ui->mainwidget->setGeometry(0, 0, w, h);
    ui->myCustomPlot->setGeometry(330 * w / 1023, 20 * h / 703, 661 * w / 1023, 331 * h / 703);
    ui->listWidget->setGeometry(20 * w / 1023, 40 * h / 703, 271 * w / 1023, 301 * h / 703);
    ui->search_button->setGeometry(20 * w / 1023, 360 * h / 703, 271 * w / 1023, 41 * h / 703);
    ui->connect_button->setGeometry(20 * w / 1023, 420 * h / 703, 271 * w / 1023, 41 * h / 703);
    ui->front_button->setGeometry(610 * w / 1023, 390 * h / 703, 80 * w / 1023, 60 * h / 703);
    ui->behind_button->setGeometry(610 * w / 1023, 570 * h / 703, 80 * w / 1023, 60 * h / 703);
    ui->left_button->setGeometry(500 * w / 1023, 480 * h / 703, 80 * w / 1023, 60 * h / 703);
    ui->right_button->setGeometry(720 * w / 1023, 480 * h / 703, 80 * w / 1023, 60 * h / 703);
    ui->revolve_button->setGeometry(610 * w / 1023, 480 * h / 703, 80 * w / 1023, 60 * h / 703);
    ui->spinBox->setGeometry(20 * w / 1023, 490 * h / 703, 271 * w / 1023, 61 * h / 703);
    ui->spinBox_2->setGeometry(20 * w / 1023, 580 * h / 703, 271 * w / 1023, 61 * h / 703);
    ui->flag->setGeometry(20 * w / 1023, 10 * h / 703, 31 * w / 1023, 21 * h / 703);

    // 调整列表控件字体大小
    int fontSize = qRound(20 * fontRatio); // 原始大小为20pt
    ui->listWidget->setStyleSheet(QString("font-size: %1pt;").arg(fontSize));

    // 调整按钮字体大小
    QFont buttonFont;
    buttonFont.setPointSize(qRound(12 * fontRatio)); // 假设原始大小为12pt
    ui->search_button->setFont(buttonFont);
    ui->connect_button->setFont(buttonFont);
    ui->front_button->setFont(buttonFont);
    ui->behind_button->setFont(buttonFont);
    ui->left_button->setFont(buttonFont);
    ui->right_button->setFont(buttonFont);
    ui->revolve_button->setFont(buttonFont);

    // 调整SpinBox字体大小
    QFont spinBoxFont;
    spinBoxFont.setPointSize(qRound(12 * fontRatio));
    ui->spinBox->setFont(spinBoxFont);
    ui->spinBox_2->setFont(spinBoxFont);

    // 调整图表字体大小
    QFont plotFont;
    plotFont.setPointSize(qRound(10 * fontRatio));
    ui->myCustomPlot->xAxis->setTickLabelFont(plotFont);
    ui->myCustomPlot->yAxis->setTickLabelFont(plotFont);

    QFont axisLabelFont;
    axisLabelFont.setPointSize(qRound(12 * fontRatio));
    ui->myCustomPlot->xAxis->setLabelFont(axisLabelFont);
    ui->myCustomPlot->yAxis->setLabelFont(axisLabelFont);

    // 刷新图表以应用字体更改
    ui->myCustomPlot->replot();
}

