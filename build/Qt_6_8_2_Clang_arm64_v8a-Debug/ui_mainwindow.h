/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QListWidget *listWidget;
    QPushButton *search_button;
    QPushButton *connect_button;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QCustomPlot *myCustomPlot;
    QPushButton *revolve_button;
    QPushButton *behind_button;
    QPushButton *right_button;
    QPushButton *front_button;
    QPushButton *left_button;
    QLabel *flag;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1023, 703);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, 10, 1001, 641));
        listWidget = new QListWidget(widget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(20, 40, 271, 301));
        search_button = new QPushButton(widget);
        search_button->setObjectName("search_button");
        search_button->setGeometry(QRect(20, 350, 271, 41));
        connect_button = new QPushButton(widget);
        connect_button->setObjectName("connect_button");
        connect_button->setGeometry(QRect(20, 400, 271, 41));
        spinBox = new QSpinBox(widget);
        spinBox->setObjectName("spinBox");
        spinBox->setGeometry(QRect(20, 470, 271, 61));
        spinBox_2 = new QSpinBox(widget);
        spinBox_2->setObjectName("spinBox_2");
        spinBox_2->setGeometry(QRect(20, 550, 271, 61));
        myCustomPlot = new QCustomPlot(widget);
        myCustomPlot->setObjectName("myCustomPlot");
        myCustomPlot->setGeometry(QRect(320, 10, 661, 331));
        revolve_button = new QPushButton(widget);
        revolve_button->setObjectName("revolve_button");
        revolve_button->setGeometry(QRect(610, 460, 80, 60));
        behind_button = new QPushButton(widget);
        behind_button->setObjectName("behind_button");
        behind_button->setGeometry(QRect(610, 550, 80, 60));
        right_button = new QPushButton(widget);
        right_button->setObjectName("right_button");
        right_button->setGeometry(QRect(720, 460, 80, 60));
        front_button = new QPushButton(widget);
        front_button->setObjectName("front_button");
        front_button->setGeometry(QRect(610, 370, 80, 60));
        left_button = new QPushButton(widget);
        left_button->setObjectName("left_button");
        left_button->setGeometry(QRect(500, 460, 80, 60));
        flag = new QLabel(widget);
        flag->setObjectName("flag");
        flag->setGeometry(QRect(20, 10, 31, 21));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1023, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        search_button->setText(QCoreApplication::translate("MainWindow", "\346\220\234\347\264\242\350\256\276\345\244\207", nullptr));
        connect_button->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245", nullptr));
        spinBox->setSuffix(QCoreApplication::translate("MainWindow", "             kg", nullptr));
        spinBox->setPrefix(QCoreApplication::translate("MainWindow", "\346\202\243\350\200\205\344\275\223\351\207\215\357\274\232            ", nullptr));
        spinBox_2->setSuffix(QCoreApplication::translate("MainWindow", "             %", nullptr));
        spinBox_2->setPrefix(QCoreApplication::translate("MainWindow", "\345\207\217\351\207\215\346\257\224\344\276\213\357\274\232            ", nullptr));
        revolve_button->setText(QCoreApplication::translate("MainWindow", "\346\227\213\350\275\254", nullptr));
        behind_button->setText(QCoreApplication::translate("MainWindow", "\345\220\216", nullptr));
        right_button->setText(QCoreApplication::translate("MainWindow", "\345\217\263", nullptr));
        front_button->setText(QCoreApplication::translate("MainWindow", "\345\211\215", nullptr));
        left_button->setText(QCoreApplication::translate("MainWindow", "\345\267\246", nullptr));
        flag->setText(QCoreApplication::translate("MainWindow", "led", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
