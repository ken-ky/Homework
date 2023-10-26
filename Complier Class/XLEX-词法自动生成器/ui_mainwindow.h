/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionopen_file;
    QWidget *centralwidget;
    QFrame *frame;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QTableWidget *NFA_t;
    QGroupBox *groupBox_3;
    QTableWidget *DFA_t;
    QGroupBox *groupBox_4;
    QTableWidget *MINI_t;
    QGroupBox *groupBox_5;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QTextBrowser *Buid_code;
    QGroupBox *groupBox;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_3;
    QTextBrowser *Input_text;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QPushButton *clean_t;
    QPushButton *rFile;
    QPushButton *build_exp;
    QSpacerItem *verticalSpacer_2;
    QPushButton *sFile;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1500, 800);
        MainWindow->setMinimumSize(QSize(1500, 800));
        MainWindow->setMaximumSize(QSize(1500, 800));
        actionopen_file = new QAction(MainWindow);
        actionopen_file->setObjectName(QString::fromUtf8("actionopen_file"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 1261, 771));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayoutWidget = new QWidget(frame);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(0, 0, 1261, 771));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        groupBox_2 = new QGroupBox(gridLayoutWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        NFA_t = new QTableWidget(groupBox_2);
        NFA_t->setObjectName(QString::fromUtf8("NFA_t"));
        NFA_t->setGeometry(QRect(0, 20, 621, 361));

        gridLayout->addWidget(groupBox_2, 0, 2, 1, 1);

        groupBox_3 = new QGroupBox(gridLayoutWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        DFA_t = new QTableWidget(groupBox_3);
        DFA_t->setObjectName(QString::fromUtf8("DFA_t"));
        DFA_t->setGeometry(QRect(0, 20, 621, 361));

        gridLayout->addWidget(groupBox_3, 0, 3, 1, 1);

        groupBox_4 = new QGroupBox(gridLayoutWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        MINI_t = new QTableWidget(groupBox_4);
        MINI_t->setObjectName(QString::fromUtf8("MINI_t"));
        MINI_t->setGeometry(QRect(0, 20, 621, 341));

        gridLayout->addWidget(groupBox_4, 1, 2, 1, 1);

        groupBox_5 = new QGroupBox(gridLayoutWidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        scrollArea = new QScrollArea(groupBox_5);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(0, 20, 621, 351));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 619, 349));
        Buid_code = new QTextBrowser(scrollAreaWidgetContents_2);
        Buid_code->setObjectName(QString::fromUtf8("Buid_code"));
        Buid_code->setGeometry(QRect(0, 0, 621, 351));
        scrollArea->setWidget(scrollAreaWidgetContents_2);

        gridLayout->addWidget(groupBox_5, 1, 3, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(1260, 0, 231, 751));
        scrollArea_2 = new QScrollArea(groupBox);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setGeometry(QRect(10, 20, 221, 311));
        scrollArea_2->setWidgetResizable(false);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 219, 249));
        Input_text = new QTextBrowser(scrollAreaWidgetContents_3);
        Input_text->setObjectName(QString::fromUtf8("Input_text"));
        Input_text->setGeometry(QRect(0, 0, 221, 551));
        Input_text->setReadOnly(false);
        scrollArea_2->setWidget(scrollAreaWidgetContents_3);
        gridLayoutWidget_2 = new QWidget(groupBox);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 360, 222, 80));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        clean_t = new QPushButton(gridLayoutWidget_2);
        clean_t->setObjectName(QString::fromUtf8("clean_t"));

        gridLayout_2->addWidget(clean_t, 1, 2, 1, 1);

        rFile = new QPushButton(gridLayoutWidget_2);
        rFile->setObjectName(QString::fromUtf8("rFile"));

        gridLayout_2->addWidget(rFile, 0, 2, 1, 1);

        build_exp = new QPushButton(gridLayoutWidget_2);
        build_exp->setObjectName(QString::fromUtf8("build_exp"));

        gridLayout_2->addWidget(build_exp, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 1, 1, 1, 1);

        sFile = new QPushButton(gridLayoutWidget_2);
        sFile->setObjectName(QString::fromUtf8("sFile"));

        gridLayout_2->addWidget(sFile, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1500, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionopen_file->setText(QCoreApplication::translate("MainWindow", "open file", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "NFA Graph", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "DFA Graph", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Mini-DFA Graph", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "Build Code", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Input Expression", nullptr));
        Input_text->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Please input RE_expression...</p></body></html>", nullptr));
        clean_t->setText(QCoreApplication::translate("MainWindow", "clean", nullptr));
        rFile->setText(QCoreApplication::translate("MainWindow", "read", nullptr));
        build_exp->setText(QCoreApplication::translate("MainWindow", "build", nullptr));
        sFile->setText(QCoreApplication::translate("MainWindow", "save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
