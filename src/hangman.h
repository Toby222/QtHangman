#ifndef HANGMAN_H
#define HANGMAN_H
#include <QMainWindow>
#include "gallows.h"

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE


enum GameState {
    Playing,
    Ended
};

class HangMan : public QMainWindow
{
    Q_OBJECT
public:
    HangMan(QWidget *parent = nullptr);

    void initGame();
    void endGame(bool won = false);

private:
    QGraphicsScene *scene;
    QVector<char> attemptedLetters = {};
    QString currentWord;
    GameState state;

    Gallows *gallows;
    QGraphicsTextItem *wordOutput;
    QGraphicsTextItem *guessedOutput;
    QGraphicsTextItem *endingOutput;

    static QString generateRandomWord();

    QString uncensoredWord();
    QString censoredWord();
    QString guessedChars();

    void keyPressEvent(QKeyEvent *event) override;
signals:

};

#endif // HANGMAN_H
