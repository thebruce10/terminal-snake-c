#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

enum directions {STOP = 0, UP = 1, LEFT = 2, DOWN = 3, RIGHT = 4};
enum boardStates {EMPTY = 0, HEAD = 1, TAIL = 2, FRUIT = 3};

int gameboard[25][25] = {{0}};


int tail[625][2];
int fruit[8][2];

int score = 0;
bool running = false;
int dir = RIGHT;
int headX = 0;
int headY = 0;

int game();
void loop();
void setup();
int rangedRand(int min, int max);
void input();
void logic();
void fillBoard();
void render();
void move();

int main() {

    game();

    return 0;
}

int game() {
    printf("\e[2J"); //clear

    setup();
    
    while(running == true) {
        loop();
    }

    return 0;
}

void setup() {

    dir = RIGHT;
    headX = 0;
    headY = 0;
    score = 0;

    srand(time(NULL));

    for (int i = 0; i < 8; i++) {
        fruit[i][0] = rangedRand(0, 24);
        fruit[i][1] = rangedRand(0, 24);
    }

    for (int i = 0; i < 100; i++) {
        tail[i][0] = 0;
        tail[i][1] = 0;
    }

    
    running = true;
}

int rangedRand(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void loop() {

    input();

    move();

    logic();

    fillBoard();

    render();

    Sleep(200);
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                if (dir != RIGHT){
                    dir = LEFT;
                }
                break;

            case 'd':
                if (dir != LEFT){
                    dir = RIGHT;
                }
                break;

            case 'w':
                if (dir != DOWN) {
                    dir = UP;
                }
                break;

            case 's':
                if (dir != UP) {
                    dir = DOWN;
                }
                break;

            case 'x':
                running = false;
                break;
        }
    }
}

void move() {
    for (int i = 625; i >= 0; i--) {
        if (i != 0){
            tail[i][0] = tail[i-1][0];
            tail[i][1] = tail[i-1][1];
        } else {
            tail[i][0] = headX;
            tail[i][1] = headY;
        }
    }

    switch (dir) {
        case UP:
            headY = --headY;
            break;

        case LEFT:
            headX = --headX;
            break;

        case DOWN:
            headY = ++headY;
            break;
        
        case RIGHT:
            headX = ++headX;
            break;
    }
}

void logic() {
    if (headX < 0 || headX >= 25 || headY < 0 || headY >= 25) {
        running = false;
    }

    for (int i = 0; i < 8; i++) {
        if (headX == fruit[i][0] && headY == fruit[i][1]) {
            score++;
            fruit[i][0] = rangedRand(0, 24);
            fruit[i][1] = rangedRand(0, 24);
        }
    }

    for (int i = 0; i < score; i++) {
        if (headX == tail[i][0] && headY == tail[i][1]) {
            running = false;
        }
    }

    if (score > 625) {
        running = false;
    }


}

void fillBoard() {
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 25; j++) {

            gameboard[i][j] = EMPTY;

        }
    }

    for (int i = 0; i < score; i++) {
        gameboard[ tail[i][0] ][ tail[i][1] ] = TAIL;
    }

    for (int i = 0; i < 8; i++) {
        gameboard[ fruit[i][0] ][ fruit[i][1] ] = FRUIT;
    }


    gameboard[headX][headY] = HEAD;
}

void render() {
    printf("\033[1;1f"); //move the cursor to the top left corner

    char frame[1500];
    int frameLocation = 0;

    for (int i = 0; i < 54; i++) {
        frame[frameLocation++] = 219;
    }
    frame[frameLocation++] = '\n';

    for (int i = 0; i < 25; i++) {
        frame[frameLocation++] = 219;
        frame[frameLocation++] = 219;

        for (int j = 0; j < 25; j++) {
            switch (gameboard[j][i]) {
                case EMPTY:
                    frame[frameLocation++] = ' ';
                    frame[frameLocation++] = ' ';
                    break;

                case HEAD:
                    switch (dir) {
                        case UP:
                            frame[frameLocation++] = '/';
                            frame[frameLocation++] = '\\';
                            break;

                        case LEFT:
                            frame[frameLocation++] = '<';
                            frame[frameLocation++] = '<';
                            break;

                        case DOWN:
                            frame[frameLocation++] = '\\';
                            frame[frameLocation++] = '/';
                            break;

                        case RIGHT:
                            frame[frameLocation++] = '>';
                            frame[frameLocation++] = '>';
                            break;
                            

                    }
                    break;

                case TAIL:
                    frame[frameLocation++] = ':';
                    frame[frameLocation++] = ':';
                    break;

                case FRUIT:
                    frame[frameLocation++] = '@';
                    frame[frameLocation++] = '^';
                    break;

            }
        }
        
        frame[frameLocation++] = 219;
        frame[frameLocation++] = 219;
        frame[frameLocation++] = '\n';
    }

    for (int i = 0; i < 54; i++) {
        frame[frameLocation++] = 219;
    }

    frame[frameLocation] = '\0';


    printf("%s\nscore: %i\n", frame, score);
}