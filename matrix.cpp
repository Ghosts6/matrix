#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

const int WORD_LENGTH = 5;
const int WORD_SPEED = 100;
const int MATRIX_COLS = 80;
const int MATRIX_ROWS = 24;

class MatrixWord {
private:
    int x;
    int y;
    char word[WORD_LENGTH + 1];

public:
    MatrixWord(int x, int y, const char* word) : x(x), y(y) {
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
        attron(COLOR_PAIR(1));
        for (size_t i = 0; i < strlen(word); i++) {
            mvprintw(y + i, x, "%c", word[i]);
        }
        attroff(COLOR_PAIR(1));
    }
};

class MatrixAnimation {
private:
    int numWords;
    MatrixWord** words;

public:
    MatrixAnimation(int numWords, const char* word) : numWords(numWords) {
        words = new MatrixWord*[numWords];
        for (int i = 0; i < numWords; i++) {
            int x = rand() % COLS;
            int y = rand() % LINES;
            words[i] = new MatrixWord(x, y, word);
        }
    }

    ~MatrixAnimation() {
        for (int i = 0; i < numWords; i++) {
            delete words[i];
        }
        delete[] words;
    }

    void update() {
        clear();
        for (int i = 0; i < numWords; i++) {
            words[i]->move();
            words[i]->draw();
        }
        refresh();
        napms(WORD_SPEED);
    }
};

int main() {
    srand(time(NULL));

    initscr();
    curs_set(0);
    start_color();
    timeout(0);
    keypad(stdscr, true);

    string color_choice;
    mvprintw(0, 0, "Welcome to our matrix project");
    mvprintw(1, 0, "Please select color (green, blue, red, white): ");
    refresh();
    cin >> color_choice;

    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    if (color_choice == "green") {
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
    } else if (color_choice == "blue") {
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
    } else if (color_choice == "red") {
        init_pair(1, COLOR_RED, COLOR_BLACK);
    } else if (color_choice == "white") {
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
    } else {
        mvprintw(2, 0, "Invalid color choice. Defaulting to green.");
    }

    char word[WORD_LENGTH + 1];
    string choice;
    mvprintw(3, 0, "Do you want to use a custom word? (yes/no): ");
    refresh();
    cin >> choice;

    if (choice == "Yes" || choice == "yes") {
        mvprintw(4, 0, "Enter the word you want to use: ");
        refresh();
        cin >> word;
    } else {
        strncpy(word, "MATRIX", WORD_LENGTH);
        word[WORD_LENGTH] = '\0';
    }

    MatrixAnimation matrixAnimation(10, word);

    int ch;
    while ((ch = getch()) != 'q') {
        matrixAnimation.update();
        if ((ch = getch()) != ERR) {
            ungetch(ch);
        }
    }
    endwin();

    return 0;
}
