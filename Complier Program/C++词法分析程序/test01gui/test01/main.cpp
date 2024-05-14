#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    codePro c("reRule.txt");
//    c.initCode("example.tny", "test.lex");
//    c.outCPP("outTestCode.cpp");

//    runCode("outTestCode.cpp", "test.exe", "", "");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
