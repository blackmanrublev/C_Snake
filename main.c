#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

static const int win_width = 400;    //Window width
static const int win_height = 400;  //Window height
static float dt = 0;    //Delta time
static float timer = 0;

typedef struct {
    int x;
    int y;
}Snake;

typedef struct {
    int x;
    int y;
}Segment;

typedef struct {
    int x;
    int y;
}Apple;

typedef struct {
    int x;
    int y;
}Direction;

typedef struct {
    Segment* data;
    int length;
    int capacity;
}DArray;

static DArray snake;

static void darray_init(DArray* arr, int start_capacity);
static void darray_push(DArray* arr, Segment segment);
static void darray_pop(DArray* arr);
static void darray_free(DArray* arr);

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void Move(Snake* s, Direction* d);
static void Eat(void);


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

static int APPLES_length = sizeof(APPLES) / sizeof(APPLES[0]);

static Direction direction[1] = {
    {0, 1}
};

int main(void)
{

    InitWindow(win_width, win_height, "nipple");
    SetTargetFPS(60);
    InitGame();

    while(!WindowShouldClose())
    {
        dt = GetFrameTime();

        timer = timer + dt;

        if (IsKeyPressed(KEY_RIGHT))
        {
            direction[0].x = 1;
            direction[0].y = 0;
        }
        else if(IsKeyPressed(KEY_DOWN))
        {
            direction[0].x = 0;
            direction[0].y = 1;
        }
        else if(IsKeyPressed(KEY_LEFT))
        {
            direction[0].x = -1;
            direction[0].y = 0;
        }
        else if(IsKeyPressed(KEY_UP))
        {
            direction[0].x = 0;
            direction[0].y = -1;
        }

        if (IsKeyPressed(KEY_A))
        {
            Segment a = snake.data[snake.length-1];
            a.x = a.x - 1;
            a.y = 1;
            darray_push(&snake, a);
            printf("%i\n", snake.capacity);
        }

        if (timer >= player_update)
        {
            Move(player, direction);
            timer = 0;
        }

        DrawGame();
    }

    CloseWindow();
    return 0;
}

void InitGame(void)
{
    darray_init(&snake, 5);
    Segment head = {11, 1};
    darray_push(&snake, head);
}

void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(BLACK);

    // for (int i = 0; i < player_length; i++)
    // {
    //     DrawRectangle(player[i].x * cell_size, player[i].y * cell_size, cell_size, cell_size, GREEN);
    // }
    for (int i = 0; i < snake.length; i++)
    {
        Segment v = snake.data[i];
        DrawRectangle(v.x * cell_size, v.y * cell_size, cell_size, cell_size, GREEN);
    }
    for (int i = 0; i < APPLES_length; i++)
    {
        Apple v = APPLES[i];
        DrawRectangle(v.x * cell_size, v.y * cell_size, cell_size, cell_size, RED);
    }

    EndDrawing();
}

void Move(Snake* s, Direction* d)
{
    // for (int i = player_length - 1; i >= 0; i--)
    // {
    //     if (i == 0)
    //     {
    //         s[i].y = s[i].y + d[i].y;
    //         s[i].x = s[i].x + d[i].x;
    //     }
    //     else
    //     {
    //         s[i] = s[i - 1];
    //     }
    // }
    for (int i = player_length - 1; i >= 0; i--)
    {
        if (i == 0)
        {
            s[i].y = s[i].y + d[i].y;
            s[i].x = s[i].x + d[i].x;
        }
        else
        {
            s[i] = s[i - 1];
        }
    }
}

void darray_init(DArray* arr, int start_capacity)
{
    arr->data = (Segment*)malloc(start_capacity * sizeof(Segment));
    arr->length = 0;
    arr->capacity = start_capacity;
}

void darray_push(DArray* arr, Segment segment)
{
    if (arr->length >= arr->capacity)
    {
        int new_capacity = arr->capacity * 2;
        Segment* new_data = (Segment*)realloc(arr->data, new_capacity * sizeof(Segment));

        if (new_data == NULL)
        {
            return;
        }
        arr->data = new_data;
        arr->capacity = new_capacity;
    }
    arr->data[arr->length] = segment;
    arr->length++;
}

void darray_pop(DArray* arr)
{
    if (arr->length > 0)
    {
        arr->length--;
    }
}

void darray_free(DArray* arr)
{
    free(arr->data);
    arr->data = NULL;
    arr->length = 0;
    arr->capacity = 0;
}