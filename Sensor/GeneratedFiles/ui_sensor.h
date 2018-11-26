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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SensorClass
{
public:
    QPushButton *pushButton_3;
    QPushButton *pushButton_16;
    QPushButton *pushButton_17;

    void setupUi(QWidget *SensorClass)
    {
        if (SensorClass->objectName().isEmpty())
            SensorClass->setObjectName(QStringLiteral("SensorClass"));
        SensorClass->resize(481, 339);
        pushButton_3 = new QPushButton(SensorClass);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(50, 70, 111, 28));
        pushButton_16 = new QPushButton(SensorClass);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));
        pushButton_16->setGeometry(QRect(190, 70, 93, 28));
        pushButton_17 = new QPushButton(SensorClass);
        pushButton_17->setObjectName(QStringLiteral("pushButton_17"));
        pushButton_17->setGeometry(QRect(320, 70, 93, 28));

        retranslateUi(SensorClass);
        QObject::connect(pushButton_3, SIGNAL(clicked()), SensorClass, SLOT(OnOpen()));
        QObject::connect(pushButton_16, SIGNAL(clicked()), SensorClass, SLOT(OnClose()));

        QMetaObject::connectSlotsByName(SensorClass);
    } // setupUi

    void retranslateUi(QWidget *SensorClass)
    {
        SensorClass->setWindowTitle(QApplication::translate("SensorClass", "Sensor", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("SensorClass", "\350\277\236\346\216\245\350\256\276\345\244\207", Q_NULLPTR));
        pushButton_16->setText(QApplication::translate("SensorClass", "\345\205\263\351\227\255\350\256\276\345\244\207", Q_NULLPTR));
        pushButton_17->setText(QApplication::translate("SensorClass", "\351\253\230\351\200\237\350\275\256\345\273\223\345\200\274", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SensorClass: public Ui_SensorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENSOR_H
