![baner](https://github.com/Ghosts6/Local-website/blob/main/img/Baner.png)

# 💻matrix:

in this repo i create matrix animation with language like bash ,bat script,c++ and html/js

<img width="1470" alt="Screenshot 2024-02-08 at 8 58 33 PM" src="https://github.com/Ghosts6/matrix/assets/95994481/6c99fd39-4361-4460-8816-3f159eee78e2">

# Bash&bat:


in this section we have some simple code with bash and cmd  that create basic version of matrix animation

bash:
```bash
#!/bin/bash

clear
echo ""
echo -e "\e[32m"  

while true; do
    echo "$RANDOM $RANDOM $RANDOM $RANDOM $RANDOM $RANDOM $RANDOM $RANDOM $RANDOM $RANDOM $RANDOM $RANDOM"
done
```
cmd:
```bat
echo.
color 0a
title matrix
:(variable)
echo %random% %random% %random% %random% %random% %random% %random% %random% %random% %random% %random% %random% 
goto(variable)
```

# C++:

for c++ i create two version of matrix in first version user have some accessablity like choosing color and word for matrix animation but on second version instead of getting input from user i work on code and some other feature like changeing color , random words and etc 

matrix.cpp
```cpp
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
```
matrix_v2.cpp
```cpp
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
```


# html/js/css:

i also created matrix animation with help of frontend tools(html/css/js) and here you can take closer look to source code of it

html:
```html
<!DOCTYPE html>
<html len="en">
<head>
    <link rel="stylesheet" href="matrix.css">
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">   
    <title>Matrix</title>
</head>
<body>
    <canvas>        
    </canvas>
    <script src="matrix.js"></script>
</body>
</html>
```
js:
```js
var canvas = document.querySelector('canvas'),
    ctx = canvas.getContext('2d');

canvas.width = window.innerWidth;
canvas.height = window.innerHeight;

var template = '"ABCDEFGHIJKLMNOPQRSTUVXYZ123456789ABCDEFGHIJKLMNOPQRSTUVXYZ123456789\
ABCDEFGHIJKLMNOPQRSTUVXYZ123456789ABCDEFGHIJKLMNOPQRSTUVXYZ123456789ABCDEFGHIJKLMNOP\
QRSTUVXYZ123456789ABCDEFGHIJKLMNOPQRSTUVXYZ123456789";';
template = template.split('');

var fontSize = 14,
    columns = canvas.width / fontSize;

var drops = [];
for (var i = 0; i < columns; i++) {
    drops[i] = 1;
}

var colors = ['#00f', '#0f0', '#f00', '#800080'];
var currentColor = 0;

function draw() {
    if (Math.floor(Date.now() / 10000) % 4 === 0) {
        currentColor = 0;
    } else if (Math.floor(Date.now() / 10000) % 4 === 1) {
        currentColor = 1;
    } else if (Math.floor(Date.now() / 10000) % 4 === 2) {
        currentColor = 2;
    } else {
        currentColor = 3;
    }

    ctx.fillStyle = 'rgba(0, 0, 0, .1)';
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    for (var i = 0; i < drops.length; i++) {
        var text = template[Math.floor(Math.random() * template.length)];
        ctx.fillStyle = colors[currentColor];
        ctx.fillText(text, i * fontSize, drops[i] * fontSize);
        drops[i]++;

        if (drops[i] * fontSize > canvas.height && Math.random() > .95) {
            drops[i] = 0;
        }
    }
}

setInterval(draw, 33);
```
css:
```css
body{
    background-color: black ;
    background-size: 100% 100%;
    background-attachment: fixed;
}
```


# ▶video:

[Screen Recording 2024-02-10 at 3.58.53 AM.webm](https://github.com/Ghosts6/matrix/assets/95994481/41938aa6-53e8-4fe9-86cc-ae505271f9af)

