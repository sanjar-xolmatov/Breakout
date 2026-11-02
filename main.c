#include "main.h"
#include <raylib.h>
#include <math.h>
#include <raymath.h>
#include <stdio.h>

int main(void) {
    const int screenWidth = 805;
    const int screenHeight = 1000;

    const float brickHeight = 15.0;
    const float brickWidth = 40.0;

    const float angle = 60.0;
    const int rows = 8;
    const int columns = 18;

    FILE* fptr;

    fptr = fopen("./data/record.txt", "r");

    if (fptr == NULL) {
        printf("Error opening file");
        return 1;
    }



    GameScreen currentScreen = TITLE;

    Brick bricks[rows * columns];

    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            float x = 2.5 + j * (40.0 + 5.0);
            float y = 60 + i * 20.0;

            if (i == 0 || i == 1) {
                Brick brick = { { x, y, 40.0, 15.0 }, 1, 3, 3, RED };
                bricks[count] = brick;
            };
            if (i == 2 || i == 3){
                Brick brick = { { x, y, 40.0, 15.0 }, 1, 2, 2, ORANGE };
                bricks[count] = brick;
            };
            if (i == 4 || i == 5){
                Brick brick = { { x, y, 40.0, 15.0 }, 1, 1, 2, YELLOW };
                bricks[count] = brick;
            };
            if (i == 6 || i == 7){
                Brick brick = { { x, y, 40.0, 15.0 }, 1, 1, 1, GREEN };
                bricks[count] = brick;
            };
            count++;
        }
    }

    int game_run = 1;

    char text[16];
    char score_text[32];

    InitWindow(screenWidth, screenHeight, "Breakout");
    InitAudioDevice();

    Sound paddle_sound = LoadSound("./assets/kenney_impact-sounds/Audio/impactMetal_medium_004.ogg");
    Sound brick_sound = LoadSound("./assets/kenney_impact-sounds/Audio/impactGeneric_light_003.ogg");

    Ball ball = { (Vector2){(screenWidth + 10.0)/2, 400}, 400.0, 300.0, 300.0, 10.0, RAYWHITE };
    Paddle paddle = { { (screenWidth - 80.0f)/2, 950.0f, 80.0f, 15.0f }, 400.0f, 3, 0, BLUE };

    SetTargetFPS(60);

    Vector2 previus;
    Vector2 mousePoint;

    Button playButton = { { (screenWidth - 200.0)/2, 400.0, 200.0, 60.0 }, "PLAY", BLUE };

    while (!WindowShouldClose()) {
        switch (currentScreen) {
            case TITLE: {
                if (IsKeyPressed(KEY_ENTER)) currentScreen = GAME;
                mousePoint = GetMousePosition();

                if (CheckCollisionPointRec(mousePoint, playButton.rec)) {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) currentScreen = GAME;
                }

                if (CheckCollisionPointRec(mousePoint, playButton.rec)) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        playButton.col = ORANGE;
                    }
                    else {
                        playButton.col = GREEN;
                    }
                }
                else {
                    playButton.col = BLUE;
                }
            } break;
            case GAME: {
                float deltaTime = GetFrameTime();

                previus = ball.position;

                ball.position.x += ball.speedX * deltaTime;
                ball.position.y += ball.speedY * deltaTime;

                if (ball.position.x + ball.radius > screenWidth) ball.speedX *= -1;
                if (ball.position.x - ball.radius < 0) ball.speedX *= -1;
                if (ball.position.y - ball.radius < 0) ball.speedY *= -1;
                if (ball.position.y + ball.radius > screenHeight) {
                    paddle.lives--;
                    ball.position = (Vector2){GetRandomValue(10, 795), 400.0};
                    ball.speedX = 300.0;
                    ball.speedY = 300.0;

                    if (paddle.lives == 0) {
                        game_run = 0;
                        ball.speedX = 0;
                        ball.speedY = 0;
                        ball.speed = 0;
                    }

                };
                if (CheckCollisionCircleRec(ball.position, ball.radius, paddle.rec) && ball.speedY > 0) {
                    float horizontal_difference = ball.position.x - ( paddle.rec.x + paddle.rec.width/2 );
                    float n = Clamp(horizontal_difference / (paddle.rec.width/2), -1, 1);

                    ball.speedX = ball.speed * sin(angle * (M_PI /180.0) * n );
                    ball.speedY = -1 * ball.speed * cos(angle * (M_PI /180.0) * n );
                    PlaySound(paddle_sound);
                }

                for (int i = 0; i < rows * columns; i++) {
                    if (CheckCollisionCircleRec(ball.position, ball.radius, bricks[i].rec) && bricks[i].active == 1) {
                        if (previus.y + ball.radius < bricks[i].rec.y && ball.speedY > 0 && previus.y < ball.position.y) ball.speedY *= -1;
                        if (previus.y - ball.radius > bricks[i].rec.y + bricks[i].rec.height && ball.speedY < 0 && previus.y > ball.position.y) ball.speedY *= -1;
                        if (previus.x - ball.radius > bricks[i].rec.x + bricks[i].rec.width && ball.speedX < 0 && previus.x > ball.position.x) ball.speedX *= -1;
                        if (previus.x + ball.radius < bricks[i].rec.x && ball.speedX > 0 && previus.x > ball.position.x) ball.speedX *= -1;
                        bricks[i].health--;
                        if (bricks[i].health == 0) {
                            bricks[i].active = 0;
                            paddle.score += bricks[i].points;
                        }
                        PlaySound(brick_sound);

                        break;
                    }
                }

                if (IsKeyDown(KEY_RIGHT)) paddle.rec.x += paddle.speed * deltaTime;
                if (IsKeyDown(KEY_LEFT)) paddle.rec.x -= paddle.speed * deltaTime;

                if (IsKeyPressed(KEY_Q)) currentScreen = TITLE;

                paddle.rec.x = Clamp(paddle.rec.x, 0, screenWidth - paddle.rec.width);


                sprintf(text, "LIVES: %d", paddle.lives);
                sprintf(score_text, "SCORE: %d", paddle.score);
            } break;
            default:break;
        }

        BeginDrawing();
        switch (currentScreen) {
            case TITLE: {
                ClearBackground(BLACK);
                DrawText("BREAKOUT", (screenWidth - MeasureText("BREAKOUT", 80))/2, 200, 80, RED);
                DrawRectangle(playButton.rec.x, playButton.rec.y, playButton.rec.width, playButton.rec.height, playButton.col);
                DrawText(playButton.text, (screenWidth - MeasureText(playButton.text, 40))/2, playButton.rec.y + (playButton.rec.height - 40)/2, 40, BLACK);
            } break;
            case GAME: {
                ClearBackground(BLACK);

                DrawText(text, 10, 10, 30, BLUE);
                DrawText(score_text, 650, 10, 30, BLUE);
                if (!game_run) {
                    DrawText("LOST", (screenWidth - MeasureText("LOST", 40))/2, screenHeight/2 - 20, 40, RED);
                }

                for (int i = 0; i < rows * columns; i++) {
                    if (bricks[i].active == 1) {
                        DrawRectangle(bricks[i].rec.x, bricks[i].rec.y, bricks[i].rec.width, bricks[i].rec.height, bricks[i].color);
                    }
                }

                DrawCircleV(ball.position, ball.radius, ball.color);
                DrawRectangle(paddle.rec.x, paddle.rec.y, paddle.rec.width, paddle.rec.height, paddle.color);
            } break;
            default:break;
        }
        EndDrawing();

    }

    UnloadSound(brick_sound);
    UnloadSound(paddle_sound);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}
