#include <stdio.h>
#include "raylib.h"

static const int win_width = 400;    //Window width
static const int win_height = 400;  //Window height
static float dt = 0;    //Delta time
static float timer = 0;

typedef struct {
    int x;
    int y;
}   Snake;

typedef struct {
    int x;
    int y;
}Apple;

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void Move(Snake* s);
static int FindLength(size_t a, size_t e);


static int cell_size = 20;

static Snake player[5] = {
    {5,1},
    {4,1},
    {3,1},
    {2,1},
    {1,1},
};

static int player_length = sizeof(player) / sizeof(player[0]);
static float player_update = 0.5;

static Apple APPLES[1] = {
    {10, 12},
};

//static int APPLES_length = size

int main(void)
{

    printf("%zu\n", FindLength(sizeof(player), sizeof(player[0])));
    InitWindow(win_width, win_height, "nipple");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        dt = GetFrameTime();

        timer = timer + dt;

        if (timer >= player_update)
        {
            Move(player);
            timer = 0;
        }

        DrawGame();
    }

    CloseWindow();
    return 0;
}

void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(BLACK);

    for (int i = 0; i < player_length; i++)
    {
        DrawRectangle(player[i].x * cell_size, player[i].y * cell_size, cell_size, cell_size, GREEN);
    }
    // for (int i = 0; i < )

    EndDrawing();
}

void Move(Snake* s)
{
    for (int i = player_length - 1; i >= 0; i--)
    {
        if (i == 0)
        {
            s[i].y = s[i].y + 1;
        }
        else
        {
            s[i] = s[i - 1];
        }
    }
}

int FindLength(size_t a, size_t e)
{
    return a / e;
}