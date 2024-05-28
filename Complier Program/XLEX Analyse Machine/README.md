#### 运行环境
1. qmake编译环境：Qt_5_14_2_MinGW_64_bit-Debug
2. 最好不要包含中文

#### 一些需要注意的问题
1. 作者在制作qt时不包含中文解释，因此避免中文路径问题
2. 作者由于太懒了，.cpp更换目录暂时没做
3. QTableView没有找到方便的删除方式，因此执行clear按钮槽函数之后仍然不能完全删除表格数据
4. 之后这些问题会慢慢更新解决

#### 本次更新内容
1. 针对mainwindow.cpp中的固定路径进行了改变，使其在生成代码时可以选择路径
2. 生成代码部分做如下处理：
+ 分词函数：
```c++
// global.h - line 292
                   "        if (isalnum(line[pos]) || line[pos] == '_') {\n"
                   "            while (isalnum(line[pos]) || line[pos] == '_' || line[pos] == '.') {\n"
                   "                if (line[pos] == '\\n' || line[pos] == '\\t' || line[pos] == ' ')\n"
                   "                    break;\n"
                   "                tmp += line[pos++];\n"
                   "            }\n"
//line 320
                   "    //words.pop_back();   //Pop the last one\n"
```

+ 转义部分：
```c++
// global.h - line 381
        char changeC[] = {'`', '~', '^', '$', '@'};
```

+ 连接符添加部分：
```c++
// global.cpp - line 42
    for (int i = 1; i < s.length(); ++i) {
        if ((precedence(s[i - 1]) == 3 && opMap.find(s[i]) == opMap.end() && s[i] != ')') ||
                (s[i - 1] == ')' && s[i] == '(') ||
                (s[i - 1] == ')' && opMap.find(s[i]) == opMap.end()) ||
                ((opMap.find(s[i - 1]) == opMap.end() && s[i - 1] != '(') &&
                 (opMap.find(s[i]) == opMap.end() && s[i] != ')')) ||
                ((opMap.find(s[i - 1]) == opMap.end() && s[i - 1] != ')') && s[i] == '(')
           ) {
            new_s += '&';
            new_s += s[i];
        } else new_s += s[i];
    }
```

+ 子代码生成部分：
```c++
// global.cpp - line 771
    //设置终态结束部分
    str += "\n"
           "    if (state != ";
    int times = 0;
    for (auto &id : dfa->finTab) {
        if (times > 0) {
            str += " && state != ";
        }
        str += to_string(id);
        times++;
    }
    str += ")\n"
           "        return false;\n"
           "    return true;\n"
           "}\n\n";
//    cout << str << endl;
    return str;
```

+ mainwindow.cpp部分若干：主要是`on_produceCPP_clicked()`和`on_saveLex_clicked()`函数
