/********************************************************************************
** Form generated from reading UI file 'LoginWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QWidget *centralwidget;
    QLabel *label_id;
    QLabel *label_pw;
    QPushButton *loginBtn;
    QLineEdit *lineEdit_id;
    QLineEdit *lineEdit_pw;
    QLabel *label_result;
    QLabel *label_ip;
    QLineEdit *lineEdit_ip;

    void setupUi(QMainWindow *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName(QString::fromUtf8("LoginWindow"));
        LoginWindow->resize(500, 184);
        centralwidget = new QWidget(LoginWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label_id = new QLabel(centralwidget);
        label_id->setObjectName(QString::fromUtf8("label_id"));
        label_id->setGeometry(QRect(20, 70, 61, 16));
        label_pw = new QLabel(centralwidget);
        label_pw->setObjectName(QString::fromUtf8("label_pw"));
        label_pw->setGeometry(QRect(20, 100, 61, 16));
        loginBtn = new QPushButton(centralwidget);
        loginBtn->setObjectName(QString::fromUtf8("loginBtn"));
        loginBtn->setGeometry(QRect(90, 150, 80, 24));
        lineEdit_id = new QLineEdit(centralwidget);
        lineEdit_id->setObjectName(QString::fromUtf8("lineEdit_id"));
        lineEdit_id->setGeometry(QRect(50, 70, 411, 24));
        lineEdit_pw = new QLineEdit(centralwidget);
        lineEdit_pw->setObjectName(QString::fromUtf8("lineEdit_pw"));
        lineEdit_pw->setGeometry(QRect(50, 100, 411, 24));
        label_result = new QLabel(centralwidget);
        label_result->setObjectName(QString::fromUtf8("label_result"));
        label_result->setGeometry(QRect(290, 150, 141, 16));
        label_ip = new QLabel(centralwidget);
        label_ip->setObjectName(QString::fromUtf8("label_ip"));
        label_ip->setGeometry(QRect(20, 40, 48, 16));
        lineEdit_ip = new QLineEdit(centralwidget);
        lineEdit_ip->setObjectName(QString::fromUtf8("lineEdit_ip"));
        lineEdit_ip->setGeometry(QRect(50, 40, 411, 24));
        LoginWindow->setCentralWidget(centralwidget);

        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QMainWindow *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "MainWindow", nullptr));
        label_id->setText(QCoreApplication::translate("LoginWindow", "ID", nullptr));
        label_pw->setText(QCoreApplication::translate("LoginWindow", "PW", nullptr));
        loginBtn->setText(QCoreApplication::translate("LoginWindow", "LOGIN", nullptr));
        label_result->setText(QCoreApplication::translate("LoginWindow", "Login Result : ", nullptr));
        label_ip->setText(QCoreApplication::translate("LoginWindow", "IP", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
