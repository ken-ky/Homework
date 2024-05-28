#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->reRulePath = "reRule.txt";
    this->cppPath = "outTestCode.cpp";
    this->tnyPath = "example.tny";
    this->lexPath = "test.lex";

    nfaFlag = false;
    dfaFlag = false;
    minFlag = false;

    ui->clearText->setEnabled(false);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_transNFA_clicked() {
    code = new codePro(reRulePath.toStdString());
    if(code->REs.empty()) {
        ui->terLines->append(QString::fromStdString("Producing NFA failed!\n"));
        return;
    }

    auto &reStr = code->reStr;
    for (int num = 0; num < reStr.size(); num++) {
        selection[QString::fromStdString(reStr[num])] = num;
    }
    ui->nfaSel->clear();

    foreach(QString str, selection.keys()) {
//        cout<<selection[str]<<endl;
        ui->nfaSel->addItem(str, selection[str]);
    }

    ui->terLines->append("NFA complete!\n");
    nfaFlag = true;
}

void MainWindow::on_transDFA_clicked() {
    if(!nfaFlag) {
        ui->terLines->append(QString::fromStdString("NFA hasn\'t built!\n"));
        return;
    }
    ui->dfaSel->clear();

    foreach(QString str, selection.keys()) {
        ui->dfaSel->addItem(str, selection[str]);
    }

    ui->terLines->append("DFA complete\n");
    dfaFlag = true;
}

void MainWindow::on_minimizeDFA_clicked() {
    if(!dfaFlag) {
        ui->terLines->append(QString::fromStdString("DFA hasn\'t built!\n"));
        return;
    }

    ui->minDfaSel->clear();

    foreach(QString str, selection.keys()) {
        ui->minDfaSel->addItem(str, selection[str]);
    }

    ui->terLines->append("minDFA complete!\n");
    minFlag = true;
}

void MainWindow::on_produceCPP_clicked() {
    if(!minFlag) {
        ui->terLines->append(QString::fromStdString("minDFA hasn\'t built!\n"));
        return;
    }

    tnyPath = QFileDialog::getOpenFileName(nullptr, "Open File", QDir::currentPath(), "All Files(*.*)");
    if(tnyPath.isEmpty()) {
        qDebug() << "No file selected!";
        return;
    }

    //生成结果并选择保存路径
    lexPath = QFileDialog::getSaveFileName(nullptr, "Save File", QDir::currentPath(), ".lex Files(*.lex)");
    if(lexPath.isEmpty()) {
        qDebug() << "No file selected!";
        return;
    }

    //生成代码
    code->initCode(tnyPath.toStdString(), lexPath.toStdString());  //需要改

    //选择路径
    cppPath = QFileDialog::getSaveFileName(nullptr, "Save File", QDir::currentPath(), ".cpp Files(*.cpp)");
    if(cppPath.isEmpty()) {
        qDebug() << "No file selected!";
        return;
    }

    code->outCPP(cppPath.toStdString());    //保存cpp文件
    vector<string> lines = getTxt(cppPath.toStdString());
    QString tmp;
    for(auto &line : lines) {
        tmp += QString::fromStdString(line);
        tmp += QString::fromStdString("\n");
    }
    ui->cppText->setText(tmp);

    code->outCPP(cppPath.toStdString());
    ui->terLines->append("Write " + cppPath + " successful!\n");
    cppFlag = true;
}

void MainWindow::on_saveLex_clicked() {
    if(!cppFlag) {
        ui->terLines->append(QString::fromStdString("cppFile hasn\'t built!\n"));
        return;
    }

    //tny代码显示
    vector<string> lines = getTxt(tnyPath.toStdString());
    QString tmp;
    for(auto &line : lines) {
        tmp += QString::fromStdString(line);
        tmp += QString::fromStdString("\n");
    }
    ui->tnyCode->setText(tmp);

    //截取相对路径
    string incpp = cppPath.toStdString();
    int pos = getLastChar(incpp, '/');
    incpp = incpp.substr(pos + 1, incpp.length() - 1 - pos);

    string exePath = incpp.substr(0, incpp.length() - 4) + ".exe";
    runCode(incpp, exePath, tnyPath.toStdString(), lexPath.toStdString());

    ui->terLines->append(QString::fromStdString("Write ") + lexPath + QString::fromStdString(" complete!\n")); //终端显示

    //显示lex文件
    lines = getTxt(lexPath.toStdString());
    tmp = "";
    for(auto &line : lines) {
        tmp += QString::fromStdString(line);
        tmp += QString::fromStdString("\n");
    }
    ui->lexRes->setText(tmp);
}

void MainWindow::on_clearText_clicked() {
    ui->lexRes->setText("");
    ui->tnyCode->setText("");
    ui->cppText->setText("");
    cppFlag = false;

    auto *ptr = ui->minDfaTab->model();
    ptr->removeRows(0, ptr->rowCount());
    ui->minDfaTab->clearSpans();
    ui->minDfaSel->clear();
    minFlag = false;

    ptr = ui->dfaTab->model();
    ptr->removeRows(0, ptr->rowCount());
    ui->dfaTab->clearSpans();
    ui->dfaSel->clear();
    dfaFlag = false;

    ptr = ui->nfaTab->model();
    ptr->removeRows(0, ptr->rowCount());
    ui->nfaTab->clearSpans();
    ui->nfaSel->clear();
    nfaFlag = false;

    ui->reRules->setText("");
    ui->terLines->setText("");

    this->reRulePath = "reRule.txt";
    this->cppPath = "outTestCode.cpp";
    this->tnyPath = "example.tny";
    this->lexPath = "test.lex";
}

void MainWindow::on_openFile_triggered() {
    reRulePath = QFileDialog::getOpenFileName(nullptr, "Open File", QDir::currentPath(), ".txt Files(*.txt)");
    if(reRulePath.isEmpty()) {
        qDebug() << "No file selected!";
        return;
    }

    //显示正则表达式内容
    vector<string> lines = getTxt(reRulePath.toStdString());
    QString tmp;
    for(auto &line : lines) {
        tmp += QString::fromStdString(line);
        tmp += QString::fromStdString("\n");
    }
    ui->reRules->setText(tmp);

    ui->terLines->append(QString::fromStdString("Reading ") + reRulePath + QString::fromStdString(" complete!\n")); //终端显示
}

void MainWindow::on_nfaSel_currentTextChanged(const QString &arg1) {
    //清空NFA表
    auto *tmp = ui->nfaTab->model();
    if(tmp != nullptr)
        tmp->removeRows(0, tmp->rowCount());

    //显示NFA图
    int row = selection[arg1];
//    cout<<row<<endl;

    auto &title = code->REs[row]->title;
    auto &nfaTab = code->REs[row]->nfaTable;
    auto &width = code->REs[row]->width;
    auto &nfa = code->REs[row]->nfa;

    vector<string> vec(width + 2, "");
    vec[0] = "Fin";
    vec[1] = "Ed";
    for (auto &[k, v] : title) {
        vec[v + 2] += k;
    }

    auto &tab = ui->nfaTab;
    auto size = nfaTab.size();
    QStandardItemModel *model = new QStandardItemModel(size, width + 2);


    //设置各列数据
    QStringList headers;
    for (int i = 0; i < width + 2; i++) {
        model->setHeaderData(i, Qt::Horizontal, QVariant(QString::fromStdString(vec[i])), Qt::DisplayRole);
    }

    //        code->REs[i]->nfaTableDis();
    //        cout<<endl;

    //设置各行数据
    for (int i = 0; i < size; i++) {
        if(nfa[i].isEnd)
            model->setItem(i, 0, new QStandardItem(QString::fromStdString("+")));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString(to_string(i))));

        int j = 2;
        for(auto &v : nfaTab[i]) {
            if(!v.empty()) {
                QString str = "";
                for(auto &k : v) {
                    str += QString::fromStdString(to_string(k));
                    str += ",";
                }
                model->setItem(i, j, new QStandardItem(str));
            }
            j++;
        }
    }
    tab->setModel(model);
}

void MainWindow::on_dfaSel_currentIndexChanged(const QString &arg1) {
    //清空DFA表
    auto *tmp = ui->dfaTab->model();
    if(tmp != nullptr)
        tmp->removeRows(0, tmp->rowCount());

    //显示DFA图
    int row = selection[arg1];
    auto &dfa = code->REs[row]->dfa;
//    code->REs[row]->dfaTableDis();

    auto size = dfa->transStates.size();
    auto width = dfa->transStates.begin()->second.size() + 2; //终态符-始键-值[...]

    auto &tab = ui->dfaTab;
    QStandardItemModel *model = new QStandardItemModel(size, width);

    //设置各行数据
    int i = 0;
    vector<QString> vec(width + 1, "");
    vec[0] = "Fin";
    vec[1] = "Id";
    vec[2] = "St";

    for(auto &[StStates, vStates] : dfa->transStates) {
        if(StStates.find(dfa->finState) != StStates.end())
            model->setItem(i, 0, new QStandardItem(QString::fromStdString("+")));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString(to_string(i))));

        QString str = "{";
        for(int state : StStates) {
            str += QString::fromStdString(to_string(state));
            str += ",";
        }
        str += "}";
        model->setItem(i, 2, new QStandardItem(str));

        int j = 3;
        for(auto &[k, EdStates] : vStates) {
            if(i == 0) {
//                cout<<k<<endl;
                vec[j] += k;
            }
            str = "";
            str += "{";
            for(int state : EdStates) {
                str += QString::fromStdString(to_string(state));
                str += ",";
            }
            str += "}";
            model->setItem(i, j, new QStandardItem(str));
            j++;
        }
        i++;
    }

    //设置表头
    QStringList headers;
    for (int i = 0; i < width + 1; i++) {
        model->setHeaderData(i, Qt::Horizontal, QVariant(vec[i]), Qt::DisplayRole);
    }

    tab->setModel(model);
}

void MainWindow::on_minDfaSel_currentIndexChanged(const QString &arg1) {
    //清空DFA表
    auto *tmp = ui->minDfaTab->model();
    if(tmp != nullptr)
        tmp->removeRows(0, tmp->rowCount());

    //显示DFA图
    int row = selection[arg1];
    auto &minDfa = code->REs[row]->minDfa;
    auto width = code->REs[row]->width;
//    code->REs[row]->minDfaTableDis();

    vector<QString> vec(width + 2, "");
    vec[0] = "Fin";
    vec[1] = "St";
    for(auto &[k, v] : minDfa->charSet) {
        if(v != '#')
            vec[v + 2] = k;
    }

    auto size = minDfa->minTransTab.size();
    auto &tab = ui->minDfaTab;
    QStandardItemModel *model = new QStandardItemModel(size, width + 1);

    //设置表头
    QStringList headers;
    for (int i = 0; i < width + 1; i++) {
        model->setHeaderData(i, Qt::Horizontal, QVariant(vec[i]), Qt::DisplayRole);
    }

    //设置表中内容
    int i = 0;
    for(auto &line : minDfa->minTransTab) {
        if(minDfa->finTab.find(line[0]) != minDfa->finTab.end()) {
            model->setItem(i, 0, new QStandardItem("+"));
        }

        int j = 1;
        for(auto state : line) {
            model->setItem(i, j, new QStandardItem(QString::fromStdString(to_string(state))));
            j++;
        }
        i++;
    }
    tab->setModel(model);
}
