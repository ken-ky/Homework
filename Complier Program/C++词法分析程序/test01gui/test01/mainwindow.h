#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "global.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QWidget>
#include <QTableView>
#include <QStandardItem>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_transNFA_clicked();

    void on_transDFA_clicked();

    void on_minimizeDFA_clicked();

    void on_produceCPP_clicked();

    void on_saveLex_clicked();

    void on_clearText_clicked();

    void on_openFile_triggered();

    void on_nfaSel_currentTextChanged(const QString &arg1);

    void on_dfaSel_currentIndexChanged(const QString &arg1);

    void on_minDfaSel_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QString reRulePath; //reExps path
    QString cppPath;    //proCpp path
    QString tnyPath;    //tiny path
    QString lexPath;    //lex path

    codePro *code;      //生成代码结构体
    bool nfaFlag;
    bool dfaFlag;
    bool minFlag;

    bool cppFlag;   //生成cpp文件与否

    QMap<QString,int> selection;
};
#endif // MAINWINDOW_H
