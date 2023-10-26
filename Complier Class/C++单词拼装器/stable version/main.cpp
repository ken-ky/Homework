#include "ui_frame.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget parent;
    WordsAssembler wordsAss(&parent);
    wordsAss.show();
    return app.exec();
}