#ifndef HANGMAN_H
#define HANGMAN_H
#include <QMainWindow>
#include <QPushButton>
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

public slots:
    void initGame();
    void endGame(bool won = false);

private:
    QVector<char> attemptedLetters = {};
    QString currentWord;
    GameState state;
    int score;

    //
    QGraphicsScene *scene;
    Gallows *gallows;

    QGraphicsTextItem *wordOutput;
    QGraphicsTextItem *guessedOutput;
    QGraphicsTextItem *endingOutput;
    QGraphicsTextItem *scoreOutput;

    QPushButton *resetButton;
    QPushButton *solveButton;
    //

    static QString generateRandomWord();

    QString uncensoredWord();
    QString censoredWord();
    QString guessedChars();

    void keyPressEvent(QKeyEvent *event) override;
signals:

};

#endif // HANGMAN_H
