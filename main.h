#ifndef MAIN_H
#define MAIN_H

#include <raylib.h>

typedef struct Ball {
    Vector2 position;
    float speed;
    float speedX;
    float speedY;
    float radius;
    Color color;
} Ball ;

typedef struct Paddle {
    Rectangle rec;
    float speed;
    int lives;
    int score;
    Color color;
} Paddle ;

typedef struct Brick {
    Rectangle rec;
    int active;
    int health;
    int points;
    Color color;
} Brick ;

typedef struct Button {
    Rectangle rec;
    char text[32];
    Color col;
} Button ;

typedef enum GameScreen {
    TITLE = 0,
    GAME,
} GameScreen;

#endif
