#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    static DatabaseManager& instance();

    // 用户相关方法
    bool registerUser(const QString& username, const QString& password);
    bool loginUser(const QString& username, const QString& password);

    // 体重相关方法
    bool saveWeightData(const QString& username, double weight,
                        const QDateTime& timestamp = QDateTime::currentDateTime());
    double getLatestWeight(const QString& username, double defaultValue = 0.0);
    QVector<QPair<QDateTime, double>> getWeightHistory(const QString& username);

    // 百分比相关方法
    bool savePercentageData(const QString& username, double percentage,
                            const QDateTime& timestamp = QDateTime::currentDateTime());
    double getLatestPercentage(const QString& username, double defaultValue = 0.0);
    QVector<QPair<QDateTime, double>> getPercentageHistory(const QString& username);

private:
    DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();

    bool openDatabase();
    bool initDatabase();
    void closeDatabase();

    QSqlDatabase m_db;
    QString m_dbPath;

    // 禁止复制
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
};

#endif // DATABASEMANAGER_H
