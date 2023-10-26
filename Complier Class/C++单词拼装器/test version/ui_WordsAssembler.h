/********************************************************************************
** Form generated from reading UI file 'WordsAssembler.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORDSASSEMBLER_H
#define UI_WORDSASSEMBLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WordsAssemblerClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WordsAssemblerClass)
    {
        if (WordsAssemblerClass->objectName().isEmpty())
            WordsAssemblerClass->setObjectName(QString::fromUtf8("WordsAssemblerClass"));
        WordsAssemblerClass->resize(600, 400);
        centralWidget = new QWidget(WordsAssemblerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        WordsAssemblerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(WordsAssemblerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        WordsAssemblerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(WordsAssemblerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        WordsAssemblerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(WordsAssemblerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        WordsAssemblerClass->setStatusBar(statusBar);

        retranslateUi(WordsAssemblerClass);

        QMetaObject::connectSlotsByName(WordsAssemblerClass);
    } // setupUi

    void retranslateUi(QMainWindow *WordsAssemblerClass)
    {
        WordsAssemblerClass->setWindowTitle(QCoreApplication::translate("WordsAssemblerClass", "WordsAssembler", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WordsAssemblerClass: public Ui_WordsAssemblerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORDSASSEMBLER_H
