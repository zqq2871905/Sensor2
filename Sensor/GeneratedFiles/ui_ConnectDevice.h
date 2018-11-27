/********************************************************************************
** Form generated from reading UI file 'ConnectDevice.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTDEVICE_H
#define UI_CONNECTDEVICE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConnectDevice
{
public:
    QTabWidget *tabWidget;
    QWidget *onedevice;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_21;
    QLabel *label_connect1;
    QWidget *twodevice;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QLineEdit *lineEdit_12;
    QLineEdit *lineEdit_13;
    QLineEdit *lineEdit_14;
    QLineEdit *lineEdit_15;
    QLineEdit *lineEdit_16;
    QLabel *label_15;
    QLabel *label_16;
    QLineEdit *lineEdit_17;
    QLineEdit *lineEdit_18;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *lineEdit_19;
    QLineEdit *lineEdit_20;
    QCheckBox *checkBox;
    QLabel *label_connect2;

    void setupUi(QWidget *ConnectDevice)
    {
        if (ConnectDevice->objectName().isEmpty())
            ConnectDevice->setObjectName(QStringLiteral("ConnectDevice"));
        ConnectDevice->setWindowModality(Qt::WindowModal);
        ConnectDevice->resize(557, 313);
        tabWidget = new QTabWidget(ConnectDevice);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(10, 20, 531, 281));
        onedevice = new QWidget();
        onedevice->setObjectName(QStringLiteral("onedevice"));
        pushButton = new QPushButton(onedevice);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(220, 140, 93, 28));
        pushButton_2 = new QPushButton(onedevice);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(330, 140, 93, 28));
        label = new QLabel(onedevice);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 30, 61, 21));
        label_4 = new QLabel(onedevice);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 80, 51, 21));
        lineEdit = new QLineEdit(onedevice);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(100, 30, 31, 21));
        lineEdit_2 = new QLineEdit(onedevice);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(150, 30, 31, 21));
        lineEdit_3 = new QLineEdit(onedevice);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(200, 30, 31, 21));
        lineEdit_4 = new QLineEdit(onedevice);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(250, 30, 31, 21));
        lineEdit_21 = new QLineEdit(onedevice);
        lineEdit_21->setObjectName(QStringLiteral("lineEdit_21"));
        lineEdit_21->setGeometry(QRect(100, 80, 61, 21));
        label_connect1 = new QLabel(onedevice);
        label_connect1->setObjectName(QStringLiteral("label_connect1"));
        label_connect1->setGeometry(QRect(20, 210, 71, 16));
        tabWidget->addTab(onedevice, QString());
        twodevice = new QWidget();
        twodevice->setObjectName(QStringLiteral("twodevice"));
        pushButton_3 = new QPushButton(twodevice);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(260, 180, 93, 28));
        pushButton_4 = new QPushButton(twodevice);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(370, 180, 93, 28));
        label_5 = new QLabel(twodevice);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 10, 61, 21));
        label_6 = new QLabel(twodevice);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 40, 31, 21));
        label_7 = new QLabel(twodevice);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 70, 61, 21));
        label_8 = new QLabel(twodevice);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 100, 31, 21));
        lineEdit_9 = new QLineEdit(twodevice);
        lineEdit_9->setObjectName(QStringLiteral("lineEdit_9"));
        lineEdit_9->setGeometry(QRect(40, 40, 31, 21));
        lineEdit_10 = new QLineEdit(twodevice);
        lineEdit_10->setObjectName(QStringLiteral("lineEdit_10"));
        lineEdit_10->setGeometry(QRect(80, 40, 31, 21));
        lineEdit_11 = new QLineEdit(twodevice);
        lineEdit_11->setObjectName(QStringLiteral("lineEdit_11"));
        lineEdit_11->setGeometry(QRect(120, 40, 31, 21));
        lineEdit_12 = new QLineEdit(twodevice);
        lineEdit_12->setObjectName(QStringLiteral("lineEdit_12"));
        lineEdit_12->setGeometry(QRect(160, 40, 31, 21));
        lineEdit_13 = new QLineEdit(twodevice);
        lineEdit_13->setObjectName(QStringLiteral("lineEdit_13"));
        lineEdit_13->setGeometry(QRect(40, 100, 31, 21));
        lineEdit_14 = new QLineEdit(twodevice);
        lineEdit_14->setObjectName(QStringLiteral("lineEdit_14"));
        lineEdit_14->setGeometry(QRect(80, 100, 31, 21));
        lineEdit_15 = new QLineEdit(twodevice);
        lineEdit_15->setObjectName(QStringLiteral("lineEdit_15"));
        lineEdit_15->setGeometry(QRect(120, 100, 31, 21));
        lineEdit_16 = new QLineEdit(twodevice);
        lineEdit_16->setObjectName(QStringLiteral("lineEdit_16"));
        lineEdit_16->setGeometry(QRect(160, 100, 31, 21));
        label_15 = new QLabel(twodevice);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(200, 40, 41, 21));
        label_16 = new QLabel(twodevice);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(200, 100, 41, 21));
        lineEdit_17 = new QLineEdit(twodevice);
        lineEdit_17->setObjectName(QStringLiteral("lineEdit_17"));
        lineEdit_17->setGeometry(QRect(240, 40, 61, 21));
        lineEdit_18 = new QLineEdit(twodevice);
        lineEdit_18->setObjectName(QStringLiteral("lineEdit_18"));
        lineEdit_18->setGeometry(QRect(240, 100, 61, 21));
        label_17 = new QLabel(twodevice);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(310, 40, 71, 21));
        label_18 = new QLabel(twodevice);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(310, 100, 71, 21));
        lineEdit_19 = new QLineEdit(twodevice);
        lineEdit_19->setObjectName(QStringLiteral("lineEdit_19"));
        lineEdit_19->setGeometry(QRect(380, 40, 61, 21));
        lineEdit_20 = new QLineEdit(twodevice);
        lineEdit_20->setObjectName(QStringLiteral("lineEdit_20"));
        lineEdit_20->setGeometry(QRect(380, 100, 61, 21));
        checkBox = new QCheckBox(twodevice);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setEnabled(true);
        checkBox->setGeometry(QRect(320, 140, 121, 20));
        checkBox->setChecked(true);
        label_connect2 = new QLabel(twodevice);
        label_connect2->setObjectName(QStringLiteral("label_connect2"));
        label_connect2->setGeometry(QRect(10, 220, 71, 16));
        tabWidget->addTab(twodevice, QString());

        retranslateUi(ConnectDevice);
        QObject::connect(pushButton, SIGNAL(clicked()), ConnectDevice, SLOT(OnBnClickedOk1()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), ConnectDevice, SLOT(OnBnClickedOk2()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), ConnectDevice, SLOT(close()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), ConnectDevice, SLOT(close()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ConnectDevice);
    } // setupUi

    void retranslateUi(QWidget *ConnectDevice)
    {
        ConnectDevice->setWindowTitle(QApplication::translate("ConnectDevice", "Form", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ConnectDevice", "OK", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("ConnectDevice", "Cancel", Q_NULLPTR));
        label->setText(QApplication::translate("ConnectDevice", "IP\345\234\260\345\235\200\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("ConnectDevice", "\347\253\257\345\217\243\357\274\232", Q_NULLPTR));
        lineEdit->setText(QApplication::translate("ConnectDevice", "10", Q_NULLPTR));
        lineEdit_2->setText(QApplication::translate("ConnectDevice", "111", Q_NULLPTR));
        lineEdit_3->setText(QApplication::translate("ConnectDevice", "228", Q_NULLPTR));
        lineEdit_4->setText(QApplication::translate("ConnectDevice", "194", Q_NULLPTR));
        lineEdit_21->setText(QApplication::translate("ConnectDevice", "24691", Q_NULLPTR));
        label_connect1->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(onedevice), QApplication::translate("ConnectDevice", "\345\215\225\350\256\276\345\244\207", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("ConnectDevice", "OK", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("ConnectDevice", "Cancel", Q_NULLPTR));
        label_5->setText(QApplication::translate("ConnectDevice", "\350\256\276\345\244\2071\357\274\232", Q_NULLPTR));
        label_6->setText(QApplication::translate("ConnectDevice", "IP\357\274\232", Q_NULLPTR));
        label_7->setText(QApplication::translate("ConnectDevice", "\350\256\276\345\244\2072\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("ConnectDevice", "IP\357\274\232", Q_NULLPTR));
        lineEdit_9->setText(QApplication::translate("ConnectDevice", "10", Q_NULLPTR));
        lineEdit_10->setText(QApplication::translate("ConnectDevice", "111", Q_NULLPTR));
        lineEdit_11->setText(QApplication::translate("ConnectDevice", "228", Q_NULLPTR));
        lineEdit_12->setText(QApplication::translate("ConnectDevice", "194", Q_NULLPTR));
        lineEdit_13->setText(QApplication::translate("ConnectDevice", "10", Q_NULLPTR));
        lineEdit_14->setText(QApplication::translate("ConnectDevice", "111", Q_NULLPTR));
        lineEdit_15->setText(QApplication::translate("ConnectDevice", "228", Q_NULLPTR));
        lineEdit_16->setText(QApplication::translate("ConnectDevice", "197", Q_NULLPTR));
        label_15->setText(QApplication::translate("ConnectDevice", "\347\253\257\345\217\243\357\274\232", Q_NULLPTR));
        label_16->setText(QApplication::translate("ConnectDevice", "\347\253\257\345\217\243\357\274\232", Q_NULLPTR));
        lineEdit_17->setText(QApplication::translate("ConnectDevice", "24691", Q_NULLPTR));
        lineEdit_18->setText(QApplication::translate("ConnectDevice", "24691", Q_NULLPTR));
        label_17->setText(QApplication::translate("ConnectDevice", "\351\253\230\351\200\237\347\253\257\345\217\243\357\274\232", Q_NULLPTR));
        label_18->setText(QApplication::translate("ConnectDevice", "\351\253\230\351\200\237\347\253\257\345\217\243\357\274\232", Q_NULLPTR));
        lineEdit_19->setText(QApplication::translate("ConnectDevice", "24692", Q_NULLPTR));
        lineEdit_20->setText(QApplication::translate("ConnectDevice", "24692", Q_NULLPTR));
        checkBox->setText(QApplication::translate("ConnectDevice", "\351\253\230\351\200\237\346\225\260\346\215\256\351\200\232\344\277\241", Q_NULLPTR));
        label_connect2->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(twodevice), QApplication::translate("ConnectDevice", "\344\270\244\345\217\260\350\256\276\345\244\207", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ConnectDevice: public Ui_ConnectDevice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTDEVICE_H
