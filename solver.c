#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define BOARD_HEIGHT = 8
#define BOARD_WIDTH = 7

typedef struct shapes {
    int height, width;
    shapes *next;
    int *data;
}shapes;

typedef struct shapes_list {
    shapes *shapes;
    shapes_list *next;
}shapes_list;

typedef struct solutions {
    int *board;
    solutions *next;
}solutions;

// Rotate a shape 90 degrees to the right and adds it to the end of the new_shapes if it does not already exist
//  -> data malloc - needs to be freed in end of main
void rotate_90_degrees(shapes *new_shape, shapes *shape);
// Adds a shape and all its rotations and mirrors (unique) to the list
void add_shapes(shapes *new_shape, shapes_list *shapes_list);
// Places a shape on the grid if possible (and returns if it fits)  
bool place(shapes *shape, int x, int y, int *board);

// Marks the date as not placeable on the board according to the given date
void mark_date(int month, int month_day, int day, int *board);
// Marks all the spots not placeable according to the given date
void generate_board(int month, int month_day, int day, int *board);
void save_board(solutions *solutions, int *board);

void find_solutions(shapes_list *shapes_list, int *board, solutions *solutions);

int main()
{

}

void rotate_90_degrees(shapes *new_shape, shapes *shape)
{
    new_shape->width = shape->height;
    new_shape->height = shape->width;

    //Create array for new data and manually allocate memory
    int* n_data = (int*)malloc(sizeof(int) * new_shape->width * new_shape->height);
    //Loop over shape and rotate it 90deg by swapping x and y when getting from original shape
    for (int y = 0; y < new_shape->height; y++)
        for (int x = 0; x < new_shape->width; x++)
            n_data[y * new_shape->width + x] = shape->data[x * shape->height + y];
}

void add_shapes(shapes *new_shape, shapes_list *shapes_list){
    
}

void mark_date(int month, int month_day, int day, int *board)
{
    // -1 on the board corresponds to a block (not placeable slot)
    // Defining the month position (0 1 2 3 4 5 - 6 7 8 9 10 11)
    if (month <= 5)
        board[month] = -1;
    else
        board[month + 1] = -1;

    // Defining the month-day position (months + 2)
    board[month_day + 14] = -1;

    // Defining the day (months + 2 + days + 0 1 2 3 + 4 + 4 5 6)
    if (day <= 3)
        board[day + 45] = -1;
    else
        board[day + 49] = -1;
}

void generate_board(int month, int month_day, int day, int *board)
{
    // First blocks the date slots
    mark_date(month, month_day, day, board);
}