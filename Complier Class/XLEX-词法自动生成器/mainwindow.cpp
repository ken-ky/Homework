#include "mainwindow.h"
#include "ctr_file.h"
#include "ui_mainwindow.h"
#include "REmodel.h"

string path = "test.txt";
vector<string> res;
RE_Exp *reExp;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_sFile_clicked() {
    QString content = ui->Input_text->toPlainText();
    string s = content.toStdString();
    edit_txt(s, path);
//    cout << "Writing complete!" << endl;
}

void MainWindow::on_rFile_clicked() {
    res = read_txt(path);

    int n = res.size();
    if (n == 0) {
        ui->Input_text->setText("");
        return;
    }

    QString qs;
    string exps = res[0];
    if (n == 1) {
        qs = QString::fromStdString(exps);
        ui->Input_text->setText(qs);
        return;
    }

    for (int i = 1; i < n; ++i) {
        exps += "\n";
        exps += res[i];
    }
    qs = QString::fromStdString(exps);
    ui->Input_text->setText(qs);
}


void MainWindow::on_build_exp_clicked() {
    on_clean_t_clicked();
    QString content = ui->Input_text->toPlainText();
    string s = content.toStdString();
    vector<string> vec = subStrToVec(s, '\n');
    s = merge_str(vec);
    init_map();

    reExp = new RE_Exp(s);
    reExp->initial_exp();

    int width = reExp->width, len_nfa = reExp->nfa_exp.size();
    int len_dfa = reExp->dfa_exp.graph.size();
    int len_mini = reExp->min_exp->stat_map.size();

    //表头设置
    QStringList HStrNFA, HStrDFA;

    HStrNFA.push_back(QString("\t"));
    HStrNFA.push_back(QString("\t"));

    HStrDFA.push_back(QString("\t"));
    HStrDFA.push_back(QString("\t"));

    for (int i = 0; i < width - 1; ++i) {
        HStrNFA.push_back(QString(reExp->title[i]));
        HStrDFA.push_back(QString(reExp->title[i]));
    }

    HStrNFA.push_back(QString(reExp->title[width - 1]));

    ui->NFA_t->setRowCount(len_nfa);
    ui->NFA_t->setColumnCount(width + 2);
    ui->NFA_t->setHorizontalHeaderLabels(HStrNFA);

    ui->DFA_t->setRowCount(len_dfa);
    ui->DFA_t->setColumnCount(width + 1);
    ui->DFA_t->setHorizontalHeaderLabels(HStrDFA);

    ui->MINI_t->setRowCount(len_mini);
    ui->MINI_t->setColumnCount(width + 1);
    ui->MINI_t->setHorizontalHeaderLabels(HStrDFA);

    //设置表格内容不可更改
    ui->NFA_t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->DFA_t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->MINI_t->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置表格选中颜色
    ui->NFA_t->setStyleSheet("selection-background-color:blue");
    ui->DFA_t->setStyleSheet("selection-background-color:green");
    ui->MINI_t->setStyleSheet("selection-background-color:red");

    //NFA表内内容设置
    string temp;
    QString item;
    NFA &nfa = reExp->nfa_exp;
    for (int i = 0; i < len_nfa; i++) {
        if (len_nfa == 1) ui->NFA_t->setItem(i, 0, new QTableWidgetItem("- +"));
        else if (!i) ui->NFA_t->setItem(i, 0, new QTableWidgetItem("-"));
        else if (i == len_nfa - 1) ui->NFA_t->setItem(i, 0, new QTableWidgetItem("+"));
        item = QString::fromStdString(to_string(i + 1));
        ui->NFA_t->setItem(i, 1, new QTableWidgetItem(item));
        for (int j = 0; j < width; ++j) {
            if (nfa[i][j].empty()) continue;
            temp = "";
            for (auto k: nfa[i][j]) {
                temp += to_string(k + 1);
                temp += ",";
            }
            item = QString::fromStdString(temp);
            ui->NFA_t->setItem(i, j + 2, new QTableWidgetItem(item));
        }
    }

    //DFA表内容进行设置
    int r = 0;
    map<DStat, vector<set<int>>> &dfa = reExp->dfa_exp.graph;
    ui->DFA_t->setItem(0, 0, new QTableWidgetItem("-"));
    for (auto &[k, v]: dfa) {
        if (k.end_flag) ui->DFA_t->setItem(r, 0, new QTableWidgetItem("+"));
        temp = "";
        temp += "{";
        for (auto &i: k.stats) {
            temp += to_string(i + 1);
            temp += ",";
        }
        temp += "}";
        item = QString::fromStdString(temp);
        ui->DFA_t->setItem(r, 1, new QTableWidgetItem(item));

        for (int i = 0; i < width - 1; ++i) {
            if (v[i].empty()) ui->DFA_t->setItem(r, i + 2, new QTableWidgetItem("{}"));
            else {
                temp = "{";
                for (auto &j: v[i]) {
                    temp += to_string(j + 1);
                    temp += ",";
                }
                temp += "}";
                item = QString::fromStdString(temp);
                ui->DFA_t->setItem(r, i + 2, new QTableWidgetItem(item));
            }
        }
        r++;
    }

    //最小DFA表内容显示
    r = 0;
    map<int, vector<int>> &min_map = reExp->min_exp->stat_map;

    ui->MINI_t->setItem(0, 0, new QTableWidgetItem("-"));
    for (auto &[k, v]: min_map) {
        if (len_mini == 1) ui->MINI_t->setItem(r, 0, new QTableWidgetItem("-+"));
        else if (r == len_mini - 1) ui->MINI_t->setItem(r, 0, new QTableWidgetItem("+"));

        item = QString::fromStdString(to_string(r + 1));
        ui->MINI_t->setItem(r, 1, new QTableWidgetItem(item));
        for (int i = 0; i < width - 1; ++i) {
            if (v[i] + 1 == 0) continue;
            temp = to_string(v[i] + 1);
            item = QString::fromStdString(temp);
            ui->MINI_t->setItem(r, i + 2, new QTableWidgetItem(item));
        }
        r++;
    }

    //生成代码
    item = QString::fromStdString(reExp->generate_Code());
    ui->Buid_code->setText(item);

//    reExp->shown(0);
}

void MainWindow::on_clean_t_clicked() {
    ui->NFA_t->clear();
    ui->NFA_t->setColumnCount(0);
    ui->NFA_t->setRowCount(0);

    ui->DFA_t->clear();
    ui->DFA_t->setColumnCount(0);
    ui->DFA_t->setRowCount(0);

    ui->MINI_t->clear();
    ui->MINI_t->setColumnCount(0);
    ui->MINI_t->setRowCount(0);

    ui->Buid_code->clear();
}

void MainWindow::on_actionopen_file_triggered() {
//    QFileDialog dialog(this);
//    dialog.setWindowTitle("选择.txt文件");
//    dialog.setFileMode(QFileDialog::ExistingFile);
//
//    // 设置筛选条件，只允许选择.cpp文件
//    dialog.setNameFilter("C++ Files (*.txt)");
//
//    if (dialog.exec()) {
//        QStringList selectedFiles = dialog.selectedFiles();
//        if (!selectedFiles.isEmpty()) {
//            QString filePath = selectedFiles.first();
//
//            // 检查所选文件的扩展名是否为.cpp
//            QFileInfo fileInfo(filePath);
//            if (fileInfo.suffix().toLower() == "txt") {
//                // 更新标签以显示选择的文件路径和名称
//                selectedFileLabel->setText("选择的文件：" + filePath);
//
//                // 此处你可以保存文件路径供后续使用
//                path = filePath.toStdString();
//            } else {
//                // 显示错误提示
//                QMessageBox::critical(this, "错误", "请选择一个.txt文件！");
//                exit(1);
//            }
//        }
//    }
}
