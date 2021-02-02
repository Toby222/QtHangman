#include <QApplication>

#include "hangman.h"

using std::string;

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(words);

    QApplication app(argc, argv);

#ifndef QT_DEBUG
    srand(time(0));
#else
    srand(4); // https://xkcd.com/221/ //
#endif

    HangMan window;
    window.show();

    return app.exec();
}
