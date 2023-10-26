#ifndef EXPERIMENT01_CTR_FILE_H
#define EXPERIMENT01_CTR_FILE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void edit_txt(string &s, const string &path);

vector<string> read_txt(const string &path);

vector<string> subStrToVec(string &str,char sep);

string merge_str(vector<string> &vec);

#endif //EXPERIMENT01_CTR_FILE_H
