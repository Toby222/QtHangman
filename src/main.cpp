#include <QApplication>

#include "hangman.h"

using std::string;

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(words);

    QApplication app(argc, argv);

    HangMan window;
    window.show();

    return app.exec();
}
