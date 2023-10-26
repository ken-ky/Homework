#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

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
    void on_sFile_clicked();

    void on_rFile_clicked();

    void on_build_exp_clicked();

    void on_clean_t_clicked();

    void on_actionopen_file_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
