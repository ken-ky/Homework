#include "ui_frame.h"
#include "word_exam.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>

WordsAssembler::WordsAssembler(QWidget *parent)
        : QMainWindow(parent) {
    ui.setupUi(this);

    // 创建一个垂直布局管理器
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建选择文件按钮
    loadFileButton = new QPushButton("选择文件", this);
    //绑定选择函数文件
    loadFileButton->setGeometry(20, 20, 100, 30);
    connect(loadFileButton, SIGNAL(clicked()), this, SLOT(chooseFile()));


    // 创建一个标签用于显示文件名
    selectedFileLabel = new QLabel("选择的文件：", this);

    // 创建分析文件按钮
    analyseButton = new QPushButton("开始分析", this);
    //绑定选择函数文件
    analyseButton->setGeometry(20, 20, 100, 30);
    connect(analyseButton, SIGNAL(clicked()), this, SLOT(ShowResult()));

    // 创建输出内容文本区域
    outputTextEdit = new QTextEdit(this);
    outputTextEdit->setGeometry(20, 70, 400, 300);
    // 设置 outputTextEdit 为只读
    outputTextEdit->setReadOnly(true);

    // 将按钮、标签和文本区域添加到水平布局管理器
    mainLayout->addWidget(loadFileButton);
    mainLayout->addWidget(selectedFileLabel);
    mainLayout->addWidget(analyseButton);
    mainLayout->addWidget(outputTextEdit);

    // 设置布局管理器为主窗口的中央部件
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    //构造mapKey
    MakeMap();
}

//选择函数文件
void WordsAssembler::chooseFile() {
    QFileDialog dialog(this);
    dialog.setWindowTitle("选择.cpp文件");
    dialog.setFileMode(QFileDialog::ExistingFile);

    // 设置筛选条件，只允许选择.cpp文件
    dialog.setNameFilter("C++ Files (*.cpp)");

    if (dialog.exec()) {
        QStringList selectedFiles = dialog.selectedFiles();
        if (!selectedFiles.isEmpty()) {
            QString filePath = selectedFiles.first();

            // 检查所选文件的扩展名是否为.cpp
            QFileInfo fileInfo(filePath);
            if (fileInfo.suffix().toLower() == "cpp" || fileInfo.suffix().toLower() == "txt") {
                // 更新标签以显示选择的文件路径和名称
                selectedFileLabel->setText("选择的文件：" + filePath);

                // 此处你可以保存文件路径供后续使用
                //调用读取cpp文件函数    filePath.toStdString()  QString-->string
                article = Read_code(filePath.toStdString());


                bool row_mark = false;
                for (auto line: article) {
                    if (line[0] == '#') {
                        res.emplace_back("<div>");
                        GetToken_defined(res, line);
                        res.emplace_back("</div>");
                    } else if (row_mark) {
                        GetToken_mark(res, line, row_mark);
                    } else {
                        res.emplace_back("<div>");
                        GetToken_normal(res, line, row_mark);
                        if(row_mark==0) res.emplace_back("</div>");
                    }
                }
            } else {
                // 显示错误提示
                QMessageBox::critical(this, "错误", "请选择一个.cpp文件！");
                exit(1);
            }
        }
    }
}

void WordsAssembler::ShowResult() {
    string s;
    for (auto re: res) s += re;
    outputTextEdit->setText(QString::fromStdString(s));
    outputTextEdit->show();
    cout << s << endl;
    res.clear();
}