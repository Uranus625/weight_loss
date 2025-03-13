#include "login.h"
#include "ui_login.h"
#include <QRegularExpressionValidator>

#include "mainwindow.h"

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    ui->username->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$")));
    ui->code->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$")));

    connect(ui->login_button, &QPushButton::clicked, this, &login::onloginseccess);
    connect(ui->register_button, &QPushButton::clicked, this, &login::onregister);

    adaptiveScreen();
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

login::~login()
{
    delete ui;
}

void login::onloginseccess()
{
    QString username = ui->username->text();
    QString password = ui->code->text();

    // 验证用户名和密码
    if (DatabaseManager::instance().loginUser(username, password)) {
        // 登录成功
        m_currentUsername = username; // 保存登录的用户名

        MainWindow *mainWindow = new MainWindow(m_currentUsername);
        mainWindow->show();
        this->close();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
    }
}

void login::onregister()
{
    QString username = ui->username->text();
    QString password = ui->code->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名和密码不能为空！");
        return;
    }

    // 注册用户
    if (DatabaseManager::instance().registerUser(username, password)) {
        QMessageBox::information(this, "注册成功", "用户注册成功，请登录！");
    } else {
        QMessageBox::warning(this, "注册失败", "用户名已存在或注册过程中出错！");
    }
}

void login::adaptiveScreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    resize(screenGeometry.width(), screenGeometry.height());

    // 原始设计尺寸
    const int designWidth = 800;
    const int designHeight = 600;

    // 计算缩放比例
    double scaleRatio = qMin((double)screenGeometry.width() / designWidth,
                             (double)screenGeometry.height() / designHeight);

    // 计算控件区域的整体宽度和高度
    // 原始布局中的边界
    const int originalLeft = 210;   // 最左侧控件的x坐标
    const int originalTop = 180;    // 最顶部控件的y坐标
    const int originalRight = 551;  // 最右侧控件的x坐标 (300+251)
    const int originalBottom = 411; // 最底部控件的y坐标 (360+51)

    // 计算整体宽度和高度
    const int groupWidth = originalRight - originalLeft;
    const int groupHeight = originalBottom - originalTop;

    // 计算缩放后的整体宽度和高度
    const int scaledWidth = groupWidth * scaleRatio;
    const int scaledHeight = groupHeight * scaleRatio;

    // 计算整个控件组的左上角坐标，使其居中
    const int groupLeft = (screenGeometry.width() - scaledWidth) / 2;
    const int groupTop = (screenGeometry.height() - scaledHeight) / 2;

    // 调整每个控件的位置和大小，保持相对位置不变

    // 用户名输入框
    ui->username->setGeometry(
        groupLeft + (300 - originalLeft) * scaleRatio,
        groupTop + (180 - originalTop) * scaleRatio,
        251 * scaleRatio,
        61 * scaleRatio
        );

    // 密码输入框
    ui->code->setGeometry(
        groupLeft + (300 - originalLeft) * scaleRatio,
        groupTop + (270 - originalTop) * scaleRatio,
        251 * scaleRatio,
        61 * scaleRatio
        );

    // 登录按钮
    ui->login_button->setGeometry(
        groupLeft + (210 - originalLeft) * scaleRatio,
        groupTop + (360 - originalTop) * scaleRatio,
        131 * scaleRatio,
        51 * scaleRatio
        );

    // 注册按钮
    ui->register_button->setGeometry(
        groupLeft + (420 - originalLeft) * scaleRatio,
        groupTop + (360 - originalTop) * scaleRatio,
        131 * scaleRatio,
        51 * scaleRatio
        );

    // 用户名标签
    ui->username_label->setGeometry(
        groupLeft + (210 - originalLeft) * scaleRatio,
        groupTop + (180 - originalTop) * scaleRatio,
        81 * scaleRatio,
        61 * scaleRatio
        );

    // 密码标签
    ui->label_2->setGeometry(
        groupLeft + (210 - originalLeft) * scaleRatio,
        groupTop + (270 - originalTop) * scaleRatio,
        81 * scaleRatio,
        61 * scaleRatio
        );

    // 调整字体大小
    QFont labelFont = ui->username_label->font();
    labelFont.setPointSize(int(24 * scaleRatio));
    ui->username_label->setFont(labelFont);
    ui->label_2->setFont(labelFont);

    // 输入框字体也调整
    QFont editFont = ui->username->font();
    editFont.setPointSize(int(10 * scaleRatio));
    ui->username->setFont(editFont);
    ui->code->setFont(editFont);

    // 按钮字体
    QFont buttonFont = ui->login_button->font();
    buttonFont.setPointSize(int(10 * scaleRatio));
    ui->login_button->setFont(buttonFont);
    ui->register_button->setFont(buttonFont);
}
