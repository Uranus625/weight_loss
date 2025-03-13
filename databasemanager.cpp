#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject* parent) : QObject(parent)
{
    // 确定数据库文件的存储位置
#ifdef Q_OS_ANDROID
    m_dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/robotcontrol.db";
#else
    m_dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/robotcontrol.db";
#endif

    // 打开数据库并初始化表结构
    if (openDatabase()) {
        initDatabase();
    }
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::openDatabase()
{
    // 确保目录存在
    QDir dir;
    dir.mkpath(QFileInfo(m_dbPath).path());

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbPath);

    if (!m_db.open()) {
        qDebug() << "无法打开数据库:" << m_db.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::initDatabase()
{
    QSqlQuery query;

    // 创建用户表
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "username TEXT UNIQUE NOT NULL, "
                    "password TEXT NOT NULL, "
                    "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                    ")")) {
        qDebug() << "创建用户表失败:" << query.lastError().text();
        return false;
    }

    // 创建体重记录表
    if (!query.exec("CREATE TABLE IF NOT EXISTS weight_records ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "username TEXT NOT NULL, "
                    "weight REAL NOT NULL, "
                    "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                    "FOREIGN KEY(username) REFERENCES users(username)"
                    ")")) {
        qDebug() << "创建体重记录表失败:" << query.lastError().text();
        return false;
    }

    // 创建百分比记录表
    if (!query.exec("CREATE TABLE IF NOT EXISTS percentage_records ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "username TEXT NOT NULL, "
                    "percentage REAL NOT NULL, "
                    "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                    "FOREIGN KEY(username) REFERENCES users(username)"
                    ")")) {
        qDebug() << "创建百分比记录表失败:" << query.lastError().text();
        return false;
    }

    return true;
}

void DatabaseManager::closeDatabase()
{
    m_db.close();
}

bool DatabaseManager::registerUser(const QString& username, const QString& password)
{
    if (username.isEmpty() || password.isEmpty()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);  // 在生产环境中应该使用哈希密码

    if (!query.exec()) {
        qDebug() << "注册用户失败:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::loginUser(const QString& username, const QString& password)
{
    if (username.isEmpty() || password.isEmpty()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);  // 在生产环境中应该使用哈希密码

    if (!query.exec() || !query.next()) {
        qDebug() << "登录失败:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::saveWeightData(const QString& username, double weight, const QDateTime& timestamp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO weight_records (username, weight, timestamp) VALUES (:username, :weight, :timestamp)");
    query.bindValue(":username", username);
    query.bindValue(":weight", weight);
    query.bindValue(":timestamp", timestamp.toString(Qt::ISODate));

    if (!query.exec()) {
        qDebug() << "保存体重数据失败:" << query.lastError().text();
        qDebug() << "错误详情:" << query.lastError().databaseText();
        return false;
    }

    return true;
}

double DatabaseManager::getLatestWeight(const QString& username, double defaultValue)
{
    QSqlQuery query;
    query.prepare("SELECT weight FROM weight_records WHERE username = :username "
                  "ORDER BY timestamp DESC LIMIT 1");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "获取最新体重数据失败:" << query.lastError().text();
        return defaultValue;
    }

    if (query.next()) {
        return query.value(0).toDouble();
    } else {
        // 没有数据时返回默认值
        return defaultValue;
    }
}

QVector<QPair<QDateTime, double>> DatabaseManager::getWeightHistory(const QString& username)
{
    QVector<QPair<QDateTime, double>> results;

    QSqlQuery query;
    query.prepare("SELECT timestamp, weight FROM weight_records WHERE username = :username ORDER BY timestamp");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "获取体重历史失败:" << query.lastError().text();
        return results;
    }

    while (query.next()) {
        QDateTime timestamp = QDateTime::fromString(query.value(0).toString(), Qt::ISODate);
        double weight = query.value(1).toDouble();
        results.append(qMakePair(timestamp, weight));
    }

    return results;
}

bool DatabaseManager::savePercentageData(const QString& username, double percentage, const QDateTime& timestamp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO percentage_records (username, percentage, timestamp) VALUES (:username, :percentage, :timestamp)");
    query.bindValue(":username", username);
    query.bindValue(":percentage", percentage);
    query.bindValue(":timestamp", timestamp.toString(Qt::ISODate));

    if (!query.exec()) {
        qDebug() << "保存百分比数据失败:" << query.lastError().text();
        qDebug() << "错误详情:" << query.lastError().databaseText();
        return false;
    }

    return true;
}

double DatabaseManager::getLatestPercentage(const QString& username, double defaultValue)
{
    QSqlQuery query;
    query.prepare("SELECT percentage FROM percentage_records WHERE username = :username "
                  "ORDER BY timestamp DESC LIMIT 1");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "获取最新百分比数据失败:" << query.lastError().text();
        return defaultValue;
    }

    if (query.next()) {
        return query.value(0).toDouble();
    } else {
        // 没有数据时返回默认值
        return defaultValue;
    }
}

QVector<QPair<QDateTime, double>> DatabaseManager::getPercentageHistory(const QString& username)
{
    QVector<QPair<QDateTime, double>> results;

    QSqlQuery query;
    query.prepare("SELECT timestamp, percentage FROM percentage_records WHERE username = :username ORDER BY timestamp");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "获取百分比历史失败:" << query.lastError().text();
        return results;
    }

    while (query.next()) {
        QDateTime timestamp = QDateTime::fromString(query.value(0).toString(), Qt::ISODate);
        double percentage = query.value(1).toDouble();
        results.append(qMakePair(timestamp, percentage));
    }

    return results;
}
