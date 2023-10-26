#include "DFAmodel.h"

void DStat::judge_end(int n) {
    for (int i: stats) {
        if (i == n) {
            end_flag = true;
//            cout<<"1"<<endl;
            return;
        }
    }
}

Min_DFA::Min_DFA(DFA &b) {
    int index = 0;
    for (auto &[d, _]: b.graph) {
        if (!stat.count(d.stats)) {
            stat[d.stats] = index;
            if(d.end_flag){
                final_stat.insert(index);
                cout<<2<<endl;
            }
            index++;
        }
    }
}