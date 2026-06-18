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
    int world_x;
    int world_y;
    char* space;
}Cell;

typedef struct {
    Cell* spaces;
    int size;
    int width;
    int height;
    int length;
}Grid;

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
    Segment last;
    float update;
}DArray;

static DArray snake;
static Grid grid;

static void darray_init(DArray* arr, int start_capacity);
static void darray_push(DArray* arr, Segment segment);
static void darray_pop(DArray* arr);
static void darray_free(DArray* arr);
static void grid_init(Grid* grid, int size, int width, int height);

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void Move(Segment* s, Direction* d, int length);
static void Eat(void);

static int cell_size = 20;

static Apple APPLES[1] = {
    {5, 5},
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
            Eat();
        }

        if (timer >= snake.update)
        {
            Move(snake.data, direction, snake.length);
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
    Segment head = {1, 1};
    darray_push(&snake, head);

    grid_init(&grid, cell_size, win_width, win_height);
}

void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(BLACK);

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
    for (int i = 0; i < grid.length; i++)
    {
        //grid.spaces[i].x * cell_size;
        DrawRectangleLines(grid.spaces[i].x * cell_size, grid.spaces[i].y * cell_size, grid.size, grid.size, WHITE);
    }
    EndDrawing();
}

void Move(Segment* s, Direction* d, int length)
{
    snake.last.x = snake.data[snake.length - 1].x;
    snake.last.y = snake.data[snake.length - 1].y;
    for (int i = length - 1; i >= 0; i--)
    {
        if (i == 0)
        {
            s[i].y = s[i].y + d[i].y;
            s[i].x = s[i].x + d[i].x;

            if (s[i].x == APPLES[0].x && s[i].y == APPLES[0].y)
            {
                Eat();
            }
        }
        else
        {
            s[i] = s[i - 1];
        }
    }
}

void Eat()
{
    darray_push(&snake, snake.last);

    int i = rand() % grid.length - 1;
    Cell c = grid.spaces[i];

    APPLES[0].x = c.x;
    APPLES[0].y = c.y;
}

void darray_init(DArray* arr, int start_capacity)
{
    arr->data = (Segment*)malloc(start_capacity * sizeof(Segment));
    arr->length = 0;
    arr->capacity = start_capacity;
    arr->update = 0.13;
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

void grid_init(Grid* grid, int size, int width, int height)
{
    int grid_x_count = width / size;
    int grid_y_count = height / size;

    grid->spaces = (Cell*)malloc((grid_x_count * grid_y_count) * sizeof(Cell));
    grid->length = 0;
    grid->size = size;
    grid->width = width;
    grid->height = height;

    int i = 0;

    for (int grid_y = 0; grid_y < grid_y_count; grid_y++)
    {
        for (int grid_x = 0; grid_x < grid_x_count; grid_x++)
        {
            Cell a = {grid_x, grid_y, grid_x * size, grid_y * size};
            grid->spaces[i] = a;
            i++;
            printf("%i, %i\n", grid_x, grid_y);
            grid->length++;
        }
    }
}