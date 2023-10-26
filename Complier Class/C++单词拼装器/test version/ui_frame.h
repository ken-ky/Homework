#ifndef UI_FRAME_H
#define UI_FRAME_H

#include "ui_WordsAssembler.h"
#include "word_exam.h"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QLabel>

QT_BEGIN_NAMESPACE

class WordsAssembler : public QMainWindow {
Q_OBJECT
public:
    vector<string> res;

    WordsAssembler(QWidget *parent);

    ~WordsAssembler() {}

private slots:

    void chooseFile();// 处理选择文件按钮点击事件
    void ShowResult();  //处理开始分析按钮点击事件,展示分析结果
private:
    Ui::WordsAssemblerClass ui;
    QPushButton *loadFileButton; // 读取文件按钮
    QTextEdit *outputTextEdit;   // 输出内容文本区域
    QLabel *selectedFileLabel; // 用于显示选择的文件路径和名称
    QPushButton *analyseButton; // 分析文件按钮
};

QT_END_NAMESPACE

#endif // UI_FRAME_H
