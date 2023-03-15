#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define BOARD_HEIGHT 8
#define BOARD_WIDTH 7

typedef struct shapes {
    int height, width;
    struct shapes *next;
    int *data;
}shapes;

typedef struct shapes_list {
    struct shapes *shapes;
    struct shapes_list *next;
}shapes_list;

typedef struct solutions {
    int *board;
    struct solutions *next;
}solutions;

void print_shape(shapes *shape);
shapes* load_shape_from_file(const char* file_name);

//Function to free shape and the data it contains
void free_shapes(shapes *shape);
//Function to free all shapes
void free_all_shapes(shapes *shape);
//Function to free list
void free_list(shapes_list *list);

//Check if two shapes are equal
bool shapes_equal(shapes *shape_1, shapes *shape_2);
//Check through list if shape equal to any of them
bool shape_equal_in_list(shapes *shape, shapes *other_shapes);
// Rotate a shape 90 degrees to the right and adds it to the end of the new_shapes if it does not already exist
//  -> returns if it discarded the shape it created
shapes* rotate_last_90_degrees(shapes *shape, bool isdone);
shapes* mirror_shape(shapes *shape, bool isdone);
// Adds a shape and all its rotations and mirrors (unique) to the list
shapes_list* add_shapes(shapes *new_shape, shapes_list *all_shapes); 

bool can_place(shapes *shape, int x, int y, int *board);
void place(shapes *shape, int x, int y, int *board);

void print_board(int *board);
// Marks the date as not placeable on the board according to the given date
void mark_date(int month, int month_day, int day, int *board);
// Marks all the spots not placeable according to the given date
void generate_board(int month, int month_day, int day, int *board);
void save_board(solutions *solutions, int *board);

void find_solutions(shapes_list *shapes_list, int *board, solutions *solutions);

void print_all_shapes(shapes *shape){
    shapes *shapes_temp = shape;
    while(shapes_temp != NULL){
        print_shape(shapes_temp);
        shapes_temp = shapes_temp->next;
    }
}

void print_shape_list(shapes_list *list){
    shapes_list *shapes_list_temp = list;
    while(shapes_list_temp != NULL){
        print_all_shapes(shapes_list_temp->shapes);
        shapes_list_temp = shapes_list_temp->next;
    }
}

void add_shapes_2(shapes *new_shape, shapes_list *all_shapes);

int main()
{
    printf("Step : Start\n\n");

    shapes_list *slist = NULL;
    // shapes_list *slist = (shapes_list*)malloc(sizeof(shapes_list));
    // slist->next = NULL;
    // slist->shapes = NULL;
    char filename[20];
    for (int i = 1; i <= 10; i++) {
        sprintf(filename, "shapes/%d.txt", i);
        shapes *shape = load_shape_from_file(filename);
        slist = add_shapes(shape, slist);
        printf("Loaded shape %d\n", i);
        // print_all_shapes(slist->shapes);
    }

    printf("\nStep : Shapes Loaded\n\n");

    // TODO : check if number of slots - 3 are filled
    
    solutions *sols = NULL;
    int board[BOARD_HEIGHT * BOARD_WIDTH] = {0};
    generate_board(0, 0, 6, board);
    
    printf("--------Board Initialized--------\n");
    print_board(board);
    printf("----------------------------------\n\n");


    find_solutions(slist, board, sols);

    //Print solutions
    int counter_sols = 0;
    while(sols != NULL)
    {
        counter_sols++;
        printf("-----Solution %d-----\n", counter_sols);
        print_board(sols->board);
        printf("\n");
        sols = sols->next;
    }

    //Free list
    free_list(slist);
}

shapes* load_shape_from_file(const char* file_name)
{
    shapes *shape = (shapes*)malloc(sizeof(shapes));
    
    FILE* fp;
    int i;

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", file_name);
        exit(1);
    }
    shape->next = NULL;
    shape->height = 0;
    shape->width = 0;
    fscanf(fp, "%d", &shape->height);
    fscanf(fp, "%d", &shape->width);

    shape->data = (int*) malloc(shape->height * shape->width * sizeof(int));
    if (shape->data == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer de la mémoire\n");
        exit(1);
    }

    for (i = 0; i < shape->height * shape->width; i++) {
        fscanf(fp, "%d", &shape->data[i]);
    }

    fclose(fp);

    return shape;
}

void print_shape(shapes *shape)
{
    for (int i = 0; i < shape->height; i++) {
        for (int j = 0; j < shape->width; j++) {
            printf("%d ", shape->data[i * shape->width + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void free_shapes(shapes *shape)
{
    free(shape->data);
    free(shape);
}

void free_all_shapes(shapes *shape)
{
    shapes *temp;
    while(shape != NULL)
    {
        temp = shape;
        shape = shape->next;
        free_shapes(temp);
    }
}

void free_list(shapes_list *list){
    shapes_list *temp;
    while(list != NULL)
    {
        temp = list;
        list = list->next;
        free_all_shapes(temp->shapes);
        free(temp);
    }
}

bool shapes_equal(shapes *shape_1, shapes *shape_2){
    //Start by comparing the sizes
    if ((shape_1->width != shape_2->width) || (shape_1->height != shape_2->height))
        return false;
    
    //Then compare the contents
    for (int y = 0; y < shape_1->height; y++){
        int i = y * shape_1->width;
        for (int x = 0; x < shape_1->width; x++)
        {
            int j = i + x;
            if (shape_1->data[j] != shape_2->data[j])
                return false;
        }
    }

    return true;
}

bool shape_equal_in_list(shapes *shape, shapes *other_shapes){
    shapes *temp = other_shapes;
    while (temp != NULL)
    {
        if (shapes_equal(shape, temp))
        {
            return true;
        }
    
        temp = temp->next;
    }
    
    return false;
}

shapes* rotate_last_90_degrees(shapes *shape, bool isdone)
{

    //Declare new shape
    shapes *new_shape = malloc(sizeof(shapes));

    new_shape->width = shape->height;
    new_shape->height = shape->width;

    //Create array for new data and manually allocate memory
    int* n_data = (int*)malloc(sizeof(int) * new_shape->width * new_shape->height);
    
    //Loop over shape and rotate it 90deg by swapping x and y when getting from original shape
    for (int y = 0; y < new_shape->height; y++){
        int i = y * new_shape->width;
        for (int x = 0; x < new_shape->width; x++)
            n_data[i + x] = shape->data[(shape->height - x - 1) * shape->width + y];
    }

    //Add array to shape
    new_shape->data = n_data;
    
    //Check if shape already in shapes array
    if (shape_equal_in_list(new_shape, shape))
    {
        isdone = true;
        free_shapes(new_shape);
        return shape;
    }
    
    new_shape->next = shape;
    isdone = false;

    return new_shape;
}

shapes* mirror_shape(shapes *shape, bool isdone){
    //Declare new shape
    shapes *new_shape = malloc(sizeof(shapes));

    new_shape->width = shape->width;
    new_shape->height = shape->height;

    //Create array for new data and manually allocate memory
    int* n_data = (int*)malloc(sizeof(int) * new_shape->width * new_shape->height);
    //Loop over shape and rotate it 90deg by swapping x and y when getting from original shape
    for (int y = 0; y < new_shape->height; y++){
        int i = y * new_shape->width;
        for (int x = 0; x < new_shape->width; x++){
            int  j = i + x;
            n_data[j] = shape->data[shape->width - x - 1 + i];
        }
    }

    //Add array to shape
    new_shape->data = n_data;

    //Check if shape is not symetric to the shape created
    if (shape_equal_in_list(new_shape, shape)){
        free_shapes(new_shape);
        isdone = true;
        return shape;
    }
    
    new_shape->next = shape;
    isdone = false;


    return new_shape;
}

shapes_list* add_shapes(shapes *new_shape, shapes_list *all_shapes){
    //For each rotation create a shape and make it from the previous shape,
    //then check it isn't equal to other shapes in list
    
    
    bool isdone = false;
    new_shape = rotate_last_90_degrees(new_shape, isdone); //90deg rotations
    if (!isdone)
        new_shape = rotate_last_90_degrees(new_shape, isdone); //180deg rotations
    if (!isdone)
        new_shape = rotate_last_90_degrees(new_shape, isdone); //270deg rotations
    
    new_shape = mirror_shape(new_shape, isdone); //Flip
    
    if(!isdone)
        new_shape = rotate_last_90_degrees(new_shape, isdone); //90deg rotations of flip
    if (!isdone)
        new_shape = rotate_last_90_degrees(new_shape, isdone); //180deg rotations of flip
    if (!isdone)
        new_shape = rotate_last_90_degrees(new_shape, isdone); //270deg rotations of flip

    shapes_list *new_list = (shapes_list*)malloc(sizeof(shapes_list));
    
    new_list->next = all_shapes;
    new_list->shapes = new_shape;

    return new_list;
}

bool can_place(shapes *shape, int x, int y, int *board)
{  
    int board_location = y * BOARD_WIDTH + x;

    // Check if the shape is out of bounds
    if (y + shape->height > BOARD_HEIGHT || x + shape->width > BOARD_WIDTH)
        return false;

    // For each marker of the shape, we check if there is a collision
    for (int i = 0; i < shape->height; i++) {
        for (int j = 0; j < shape->width; j++) {
            if (shape->data[i * shape->width + j] == 0)
                continue;
            // There is a collision, we stop
            if (board[(i + y) * BOARD_WIDTH + j + x] != 0)
            {
                return false;
            }
        }
    }

    return true;
}

void place(shapes *shape, int x, int y, int *board)
{
    int board_location = y * BOARD_WIDTH + x;
    
    for (int i = 0; i < shape->height; i++) {
        for (int j = 0; j < shape->width; j++) {
            if (shape->data[i * shape->width + j] == 0)
                continue;
            board[(i + y) * BOARD_WIDTH + j + x] = shape->data[i * shape->width + j];
        }
    }
}

void print_board(int *board) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            printf("%d\t", board[i * BOARD_WIDTH + j]);
        }
        printf("\n");
    }
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

    // Blocks the border slots
    board[6] = -1;
    board[13] = -1;
    board[49] = -1;
    board[50] = -1;
    board[51] = -1;
    board[52] = -1;
}

void copy(int *initial_board, int *new_board)
{

}

void save_board2(solutions **solutions, int *board)
{
    struct solutions *new_solution = (struct solutions*)malloc(sizeof(struct solutions));

    int board_copy[BOARD_HEIGHT * BOARD_WIDTH];
    memcpy(board_copy, board, sizeof(int) * BOARD_HEIGHT * BOARD_WIDTH);

    new_solution->board = board_copy;
    new_solution->next = *solutions;
    *solutions = new_solution;
}

void save_board(solutions *solutions, int *board)
{
    struct solutions *new_solution = (struct solutions*)malloc(sizeof(struct solutions));

    int *board_copy = (int*)malloc(sizeof(int) * BOARD_HEIGHT * BOARD_WIDTH);
    memcpy(board_copy, board, sizeof(int) * BOARD_HEIGHT * BOARD_WIDTH);


    new_solution->board = board_copy;
    new_solution->next = NULL;

    struct solutions *temp = solutions;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    
    temp->next = new_solution;
}

int depth = 0;
void debug_exploration(bool up){
    if (up){
        depth--;
    }else{
        depth++;
    }

    printf("Current depth %d\n", depth);
}

void find_solutions(shapes_list *all_shapes, int *board, solutions *solutions)
{
    if (all_shapes == NULL)
    {
        // puts("Ended");
        printf("\n-------solution_found-------\n");
        print_board(board);
        // exit(1);
        // save_board(solutions, board);
        return;
    }
    
    shapes *current_shape = all_shapes->shapes;

    // For each shape variant
    while (current_shape != NULL)
    {
        // For each slots
        for(int y = 0; y < BOARD_HEIGHT; y++)
        {
            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                if (can_place(current_shape, x, y, board))
                {
                    //Copy board
                    // int *new_board = (int*)malloc(sizeof(int) * BOARD_HEIGHT * BOARD_WIDTH);
                    int new_board[BOARD_HEIGHT * BOARD_WIDTH];
                    memcpy(new_board, board, sizeof(int) * BOARD_HEIGHT * BOARD_WIDTH);
                    //Place piece on board
                    place(current_shape, x, y, new_board);
                    
                    // puts(" === ");
                    // print_board(new_board);

                    //Continue recursion

                    // debug_exploration(false);
                    find_solutions(all_shapes->next, new_board, solutions);
                    // debug_exploration(true);

                    // free(new_board);
                }
            }
        }

        current_shape = current_shape->next;
    }
}



// void add_shapes(shapes *new_shape, shapes_list **all_shapes){
//     //For each rotation create a shape and make it from the previous shape,
//     //then check it isn't equal to other shapes in list
    
    
//     bool isdone = rotate_last_90_degrees(&new_shape); //90deg rotations
//     if (!isdone)
//         isdone = rotate_last_90_degrees(&new_shape); //180deg rotations
//     if (!isdone)
//         isdone = rotate_last_90_degrees(&new_shape); //270deg rotations
    
    
    
//     isdone = mirror_shape(&new_shape); //Flip
    
//     if(!isdone)
//         isdone = rotate_last_90_degrees(&new_shape); //90deg rotations of flip
//     if (!isdone)
//         isdone = rotate_last_90_degrees(&new_shape); //180deg rotations of flip
//     if (!isdone)
//         isdone = rotate_last_90_degrees(&new_shape); //2700deg rotations of flip

//     if ((*all_shapes)->shapes == NULL)
//     {
//         (*all_shapes)->shapes = new_shape;
//         return;
//     }

//     shapes_list *new_list = (shapes_list*)malloc(sizeof(shapes_list));
    
//     // new_list->next = *all_shapes;
//     // new_list->shapes = new_shape;

//     // *all_shapes = new_list;

//     new_list->next = NULL;
//     new_list->shapes = new_shape;

//     shapes_list *current = *all_shapes;
//     while (current->next != NULL)
//     {
//         current = current->next;
//     }

//     current->next = new_list;
// }