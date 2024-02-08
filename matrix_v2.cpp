#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <vector>
#include <unistd.h>

using namespace std;

const int WORD_LENGTH = 20;
const int WORD_SPEED = 100;
const int COLOR_CHANGE_INTERVAL = 1000;
const int MATRIX_COLS = 80;
const int MATRIX_ROWS = 45;

class MatrixWord {
private:
    int x;
    int y;
    char word[WORD_LENGTH + 1];
    int colorPair;
    int lastColorChangeTime;

public:
    MatrixWord(int x, int y, const char* word, int colorPair) : x(x), y(y), colorPair(colorPair), lastColorChangeTime(0) {
        strncpy(this->word, word, WORD_LENGTH);
        this->word[WORD_LENGTH] = '\0';
    }

    void move() {
        y++;
        if (y >= LINES) {
            y = 0;
            x = rand() % COLS;
        }
    }

    void draw() {
        attron(COLOR_PAIR(colorPair));
        for (size_t i = 0; i < strlen(word); i++) {
            mvprintw(y + i, x, "%c", word[i]);
        }
        attroff(COLOR_PAIR(colorPair));
    }

    void setColor(int newColor) {
        colorPair = newColor;
    }

    int getLastColorChangeTime() const {
        return lastColorChangeTime;
    }

    void updateLastColorChangeTime(int currentTime) {
        lastColorChangeTime = currentTime;
    }
};

class MatrixAnimation {
private:
    int numWords;
    vector<MatrixWord*> words;
    int currentColor;

public:
    MatrixAnimation(int numWords, const char* wordChars) : numWords(numWords), currentColor(1) {
        for (int i = 0; i < numWords; i++) {
            int x = rand() % COLS;
            int y = rand() % LINES;

            char randomWord[WORD_LENGTH + 1];
            for (int j = 0; j < WORD_LENGTH; j++) {
                randomWord[j] = wordChars[rand() % strlen(wordChars)];
            }
            randomWord[WORD_LENGTH] = '\0';

            words.push_back(new MatrixWord(x, y, randomWord, currentColor));
        }
    }

    ~MatrixAnimation() {
        for (auto word : words) {
            delete word;
        }
    }

    void update() {
        clear();
        int currentTime = static_cast<int>(time(nullptr) * 1000);

        for (auto word : words) {
            word->move();
            word->draw();

            if (currentTime - word->getLastColorChangeTime() >= COLOR_CHANGE_INTERVAL * 1000) {
                word->setColor(currentColor);
                word->updateLastColorChangeTime(currentTime);
            }
        }

        refresh();
        napms(WORD_SPEED);
    }

    void changeColor() {
        currentColor = (currentColor % 4) + 1;
    }
};

void initColorPairs() {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
}

int main() {
    srand(time(nullptr));

    initscr();
    curs_set(0);
    timeout(0);
    keypad(stdscr, true);

    const char* wordChars = "ABCDEFGHIJKMLN123456";

    int numWords = 20;

    initColorPairs();

    MatrixAnimation matrixAnimation(numWords, wordChars);

    while (true) {
        matrixAnimation.update();
        usleep(10000);
        matrixAnimation.changeColor();
    }

    endwin();

    return 0;
}
