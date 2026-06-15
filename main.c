#include <stdio.h>
#include "raylib.h"

static const int screen_width = 800;
static const int screen_height = 450;

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);



int main(void)
{
    printf("%d\n", 155);
    InitWindow(400, 400, "nipple");
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        DrawGame();
    }
}

void DrawGame(void)
{
    BeginDrawing();

    EndDrawing();
}