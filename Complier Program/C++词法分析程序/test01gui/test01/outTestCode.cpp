#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <map>
#include <cctype>

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::ios;
using std::getline;

using std::string;
using std::vector;
using std::map;
using std::isalnum;
using std::isdigit;
using std::isalpha;

int transState(const string &str, bool &flag, int pos, vector<int> &arr, char tab[], size_t colLen) {
    for (int i = 0; i < colLen; ++i) {
        if (str[pos] == tab[i]) {
            flag = true;
            return arr[i];
        }
    }
    return 0;
}

vector<string> splitCode(string &line) {
    vector<string> words;

    int pos = 0;
    auto len = line.size();
    string tmp;
    while (pos < len) {
        while (line[pos] == '\n' || line[pos] == '\t' || line[pos] == ' ')
            pos++;
        if (pos == len)
            break;

        if (isalnum(line[pos]) || line[pos] == '_') {
            while (isalnum(line[pos]) || line[pos] == '_') {
                if (line[pos] == '\n' || line[pos] == '\t' || line[pos] == ' ')
                    break;
                tmp += line[pos++];
            }
        } else if (line[pos] != '(' && line[pos] != ')' &&
                   line[pos] != '[' && line[pos] != ']' &&
                   line[pos] != '{' && line[pos] != '}'
                ) {
            while (!isalnum(line[pos]) &&
                   line[pos] != '(' && line[pos] != ')' &&
                   line[pos] != '[' && line[pos] != ']' &&
                   line[pos] != '{' && line[pos] != '}'
                    ) {
                if (isalnum(line[pos]) || line[pos] == '_' || line[pos] == '\n' || line[pos] == '\t' ||
                    line[pos] == ' ')
                    break;
                tmp += line[pos++];
            }
        } else if (line[pos] == '(' || line[pos] == ')' ||
                   line[pos] == '[' || line[pos] == ']' ||
                   line[pos] == '{' || line[pos] == '}')
            tmp += line[pos++];
        else pos++;
        words.emplace_back(tmp);
        tmp = "";
    }
    words.pop_back();   //Pop the last one
    return words;
}

void transChar(string &word, map<char, char> &reC) {
    for (auto &c: word) {
        if (reC.find(c) != reC.end())
            c = reC[c];
    }
}

vector<string> getTny(string addr, map<char, char> &reC) {
    fstream op(addr, ios::in);
    string lines;

    string line;
    while (getline(op, line)) {
        lines += line;
        lines += "\n";
    }
    op.close();
    

    transChar(lines, reC);    return splitCode(lines);
}

void saveLex(string addr, string txt) {
    fstream op(addr, ios::out);
    op << txt;
    op.close();}

bool strMatch0(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'i','f'};

    map<int, vector<int>> arr = {{1,{3,0}},{2,{0,0}},{3,{0,2}}};
    int state = 1;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 2)
        return false;
    return true;
}

bool strMatch1(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'t','h','e','n'};

    map<int, vector<int>> arr = {{1,{0,0,0,0}},{2,{0,0,4,0}},{3,{5,0,0,0}},{4,{0,0,0,1}},{5,{0,2,0,0}}};
    int state = 3;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 4:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 5:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch2(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'e','l','s'};

    map<int, vector<int>> arr = {{1,{0,0,0}},{2,{0,0,4}},{3,{5,0,0}},{4,{1,0,0}},{5,{0,2,0}}};
    int state = 3;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 4:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 5:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch3(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'e','n','d'};

    map<int, vector<int>> arr = {{1,{0,0,0}},{2,{0,0,1}},{3,{4,0,0}},{4,{0,2,0}}};
    int state = 3;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 4:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch4(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'r','e','p','a','t'};

    map<int, vector<int>> arr = {{1,{0,0,0,0,0}},{2,{0,6,0,0,0}},{3,{0,7,0,0,0}},{4,{0,0,0,0,1}},{5,{2,0,0,0,0}},{6,{0,0,3,0,0}},{7,{0,0,0,4,0}}};
    int state = 5;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 4:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 5:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 6:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 7:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch5(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'u','n','t','i','l'};

    map<int, vector<int>> arr = {{1,{0,0,0,0,0}},{2,{0,0,0,0,1}},{3,{0,0,5,0,0}},{4,{6,0,0,0,0}},{5,{0,0,0,2,0}},{6,{0,3,0,0,0}}};
    int state = 4;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 4:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 5:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 6:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch6(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'r','e','a','d'};

    map<int, vector<int>> arr = {{1,{0,0,0,0}},{2,{0,0,4,0}},{3,{5,0,0,0}},{4,{0,0,0,1}},{5,{0,2,0,0}}};
    int state = 3;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 4:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 5:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch7(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'w','r','i','t','e'};

    map<int, vector<int>> arr = {{1,{0,0,0,0,0}},{2,{0,0,0,0,1}},{3,{0,0,5,0,0}},{4,{6,0,0,0,0}},{5,{0,0,0,2,0}},{6,{0,3,0,0,0}}};
    int state = 4;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 4:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 5:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 6:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch8(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'@'};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch9(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'-'};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch10(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'^'};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch11(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'/'};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch12(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'%'};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch13(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'<'};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch14(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'<','>'};

    map<int, vector<int>> arr = {{1,{3,0}},{2,{0,0}},{3,{0,2}}};
    int state = 1;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 2)
        return false;
    return true;
}

bool strMatch15(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'<','='};

    map<int, vector<int>> arr = {{1,{3,0}},{2,{0,0}},{3,{0,2}}};
    int state = 1;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 2)
        return false;
    return true;
}

bool strMatch16(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'>','='};

    map<int, vector<int>> arr = {{1,{3,0}},{2,{0,0}},{3,{0,2}}};
    int state = 1;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 2)
        return false;
    return true;
}

bool strMatch17(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'>'};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch18(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'='};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch19(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'{'};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch20(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'}'};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch21(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {';'};

    map<int, vector<int>> arr = {{1,{0}},{2,{1}}};
    int state = 2;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch22(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {':','='};

    map<int, vector<int>> arr = {{1,{3,0}},{2,{0,0}},{3,{0,2}}};
    int state = 1;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 2)
        return false;
    return true;
}

bool strMatch23(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'_','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9'};

    map<int, vector<int>> arr = {{1,{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},{37,{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0}}};
    int state = 37;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 4:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 5:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 6:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 7:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 8:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 9:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 10:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 11:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 12:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 13:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 14:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 15:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 16:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 17:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 18:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 19:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 20:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 21:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 22:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 23:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 24:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 25:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 26:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 27:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 28:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 29:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 30:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 31:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 32:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 33:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 34:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 35:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 36:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 37:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 38:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 39:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 40:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 41:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 42:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 43:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 44:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 45:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 46:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 47:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 48:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 49:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 50:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 51:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 52:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 53:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 54:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 55:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 56:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 57:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 58:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 59:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 60:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 61:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 62:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 63:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 64:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 65:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 66:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 67:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 68:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 69:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 70:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 71:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 72:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 73:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 74:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 75:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 76:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 77:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 78:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 79:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 80:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 81:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 82:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 83:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 84:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 85:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 86:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 87:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 88:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 89:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 90:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 91:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 92:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 93:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 94:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 95:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 96:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 97:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 98:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 99:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 100:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 101:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 102:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 103:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 104:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 105:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 106:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 107:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 108:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 109:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 110:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 111:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 112:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 113:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 114:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 115:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 116:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 117:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

bool strMatch24(const string &str) {
    auto size = str.length();   //Len of a word
    char tab[] = {'0','1','2','3','4','5','6','7','8','9'};

    map<int, vector<int>> arr = {{1,{1,1,1,1,1,1,1,1,1,1}}};
    int state = 1;

    auto colLen = arr[state].size();

    int pos = 0;
    while (pos < size) {
        bool flag = false;
        switch (state) {
            case 1:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 2:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 3:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 4:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 5:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 6:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 7:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 8:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 9:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 10:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            case 11:
                state = transState(str, flag, pos, arr[state], tab, colLen);
                break;
            default:
                return false;
        }
        pos++;
    }

    if (state != 1)
        return false;
    return true;
}

string analyse(vector<string> &words, map<char, char> &backC) {
    vector<string> typeStr={"IF","THEN","ELSE","END","REPEAT","UNTIL","READ","WRITE","ADD","SUB","MUL","DIV","MOD","LES","NEQ","LEQ","GEQ","GRE","EQU","LET","RET","HAD","ASS","ID","NUM"};
    string res;
    for (auto &word: words) {
        if (strMatch0(word)) {
            res += word + " ";
            res += typeStr[0] + "\n";
        } else  if (strMatch1(word)) {
            res += word + " ";
            res += typeStr[1] + "\n";
        } else  if (strMatch2(word)) {
            res += word + " ";
            res += typeStr[2] + "\n";
        } else  if (strMatch3(word)) {
            res += word + " ";
            res += typeStr[3] + "\n";
        } else  if (strMatch4(word)) {
            res += word + " ";
            res += typeStr[4] + "\n";
        } else  if (strMatch5(word)) {
            res += word + " ";
            res += typeStr[5] + "\n";
        } else  if (strMatch6(word)) {
            res += word + " ";
            res += typeStr[6] + "\n";
        } else  if (strMatch7(word)) {
            res += word + " ";
            res += typeStr[7] + "\n";
        } else  if (strMatch8(word)) {
            res += word + " ";
            res += typeStr[8] + "\n";
        } else  if (strMatch9(word)) {
            res += word + " ";
            res += typeStr[9] + "\n";
        } else  if (strMatch10(word)) {
            res += word + " ";
            res += typeStr[10] + "\n";
        } else  if (strMatch11(word)) {
            res += word + " ";
            res += typeStr[11] + "\n";
        } else  if (strMatch12(word)) {
            res += word + " ";
            res += typeStr[12] + "\n";
        } else  if (strMatch13(word)) {
            res += word + " ";
            res += typeStr[13] + "\n";
        } else  if (strMatch14(word)) {
            res += word + " ";
            res += typeStr[14] + "\n";
        } else  if (strMatch15(word)) {
            res += word + " ";
            res += typeStr[15] + "\n";
        } else  if (strMatch16(word)) {
            res += word + " ";
            res += typeStr[16] + "\n";
        } else  if (strMatch17(word)) {
            res += word + " ";
            res += typeStr[17] + "\n";
        } else  if (strMatch18(word)) {
            res += word + " ";
            res += typeStr[18] + "\n";
        } else  if (strMatch19(word)) {
            res += word + " ";
            res += typeStr[19] + "\n";
        } else  if (strMatch20(word)) {
            res += word + " ";
            res += typeStr[20] + "\n";
        } else  if (strMatch21(word)) {
            res += word + " ";
            res += typeStr[21] + "\n";
        } else  if (strMatch22(word)) {
            res += word + " ";
            res += typeStr[22] + "\n";
        } else  if (strMatch23(word)) {
            res += word + " ";
            res += typeStr[23] + "\n";
        } else  if (strMatch24(word)) {
            res += word + " ";
            res += typeStr[24] + "\n";
        } else if (word == "(") {
            res += word + " ";
            res += "LEC\n";
        } else if (word == ")") {
            res += word + " ";
            res += "REC\n";
        } else {
            res += word + " ERR\n";
        }
    }
    transChar(res, backC);
    return res;
}

int main() {
    map<char, char> reC = {{'&','.'},{'|','~'},{'*','^'},{'?','$'},{'+','@'}};
    auto vec = getTny("example.tny", reC);

    map<char, char> backC = {{'.','&'},{'~','|'},{'^','*'},{'$','?'},{'@','+'}};
    saveLex("test.lex", analyse(vec, backC));
    return 0;
}
