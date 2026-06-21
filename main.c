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
    Direction* queue;
    int length;
    int capacity;
}DirectionQueue;

typedef struct {
    Segment* data;
    int length;
    int capacity;
    Segment last;
    float update;
}DArray;

static DArray snake;
static Grid grid;
static DirectionQueue direction_queue;

static void DarrayInit(DArray* arr, int start_capacity);
static void DarrayPush(DArray* arr, Segment segment);
static void DarrayPop(DArray* arr);
static void DarrayFree(DArray* arr);
static void DirectionQueueInit(DirectionQueue* arr, int start_capacity);
static void DirectionQueuePush(DirectionQueue* arr, Direction direction);
static Direction DirectionQueuePop(DirectionQueue* arr, int index);
static void GridInit(Grid* grid, int size, int width, int height);
static int GetIndex(int x, int y);
static bool IsFree(int i);

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void Move(Segment* s, DirectionQueue* d, int length);
static void Eat(void);

static int cell_size = 20;

static Apple APPLES[1] = {
    {4, 1},
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

        Direction directiion = {0, 0};

        if (IsKeyPressed(KEY_RIGHT))
        {
            directiion.x = 1;
            directiion.y = 0;
            DirectionQueuePush(&direction_queue, directiion);
        }
        else if(IsKeyPressed(KEY_DOWN))
        {
            directiion.x = 0;
            directiion.y = 1;
            DirectionQueuePush(&direction_queue, directiion);
        }
        else if(IsKeyPressed(KEY_LEFT))
        {
            directiion.x = -1;
            directiion.y = 0;
            DirectionQueuePush(&direction_queue, directiion);
        }
        else if(IsKeyPressed(KEY_UP))
        {
            directiion.x = 0;
            directiion.y = -1;
            DirectionQueuePush(&direction_queue, directiion);
        }

        if (IsKeyPressed(KEY_A))
        {
            Eat();
        }

        UpdateGame();
        


        DrawGame();
    }

    CloseWindow();
    return 0;
}

void InitGame(void)
{
    DarrayInit(&snake, 5);
    Segment head = {1, 1};
    DarrayPush(&snake, head);

    DirectionQueueInit(&direction_queue, 3);
    Direction directionn = {0, 1};
    DirectionQueuePush(&direction_queue, directionn);

    GridInit(&grid, cell_size, win_width, win_height);

    for (int i = 0; i < snake.length; i++)
    {
        grid.spaces[GetIndex(snake.data[i].x, snake.data[i].y)].space = "snake";
    }

    grid.spaces[GetIndex(APPLES[0].x, APPLES[0].y)].space = "apple";
}

void UpdateGame(void)
{
    /*if (direction_queue.length == 1)
    {
        DirectionQueuePush(&direction_queue, direction_queue.queue[0]);
    }*/


    if (timer >= snake.update)
    {
        printf("%i", direction_queue.length);
        Move(snake.data, &direction_queue, snake.length);
        timer = 0;
    }

    for (int i = 0; i < snake.length; i++)
    {
        if (GetIndex(snake.data[i].x, snake.data[i].y) != -1)
        {
            grid.spaces[GetIndex(snake.data[i].x, snake.data[i].y)].space = "snake";

            grid.spaces[GetIndex(APPLES[0].x, APPLES[0].y)].space = "apple";
        }
        if (i == 0 && grid.spaces[GetIndex(snake.last.x, snake.last.y)].space == "snake")
        {
            grid.spaces[GetIndex(snake.last.x, snake.last.y)].space = "";
        }
    }

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
        DrawText(grid.spaces[i].space, grid.spaces[i].x * cell_size, grid.spaces[i].y * cell_size, 18, WHITE);
    }
    EndDrawing();
}

void Move(Segment* s, DirectionQueue* d, int length)
{
    snake.last.x = snake.data[snake.length - 1].x;
    snake.last.y = snake.data[snake.length - 1].y;
    for (int i = length - 1; i >= 0; i--)
    {
        if (i == 0)
        {
            if (d->length > 0)
            {
                direction[i] = DirectionQueuePop(d, i);
            }
            
            s[i].y = s[i].y + direction[i].y;
            s[i].x = s[i].x + direction[i].x;

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

    int i = rand() % (grid.length);
    Cell c = grid.spaces[i];

    if (c.space != "")
    {
        if (snake.length < grid.length - 1)
        {
            return Eat();
        }
        else
        {
            c.x = -1000;
            c.y = -1000;
        }
    }
    
    DarrayPush(&snake, snake.last);

    APPLES[0].x = c.x;
    APPLES[0].y = c.y;
}

void DarrayInit(DArray* arr, int start_capacity)
{
    arr->data = (Segment*)malloc(start_capacity * sizeof(Segment));
    arr->length = 0;
    arr->capacity = start_capacity;
    arr->update = 0.5;
}

void DarrayPush(DArray* arr, Segment segment)
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

void DarrayPop(DArray* arr)
{
    if (arr->length > 0)
    {
        arr->length--;
    }
}

void DarrayFree(DArray* arr)
{  
    free(arr->data);
    arr->data = NULL;
    arr->length = 0;
    arr->capacity = 0;
}

void DirectionQueueInit(DirectionQueue* arr, int start_capacity)
{
    arr->queue = (Direction*)malloc(start_capacity * sizeof(Direction));
    arr->length = 0;
    arr->capacity = start_capacity;
}

void DirectionQueuePush(DirectionQueue* arr, Direction direction)
{
    if (arr->length >= arr->capacity)
    {
        int new_capacity = arr->capacity * 2;
        Direction* new_queue = (Direction*)realloc(arr->queue, new_capacity * sizeof(Direction));

        if (new_queue == NULL)
        {
            return;
        }
        arr->queue = new_queue;
        arr->capacity = new_capacity;
    }

    arr->queue[arr->length] = direction;
    arr->length++;
}

Direction DirectionQueuePop(DirectionQueue* arr, int index)
{
    if(arr->length > 0)
    {
        Direction direction = arr->queue[index];
        for (int i = index; i < arr->length - 1; i++)
        {
            arr->queue[i] = arr->queue[i+1];
        }
        arr->length--;
        return direction;
    }
}

void GridInit(Grid* grid, int size, int width, int height)
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
            grid->spaces[i].space = "";
            i++;
            grid->length++;
        }
    }
}

int GetIndex(int x, int y)
{
    int index = x + y * grid.width / grid.size;
    if (index < -1 || index > grid.length-1)
    {
        return -1;
    }
    else
    {
        return index;
    }
}

bool IsFree(int i)
{
    Cell a = grid.spaces[i];
    if (a.space != "")
    {
        return false;
    }
    else
    {
        return true;
    }
}