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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *openFile;
    QWidget *centralwidget;
    QTabWidget *Graphs;
    QWidget *reExps;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTextBrowser *reRules;
    QWidget *nfa;
    QWidget *verticalLayoutWidget_6;
    QVBoxLayout *nfaLayout;
    QTableView *nfaTab;
    QWidget *nfaMenu;
    QComboBox *nfaSel;
    QLabel *label;
    QWidget *dfa;
    QWidget *verticalLayoutWidget_7;
    QVBoxLayout *dfaLayout;
    QTableView *dfaTab;
    QWidget *dfaMenu;
    QComboBox *dfaSel;
    QLabel *label_2;
    QWidget *minDfa;
    QWidget *verticalLayoutWidget_8;
    QVBoxLayout *minDfaLayout;
    QTableView *minDfaTab;
    QFrame *frame;
    QLabel *label_3;
    QComboBox *minDfaSel;
    QWidget *cppCode;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QTextBrowser *cppText;
    QWidget *tnyFile;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QTextBrowser *tnyCode;
    QWidget *lexFile;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_4;
    QTextBrowser *lexRes;
    QWidget *verticalLayoutWidget_5;
    QVBoxLayout *verticalLayout_5;
    QWidget *widget;
    QGroupBox *terminal;
    QTextBrowser *terLines;
    QWidget *widget_2;
    QWidget *widget_3;
    QGroupBox *Menu;
    QWidget *widget_4;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QWidget *widget_5;
    QPushButton *transNFA;
    QPushButton *minimizeDFA;
    QWidget *widget_6;
    QPushButton *transDFA;
    QPushButton *produceCPP;
    QPushButton *saveLex;
    QPushButton *clearText;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(1080, 720);
        MainWindow->setMinimumSize(QSize(1080, 720));
        MainWindow->setMaximumSize(QSize(1080, 720));
        openFile = new QAction(MainWindow);
        openFile->setObjectName(QString::fromUtf8("openFile"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        Graphs = new QTabWidget(centralwidget);
        Graphs->setObjectName(QString::fromUtf8("Graphs"));
        Graphs->setGeometry(QRect(5, 8, 560, 660));
        Graphs->setMinimumSize(QSize(560, 660));
        Graphs->setMaximumSize(QSize(560, 660));
        reExps = new QWidget();
        reExps->setObjectName(QString::fromUtf8("reExps"));
        verticalLayoutWidget = new QWidget(reExps);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(-1, 9, 551, 621));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        reRules = new QTextBrowser(verticalLayoutWidget);
        reRules->setObjectName(QString::fromUtf8("reRules"));

        verticalLayout->addWidget(reRules);

        Graphs->addTab(reExps, QString());
        nfa = new QWidget();
        nfa->setObjectName(QString::fromUtf8("nfa"));
        verticalLayoutWidget_6 = new QWidget(nfa);
        verticalLayoutWidget_6->setObjectName(QString::fromUtf8("verticalLayoutWidget_6"));
        verticalLayoutWidget_6->setGeometry(QRect(0, 170, 551, 461));
        nfaLayout = new QVBoxLayout(verticalLayoutWidget_6);
        nfaLayout->setObjectName(QString::fromUtf8("nfaLayout"));
        nfaLayout->setContentsMargins(0, 0, 0, 0);
        nfaTab = new QTableView(verticalLayoutWidget_6);
        nfaTab->setObjectName(QString::fromUtf8("nfaTab"));

        nfaLayout->addWidget(nfaTab);

        nfaMenu = new QWidget(nfa);
        nfaMenu->setObjectName(QString::fromUtf8("nfaMenu"));
        nfaMenu->setGeometry(QRect(0, 10, 551, 151));
        nfaSel = new QComboBox(nfaMenu);
        nfaSel->addItem(QString());
        nfaSel->setObjectName(QString::fromUtf8("nfaSel"));
        nfaSel->setGeometry(QRect(220, 30, 281, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Adobe Arabic"));
        font.setPointSize(14);
        nfaSel->setFont(font);
        label = new QLabel(nfaMenu);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 30, 161, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Adobe Arabic"));
        font1.setPointSize(20);
        label->setFont(font1);
        Graphs->addTab(nfa, QString());
        dfa = new QWidget();
        dfa->setObjectName(QString::fromUtf8("dfa"));
        verticalLayoutWidget_7 = new QWidget(dfa);
        verticalLayoutWidget_7->setObjectName(QString::fromUtf8("verticalLayoutWidget_7"));
        verticalLayoutWidget_7->setGeometry(QRect(0, 170, 551, 461));
        dfaLayout = new QVBoxLayout(verticalLayoutWidget_7);
        dfaLayout->setObjectName(QString::fromUtf8("dfaLayout"));
        dfaLayout->setContentsMargins(0, 0, 0, 0);
        dfaTab = new QTableView(verticalLayoutWidget_7);
        dfaTab->setObjectName(QString::fromUtf8("dfaTab"));

        dfaLayout->addWidget(dfaTab);

        dfaMenu = new QWidget(dfa);
        dfaMenu->setObjectName(QString::fromUtf8("dfaMenu"));
        dfaMenu->setGeometry(QRect(0, 10, 551, 151));
        dfaSel = new QComboBox(dfaMenu);
        dfaSel->addItem(QString());
        dfaSel->setObjectName(QString::fromUtf8("dfaSel"));
        dfaSel->setGeometry(QRect(220, 30, 281, 31));
        dfaSel->setFont(font);
        label_2 = new QLabel(dfaMenu);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 30, 161, 31));
        label_2->setFont(font1);
        Graphs->addTab(dfa, QString());
        minDfa = new QWidget();
        minDfa->setObjectName(QString::fromUtf8("minDfa"));
        verticalLayoutWidget_8 = new QWidget(minDfa);
        verticalLayoutWidget_8->setObjectName(QString::fromUtf8("verticalLayoutWidget_8"));
        verticalLayoutWidget_8->setGeometry(QRect(0, 170, 551, 461));
        minDfaLayout = new QVBoxLayout(verticalLayoutWidget_8);
        minDfaLayout->setObjectName(QString::fromUtf8("minDfaLayout"));
        minDfaLayout->setContentsMargins(0, 0, 0, 0);
        minDfaTab = new QTableView(verticalLayoutWidget_8);
        minDfaTab->setObjectName(QString::fromUtf8("minDfaTab"));

        minDfaLayout->addWidget(minDfaTab);

        frame = new QFrame(minDfa);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 10, 551, 151));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 30, 181, 31));
        label_3->setFont(font1);
        minDfaSel = new QComboBox(frame);
        minDfaSel->addItem(QString());
        minDfaSel->setObjectName(QString::fromUtf8("minDfaSel"));
        minDfaSel->setGeometry(QRect(220, 30, 281, 31));
        minDfaSel->setFont(font);
        Graphs->addTab(minDfa, QString());
        cppCode = new QWidget();
        cppCode->setObjectName(QString::fromUtf8("cppCode"));
        verticalLayoutWidget_2 = new QWidget(cppCode);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(0, 10, 551, 621));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        cppText = new QTextBrowser(verticalLayoutWidget_2);
        cppText->setObjectName(QString::fromUtf8("cppText"));

        verticalLayout_2->addWidget(cppText);

        Graphs->addTab(cppCode, QString());
        tnyFile = new QWidget();
        tnyFile->setObjectName(QString::fromUtf8("tnyFile"));
        verticalLayoutWidget_3 = new QWidget(tnyFile);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(0, 10, 551, 621));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        tnyCode = new QTextBrowser(verticalLayoutWidget_3);
        tnyCode->setObjectName(QString::fromUtf8("tnyCode"));

        verticalLayout_3->addWidget(tnyCode);

        Graphs->addTab(tnyFile, QString());
        lexFile = new QWidget();
        lexFile->setObjectName(QString::fromUtf8("lexFile"));
        verticalLayoutWidget_4 = new QWidget(lexFile);
        verticalLayoutWidget_4->setObjectName(QString::fromUtf8("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(0, 10, 551, 621));
        verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        lexRes = new QTextBrowser(verticalLayoutWidget_4);
        lexRes->setObjectName(QString::fromUtf8("lexRes"));

        verticalLayout_4->addWidget(lexRes);

        Graphs->addTab(lexFile, QString());
        verticalLayoutWidget_5 = new QWidget(centralwidget);
        verticalLayoutWidget_5->setObjectName(QString::fromUtf8("verticalLayoutWidget_5"));
        verticalLayoutWidget_5->setGeometry(QRect(600, 30, 461, 631));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_5);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(verticalLayoutWidget_5);
        widget->setObjectName(QString::fromUtf8("widget"));
        terminal = new QGroupBox(widget);
        terminal->setObjectName(QString::fromUtf8("terminal"));
        terminal->setGeometry(QRect(-1, -1, 461, 311));
        terLines = new QTextBrowser(terminal);
        terLines->setObjectName(QString::fromUtf8("terLines"));
        terLines->setGeometry(QRect(0, 20, 460, 280));
        terLines->setMinimumSize(QSize(460, 280));
        terLines->setMaximumSize(QSize(460, 280));

        verticalLayout_5->addWidget(widget);

        widget_2 = new QWidget(verticalLayoutWidget_5);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(0, 0, 461, 311));
        Menu = new QGroupBox(widget_3);
        Menu->setObjectName(QString::fromUtf8("Menu"));
        Menu->setGeometry(QRect(0, 0, 461, 311));
        widget_4 = new QWidget(Menu);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setGeometry(QRect(10, 30, 441, 161));
        gridLayoutWidget = new QWidget(widget_4);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 0, 411, 161));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        widget_5 = new QWidget(gridLayoutWidget);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        transNFA = new QPushButton(widget_5);
        transNFA->setObjectName(QString::fromUtf8("transNFA"));
        transNFA->setGeometry(QRect(40, 40, 111, 28));
        minimizeDFA = new QPushButton(widget_5);
        minimizeDFA->setObjectName(QString::fromUtf8("minimizeDFA"));
        minimizeDFA->setGeometry(QRect(40, 100, 111, 28));

        gridLayout->addWidget(widget_5, 0, 0, 1, 1);

        widget_6 = new QWidget(gridLayoutWidget);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        transDFA = new QPushButton(widget_6);
        transDFA->setObjectName(QString::fromUtf8("transDFA"));
        transDFA->setGeometry(QRect(40, 40, 111, 28));
        produceCPP = new QPushButton(widget_6);
        produceCPP->setObjectName(QString::fromUtf8("produceCPP"));
        produceCPP->setGeometry(QRect(40, 100, 111, 28));

        gridLayout->addWidget(widget_6, 0, 1, 1, 1);

        saveLex = new QPushButton(Menu);
        saveLex->setObjectName(QString::fromUtf8("saveLex"));
        saveLex->setGeometry(QRect(270, 210, 111, 28));
        clearText = new QPushButton(Menu);
        clearText->setObjectName(QString::fromUtf8("clearText"));
        clearText->setGeometry(QRect(60, 210, 111, 28));

        verticalLayout_5->addWidget(widget_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1080, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(openFile);

        retranslateUi(MainWindow);

        Graphs->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        openFile->setText(QCoreApplication::translate("MainWindow", "open file", nullptr));
        Graphs->setTabText(Graphs->indexOf(reExps), QCoreApplication::translate("MainWindow", "RE exps", nullptr));
        nfaSel->setItemText(0, QCoreApplication::translate("MainWindow", "None", nullptr));

        label->setText(QCoreApplication::translate("MainWindow", "NFA selection:", nullptr));
        Graphs->setTabText(Graphs->indexOf(nfa), QCoreApplication::translate("MainWindow", "nfa", nullptr));
        dfaSel->setItemText(0, QCoreApplication::translate("MainWindow", "None", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "DFA selection:", nullptr));
        Graphs->setTabText(Graphs->indexOf(dfa), QCoreApplication::translate("MainWindow", "dfa", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "minDFA selection:", nullptr));
        minDfaSel->setItemText(0, QCoreApplication::translate("MainWindow", "None", nullptr));

        Graphs->setTabText(Graphs->indexOf(minDfa), QCoreApplication::translate("MainWindow", "minDfa", nullptr));
        Graphs->setTabText(Graphs->indexOf(cppCode), QCoreApplication::translate("MainWindow", ".cpp", nullptr));
        Graphs->setTabText(Graphs->indexOf(tnyFile), QCoreApplication::translate("MainWindow", ".tny", nullptr));
        Graphs->setTabText(Graphs->indexOf(lexFile), QCoreApplication::translate("MainWindow", ".lex", nullptr));
        terminal->setTitle(QCoreApplication::translate("MainWindow", "Terminal", nullptr));
        Menu->setTitle(QCoreApplication::translate("MainWindow", "Tool Menu", nullptr));
        transNFA->setText(QCoreApplication::translate("MainWindow", "RE -> NFA", nullptr));
        minimizeDFA->setText(QCoreApplication::translate("MainWindow", "minimize DFA", nullptr));
        transDFA->setText(QCoreApplication::translate("MainWindow", "NFA -> DFA", nullptr));
        produceCPP->setText(QCoreApplication::translate("MainWindow", "Code produce", nullptr));
        saveLex->setText(QCoreApplication::translate("MainWindow", "Display .lex", nullptr));
        clearText->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
