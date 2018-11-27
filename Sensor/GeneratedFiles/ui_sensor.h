/********************************************************************************
** Form generated from reading UI file 'sensor.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENSOR_H
#define UI_SENSOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SensorClass
{
public:
    QPushButton *pushButton_3;
    QPushButton *pushButton_16;
    QPushButton *pushButton_17;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QTextEdit *textEdit_2;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *SensorClass)
    {
        if (SensorClass->objectName().isEmpty())
            SensorClass->setObjectName(QStringLiteral("SensorClass"));
        SensorClass->resize(481, 339);
        pushButton_3 = new QPushButton(SensorClass);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(20, 30, 111, 28));
        pushButton_16 = new QPushButton(SensorClass);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));
        pushButton_16->setGeometry(QRect(170, 30, 93, 28));
        pushButton_17 = new QPushButton(SensorClass);
        pushButton_17->setObjectName(QStringLiteral("pushButton_17"));
        pushButton_17->setGeometry(QRect(30, 230, 93, 28));
        pushButton = new QPushButton(SensorClass);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(22, 90, 111, 28));
        textEdit = new QTextEdit(SensorClass);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(150, 120, 104, 87));
        textEdit->setReadOnly(true);
        textEdit_2 = new QTextEdit(SensorClass);
        textEdit_2->setObjectName(QStringLiteral("textEdit_2"));
        textEdit_2->setGeometry(QRect(270, 120, 104, 87));
        textEdit_2->setReadOnly(true);
        label = new QLabel(SensorClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(170, 100, 72, 15));
        label_2 = new QLabel(SensorClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(300, 100, 72, 15));

        retranslateUi(SensorClass);
        QObject::connect(pushButton_3, SIGNAL(clicked()), SensorClass, SLOT(OnOpen()));
        QObject::connect(pushButton_16, SIGNAL(clicked()), SensorClass, SLOT(OnClose()));
        QObject::connect(pushButton, SIGNAL(clicked()), SensorClass, SLOT(OnGetcurrentvalue()));

        QMetaObject::connectSlotsByName(SensorClass);
    } // setupUi

    void retranslateUi(QWidget *SensorClass)
    {
        SensorClass->setWindowTitle(QApplication::translate("SensorClass", "Sensor", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("SensorClass", "\350\277\236\346\216\245\350\256\276\345\244\207", Q_NULLPTR));
        pushButton_16->setText(QApplication::translate("SensorClass", "\345\205\263\351\227\255\350\256\276\345\244\207", Q_NULLPTR));
        pushButton_17->setText(QApplication::translate("SensorClass", "\351\253\230\351\200\237\350\275\256\345\273\223\345\200\274", Q_NULLPTR));
        pushButton->setText(QApplication::translate("SensorClass", "\350\216\267\345\217\226\345\275\223\345\211\215\345\200\274", Q_NULLPTR));
        label->setText(QApplication::translate("SensorClass", "\346\216\247\345\210\266\345\231\250A", Q_NULLPTR));
        label_2->setText(QApplication::translate("SensorClass", "\346\216\247\345\210\266\345\231\250B", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SensorClass: public Ui_SensorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENSOR_H
