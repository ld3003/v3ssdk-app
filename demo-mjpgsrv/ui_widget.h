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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelport_2;
    QTextEdit *textEdit;
    QLabel *labelport;
    QLabel *labelport_3;
    QLabel *labelport_4;
    QLabel *labelport_5;
    QLabel *labelstat;
    QSpacerItem *verticalSpacer;

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
        labelport_2 = new QLabel(Widget);
        labelport_2->setObjectName(QString::fromUtf8("labelport_2"));

        verticalLayout->addWidget(labelport_2);

        textEdit = new QTextEdit(Widget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setEnabled(true);
        textEdit->setMaximumSize(QSize(16777215, 90));
        textEdit->setFocusPolicy(Qt::NoFocus);
        textEdit->setStyleSheet(QString::fromUtf8("font: 75 12pt \"Algerian\";"));

        verticalLayout->addWidget(textEdit);

        labelport = new QLabel(Widget);
        labelport->setObjectName(QString::fromUtf8("labelport"));

        verticalLayout->addWidget(labelport);

        labelport_3 = new QLabel(Widget);
        labelport_3->setObjectName(QString::fromUtf8("labelport_3"));

        verticalLayout->addWidget(labelport_3);

        labelport_4 = new QLabel(Widget);
        labelport_4->setObjectName(QString::fromUtf8("labelport_4"));

        verticalLayout->addWidget(labelport_4);

        labelport_5 = new QLabel(Widget);
        labelport_5->setObjectName(QString::fromUtf8("labelport_5"));

        verticalLayout->addWidget(labelport_5);

        labelstat = new QLabel(Widget);
        labelstat->setObjectName(QString::fromUtf8("labelstat"));
        labelstat->setMinimumSize(QSize(0, 24));
        labelstat->setMaximumSize(QSize(16777215, 24));

        verticalLayout->addWidget(labelstat);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0, QApplication::UnicodeUTF8));
        labelport_2->setText(QApplication::translate("Widget", "server ip list:", 0, QApplication::UnicodeUTF8));
        labelport->setText(QApplication::translate("Widget", "server port:", 0, QApplication::UnicodeUTF8));
        labelport_3->setText(QApplication::translate("Widget", "use browser with any ip in list and ", 0, QApplication::UnicodeUTF8));
        labelport_4->setText(QApplication::translate("Widget", "port to get vedio as:", 0, QApplication::UnicodeUTF8));
        labelport_5->setText(QApplication::translate("Widget", "192.168.2.138:7980", 0, QApplication::UnicodeUTF8));
        labelstat->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
