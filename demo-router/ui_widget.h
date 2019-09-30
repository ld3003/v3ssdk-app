/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QLabel *labelonline;
    QLabel *labelonlinecsq;
    QLabel *labelonlinepic;
    QLabel *labelonlinecops;
    QLabel *labelband;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *verticalLayout_7;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(240, 320);
        Widget->setMinimumSize(QSize(240, 320));
        Widget->setMaximumSize(QSize(240, 320));
        verticalLayout_2 = new QVBoxLayout(Widget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 147));
        groupBox->setMaximumSize(QSize(16777215, 147));
        verticalLayout_6 = new QVBoxLayout(groupBox);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        labelonline = new QLabel(groupBox);
        labelonline->setObjectName(QString::fromUtf8("labelonline"));
        labelonline->setMaximumSize(QSize(180, 16777215));

        verticalLayout_3->addWidget(labelonline);

        labelonlinecsq = new QLabel(groupBox);
        labelonlinecsq->setObjectName(QString::fromUtf8("labelonlinecsq"));
        labelonlinecsq->setMaximumSize(QSize(180, 16777215));

        verticalLayout_3->addWidget(labelonlinecsq);


        horizontalLayout->addLayout(verticalLayout_3);

        labelonlinepic = new QLabel(groupBox);
        labelonlinepic->setObjectName(QString::fromUtf8("labelonlinepic"));
        labelonlinepic->setMinimumSize(QSize(64, 36));
        labelonlinepic->setMaximumSize(QSize(63, 36));
        labelonlinepic->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(labelonlinepic);


        verticalLayout_4->addLayout(horizontalLayout);

        labelonlinecops = new QLabel(groupBox);
        labelonlinecops->setObjectName(QString::fromUtf8("labelonlinecops"));

        verticalLayout_4->addWidget(labelonlinecops);


        verticalLayout_5->addLayout(verticalLayout_4);

        labelband = new QLabel(groupBox);
        labelband->setObjectName(QString::fromUtf8("labelband"));

        verticalLayout_5->addWidget(labelband);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        verticalLayout_6->addLayout(verticalLayout_5);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(Widget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(0, 147));
        groupBox_2->setMaximumSize(QSize(16777215, 147));
        verticalLayout_8 = new QVBoxLayout(groupBox_2);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_7->addWidget(label);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_7->addWidget(label_2);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_7->addWidget(label_3);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_2);


        verticalLayout_8->addLayout(verticalLayout_7);


        verticalLayout->addWidget(groupBox_2);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Widget", "4g info", 0, QApplication::UnicodeUTF8));
        labelonline->setText(QApplication::translate("Widget", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelonlinecsq->setText(QApplication::translate("Widget", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelonlinepic->setText(QString());
        labelonlinecops->setText(QApplication::translate("Widget", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelband->setText(QApplication::translate("Widget", "TextLabel", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Widget", "wifi info", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Widget", "ssid:V3S-ESP8089", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Widget", "password:88888888", 0, QApplication::UnicodeUTF8));
        label_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
