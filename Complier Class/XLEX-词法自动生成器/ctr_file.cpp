#include "ctr_file.h"

void edit_txt(string &s, const string &path) {
    ofstream outfile;
    outfile.open(path, ios::out);
    if (!outfile) {
        cerr << "The file open error!" << endl;
        exit(403);
    }
    outfile << s << endl;
    outfile.close();
}

vector<string> read_txt(const string &path) {
    ifstream infile;
    vector<string> vec;
    string tmp;

    infile.open(path, ios::in);
    while (getline(infile, tmp)) {
        vec.emplace_back(tmp);
    }
    infile.close();
    return vec;
}

// 功能函数
vector<string> subStrToVec(string &str, char sep) {
    vector<string> vecArr;
    int flagSub = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == sep) {
            string temp = str.substr(flagSub, i - flagSub);
            vecArr.push_back(temp);
            flagSub = i + 1;
        }
    }
    vecArr.push_back(str.substr(flagSub, str.length() - flagSub));
    return vecArr;
}

string merge_str(vector<string> &vec) {
    if (vec.empty()) {
        cerr << "Error, input is empty!" << endl;
        exit(4);
    }

    int len = vec.size();
    string tmp = vec[0];
    for (int i = 1; i < len; ++i) {
        tmp += "|(";
        tmp += vec[i];
        tmp += ")";
    }
    return tmp;
}