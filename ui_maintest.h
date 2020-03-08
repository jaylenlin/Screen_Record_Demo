/********************************************************************************
** Form generated from reading UI file 'maintest.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINTEST_H
#define UI_MAINTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainTest
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QLabel *label_2;
    QPushButton *m_grab_rect;
    QPushButton *m_grab_window;
    QPushButton *start;
    QPushButton *stop;
    QMenuBar *menuBar;
    QMenu *menuDemo;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainTest)
    {
        if (MainTest->objectName().isEmpty())
            MainTest->setObjectName(QStringLiteral("MainTest"));
        MainTest->resize(400, 300);
        centralWidget = new QWidget(MainTest);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 71, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 20, 361, 171));
        m_grab_rect = new QPushButton(centralWidget);
        m_grab_rect->setObjectName(QStringLiteral("m_grab_rect"));
        m_grab_rect->setGeometry(QRect(310, 180, 80, 26));
        m_grab_window = new QPushButton(centralWidget);
        m_grab_window->setObjectName(QStringLiteral("m_grab_window"));
        m_grab_window->setGeometry(QRect(210, 180, 91, 26));
        start = new QPushButton(centralWidget);
        start->setObjectName(QStringLiteral("start"));
        start->setGeometry(QRect(210, 210, 80, 26));
        stop = new QPushButton(centralWidget);
        stop->setObjectName(QStringLiteral("stop"));
        stop->setGeometry(QRect(310, 210, 80, 26));
        MainTest->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainTest);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 23));
        menuDemo = new QMenu(menuBar);
        menuDemo->setObjectName(QStringLiteral("menuDemo"));
        MainTest->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainTest);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainTest->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainTest);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainTest->setStatusBar(statusBar);

        menuBar->addAction(menuDemo->menuAction());

        retranslateUi(MainTest);

        QMetaObject::connectSlotsByName(MainTest);
    } // setupUi

    void retranslateUi(QMainWindow *MainTest)
    {
        MainTest->setWindowTitle(QApplication::translate("MainTest", "MainTest", Q_NULLPTR));
        label->setText(QApplication::translate("MainTest", "grab_0_0_1", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainTest", "                                                   Img is here", Q_NULLPTR));
        m_grab_rect->setText(QApplication::translate("MainTest", "Grab_Rect", Q_NULLPTR));
        m_grab_window->setText(QApplication::translate("MainTest", "Grab_window", Q_NULLPTR));
        start->setText(QApplication::translate("MainTest", "start", Q_NULLPTR));
        stop->setText(QApplication::translate("MainTest", "stop", Q_NULLPTR));
        menuDemo->setTitle(QApplication::translate("MainTest", "demo", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainTest: public Ui_MainTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINTEST_H
