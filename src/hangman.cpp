#include <QGraphicsScene>
#include <QtWidgets>
#include <QHBoxLayout>

#include "hangman.h"
#include "gallows.h"
#include "wordlist.h"

QString HangMan::generateRandomWord()
{
    return QString::fromUtf8(wordList[rand() % sizeof(wordList)/sizeof(std::string)].c_str());
}

HangMan::HangMan(QWidget *parent)
    : QMainWindow(parent, {Qt::MSWindowsFixedSizeDialogHint}), scene(new QGraphicsScene(this)), state(GameState::Ended), gallows(new Gallows)
{
    setWindowTitle("Hangman");

    // Get system-default monospace font
    auto monoFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    auto monoFontBold = QFont(monoFont);
    monoFontBold.setBold(true);

    scene->addItem(gallows);
    wordOutput = scene->addText("", monoFont);
    wordOutput->setPos(200, 15);

    guessedOutput = scene->addText("", monoFont);
    guessedOutput->setPos(200, 150);

    endingOutput = scene->addText("", monoFontBold);
    endingOutput->setPos(gallows->x()+23, gallows->y()+50);

    endingOutput->setDefaultTextColor(Qt::red);

    scene->setSceneRect(0, 0, 408, 190);

    QGraphicsView *view = new QGraphicsView;
    view->setScene(scene);

    setCentralWidget(view);

    initGame();
}

void HangMan::initGame()
{
    currentWord = generateRandomWord();
    attemptedLetters.clear();

    wordOutput->setPlainText(censoredWord());
    guessedOutput->setPlainText(guessedChars());
    endingOutput->setPlainText("");

    gallows->reset();

    state = GameState::Playing;
    scene->update();
}

void HangMan::endGame(bool won)
{
    state = GameState::Ended;

    wordOutput->setPlainText(uncensoredWord());

    /*
     *      You won!
     * Press F5 to restart
     */

    if(won) endingOutput->setPlainText("     You won!\nPress F5 to restart");
    else endingOutput->setPlainText("     You lost\nPress F5 to restart");
    scene->update();
}

QString HangMan::uncensoredWord()
{
    // replace method is in-place so make a copy
    auto uncensored = QString(currentWord);
    uncensored.replace(QRegularExpression("(.)(?!$)"), "\\1 ");
    return uncensored;
}

QString HangMan::censoredWord()
{
    auto censored = QString(currentWord);
    auto won = true;
    for (auto c : currentWord) {
        if(attemptedLetters.contains(c.toLatin1())) continue;
        censored.replace(c, '_');
        won = false;
    }
    censored.replace(QRegularExpression("(.)(?!$)"), "\\1 ");

    if(won) endGame(won);

    return censored;
}

QString HangMan::guessedChars()
{
    auto formatted = QString();
    for(auto c = 'a'; c < 'z'; c++) {
        formatted.append(attemptedLetters.contains(c) ? '_' : c);
        if(c == 'm') formatted.append('\n');
        else formatted.append(' ');
    }
    formatted.append(attemptedLetters.contains('z') ? '_' : 'z');
    return formatted;
}

void HangMan::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key::Key_Escape) window()->close();
    if(event->key() == Qt::Key::Key_F5) initGame();
    if(state==GameState::Ended) return;

    auto key = event->text();
    if(key.length() == 0) return;

    auto pressed = key.at(0).toLower().toLatin1();
    if(pressed < 'a' || pressed > 'z') return;
    if(attemptedLetters.contains(pressed)) return;

    attemptedLetters.append(pressed);
    guessedOutput->setPlainText(guessedChars());

    if(!currentWord.contains(pressed)) {
        if(gallows->increasePartCount() >= Gallows::PART_COUNT) {
            endGame();
        }
    } else {
        wordOutput->setPlainText(censoredWord());
    }
    scene->update();
}
