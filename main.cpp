#include <QCoreApplication>

#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include "wordlist.h"

using std::string;

const int PART_COUNT = 10;
const int GAME_WIDTH = 35;

void print(string s) {
    std::cout << s.c_str();
}
void println(string s = "") {
    std::cout << s.c_str();
    std::cout << std::endl;
}


void printMsg(string msg = "", bool top = true, bool bottom = true) {
    if(top) {
        println('*' + string(GAME_WIDTH-2, '-') + '*');
    }
    auto edge_width = (GAME_WIDTH - 2 - msg.length()) / 2.0f;

    auto left  = string(std::floor(edge_width), ' ');
    auto right = string(std::ceil (edge_width), ' ');

    println("|" + left + msg + right + "|");

    if(bottom) {
        println('*' + string(GAME_WIDTH-2, '-') + '*');
    }
}

void printSubMsg(string msg = "") {
    printMsg(msg, false, false);
}

void drawState(int guesses = 0) {
    /*-------------------------------*
     |               |               |
     |               |               |
     |               O               |
     |              /|\              |
     |               |               |
     |              / \              |
     *-------------------------------*/

    printSubMsg(guesses > 0 ? "|" : "");
    printSubMsg(guesses > 1 ? "|" : "");
    printSubMsg(guesses > 2 ? "O" : "");

    if(guesses == 4) {
        printSubMsg("/ ");
    } else if(guesses == 5) {
        printSubMsg("/| ");
    } else if(guesses > 5) {
        printSubMsg("/|\\");
    } else {
        printSubMsg();
    }

    printSubMsg(guesses > 6 ? "|" : "");

    if(guesses == 8) {
        printSubMsg("/ ");
    } else if(guesses > 8) {
        printSubMsg("/ \\");
    } else {
        printSubMsg();
    }
}

int triesUsed(string solution, string guessed) {
    int error = 0;
    for(uint i = 0; i < guessed.length(); i++) {
        if(solution.find(guessed[i]) == string::npos) {
            error++;
        }
    }
    return error;
}

void printAvailableLetters(string taken) {
    printMsg("Available Letters");

    string letters;
    for(char c = 'a'; c <= 'z'; c++) {
        if(taken.find(c) == string::npos) {
            letters += c;
        } else {
            letters += ' ';
        }
        letters += ' ';

        if(c == 'm') {
            printSubMsg(letters);
            letters.clear();
        }
    }
    printSubMsg(letters);
}

bool printWordCheckWin(string word, string guessed) {
    bool won = true;
    string s;

    for(uint i = 0; i < word.length(); i++) {
        if(guessed.find(word[i]) == string::npos) {
            won = false;
            s += '_';
        } else {
            s += word[i];
        }
        s += ' ';
    }

    printMsg(s, false);
    return won;
}

const string randomWord() {
    return wordList[rand()%(sizeof(wordList)/sizeof(string))];
}

void clearScreen() {
    for(int i = 0; i < 30; i ++) {
        println();
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    srand(time(0)); // srand(4); // https://xkcd.com/221/ //
    while(true) {

        int guessesTaken = 0;
        bool won = false;
        string guessed;
        string currentWord = randomWord();

        do {
            clearScreen();

            printMsg("HANGMAN");
            drawState(guessesTaken);

            printAvailableLetters(guessed);
            printMsg("Guess the word");

            if((won = printWordCheckWin(currentWord, guessed)))
                break;

            print(">");
            char x = std::cin.get();

            // basically "toUpper()"
            if(x >= 'A' && x <= 'Z')
                x += ' ';

            if(x < 'a' || x > 'z') continue;

            if(guessed.find(x) == string::npos) guessed += x;

            guessesTaken = triesUsed(currentWord, guessed);
        } while(guessesTaken < PART_COUNT);

        if(won) {
            printMsg("You won!");
        } else {
            clearScreen();
            printMsg("You lost", true, false);

            printSubMsg("You guessed:");
            printSubMsg(guessed);

            printSubMsg();

            printSubMsg("The word was:");
            printMsg(currentWord, false, true);
        }
        printMsg("Enter . to end the game", false);
        std::cin.get();
        char c = std::cin.get();
        if(c == '.') {
            exit(0);
        }
    }
    return a.exec();
}
