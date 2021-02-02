#include <QGraphicsScene>
#include <QtWidgets>
#include <QHBoxLayout>

#include "hangman.h"
#include "gallows.h"
#include "wordlist.h"

QString HangMan::generateRandomWord()
{
    return QString::fromUtf8(wordList[QRandomGenerator::global()->bounded(0, sizeof(wordList)/sizeof(std::string))].c_str());
}

HangMan::HangMan(QWidget *parent)
    : QMainWindow(parent, {Qt::MSWindowsFixedSizeDialogHint}),
      state(GameState::Ended),
      scene(new QGraphicsScene(this)),
      gallows(new Gallows),
      resetButton(new QPushButton("New word")),
      solveButton(new QPushButton("Reveal"))
{
    setWindowTitle("Hangman");

    // Get system-default monospace font
    auto monoFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    auto monoFontBold = QFont(monoFont);
    monoFontBold.setBold(true);

    scene->setSceneRect(0, 0, 408, 190);

    scene->addItem(gallows);

    wordOutput = scene->addText("", monoFont);
    wordOutput->setPos(200, 45);

    guessedOutput = scene->addText("", monoFont);
    guessedOutput->setPos(200, 150);

    scoreOutput = scene->addText("Score: 0", monoFont);
    scoreOutput->setPos(200, 10);

    endingOutput = scene->addText("", monoFontBold);
    endingOutput->setPos(gallows->x()+23, gallows->y()+50);

    scene->addWidget(resetButton);
    resetButton->setFont(monoFont);
    resetButton->move(200, 100);
    connect(resetButton, &QPushButton::clicked, this, &HangMan::initGame);

    scene->addWidget(solveButton);
    solveButton->setFont(monoFont);
    solveButton->move(300, 100);
    connect(solveButton, &QPushButton::clicked, this, &HangMan::endGame);

    endingOutput->setDefaultTextColor(Qt::red);

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

    if(won) scoreOutput->setPlainText(QString("Score: %1").arg(score += currentWord.length()));

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
        score--;
        scoreOutput->setPlainText(QString("Score: %1").arg(score));
        if(gallows->increasePartCount() >= Gallows::PART_COUNT) {
            endGame();
        }
    } else {
        wordOutput->setPlainText(censoredWord());
    }
    scene->update();
}
