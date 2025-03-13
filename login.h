#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <Qstring>
#include "databasemanager.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private:
    Ui::login *ui;
    QString m_currentUsername;
    void adaptiveScreen();

signals:

private slots:
    void onloginseccess();
    void onregister();
};

#endif // LOGIN_H
