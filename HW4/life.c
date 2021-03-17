/**
 * @author David Hines (dhhines)
 * @file life.c
 *
 * This program emulates a game of life simulation in which each cell of a M x N 2-Dimensional
 * array is identified by a 1 or 0.  Each generation of the grid will update every cell based
 * on the contents of its neighbor cells (see rules below).  The initial grid is populated
 * using an input file provided as an argument on the command line and the number of generations
 * for the game to play out is provided as a second argument on the command line (as an integer).
 *
 * Rules:
 *  - A 1 cell value stays 1 if exactly two or three neighbors are 1 valued.
 *  - A 1 cell value becomes 0 if less than two or greater than three neighbors are 1 valued.
 *  - A 0 cell value becomes 1 if exactly three neighbors are 1 valued.
 *  - A 0 cell value stays 0 if less than three or greater than three neighbors are 1 valued.
 *
 * This program utilizes threads to execute each cell value in parallel which will ultimately
 * populate the final table after the proper number of generations has been executed.
 *
 * Compile commands: gcc -Wall -g -std=c99 life.c -o life -lpthread
 *
 * Usage: ./life <input file> <integer for generations>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>


//struct for holding the shared data for threads
typedef struct Data_struct {
    int currGen; //the current generation
    int rows;  //number of rows in grid
    int cols;  //number of columns in grid
    int **currGrid; //declare pointer to starting gen 2D array; dynamically created later
    int **nextGenGrid; //declare pointer to next gen 2D array; dynamically created later
    int m; //the current row in M x N grid
    int n; //the current column in M x N grid
}Data;

/**
 * This function is used by each thread to calculate the cell it is assigned with the
 * proper next generation value based on the rules provided (see header section). The
 * new value for the cell is set in the nextGenGrid array so as not to change the
 * currGrid values while the other threads work in parallel to update their respective
 * cells.
 *
 * @param param  pointer to the parameter passed to the function
 */
void *genUpdate(void *param)
{
    //cast the void param pointer to the Data struct type using new struct declaration
    Data *t_data = (Data *)param;

    //sum of all neighbors
    int sum = 0;

    //check the row back up --> m - 1
    if (t_data->m > 0){

        //check column back --> n - 1
        if (t_data->n > 0)
            sum += t_data->currGrid[t_data->m - 1][t_data->n - 1];

        //check current column --> n
        sum += t_data->currGrid[t_data->m - 1][t_data->n];

        //check column forward --> n + 1
        if (t_data->n < t_data->cols - 1)
            sum += t_data->currGrid[t_data->m - 1][t_data->n + 1];
    }

    //check the current row --> m
    //check column back --> n - 1
    if (t_data->n > 0)
        sum += t_data->currGrid[t_data->m][t_data->n - 1];

    //check column forward --> n + 1
    if (t_data->n < t_data->cols - 1)
        sum += t_data->currGrid[t_data->m][t_data->n + 1];

    //check the row forward and down --> m + 1
    if (t_data->m < t_data->rows - 1){

        //check column back --> n - 1
        if (t_data->n > 0)
            sum += t_data->currGrid[t_data->m + 1][t_data->n - 1];

        //check current column --> n
        sum += t_data->currGrid[t_data->m + 1][t_data->n];

        //check column forward --> n + 1
        if (t_data->n < t_data->cols - 1)
            sum += t_data->currGrid[t_data->m + 1][t_data->n + 1];
    }

    if (t_data->currGrid[t_data->m][t_data->n] == 1){
        if (sum == 2 || sum == 3)
            t_data->nextGenGrid[t_data->m][t_data->n] = 1;
        else
            t_data->nextGenGrid[t_data->m][t_data->n] = 0;
    }
    if (t_data->currGrid[t_data->m][t_data->n] == 0){
        if (sum == 3)
            t_data->nextGenGrid[t_data->m][t_data->n] = 1;
        else
            t_data->nextGenGrid[t_data->m][t_data->n] = 0;
    }

    sum = 0;
    pthread_exit(0);
}

/**
 * Function that prints the specified version of the grid to the console
 *
 * @param shrdData pointer to the Data struct holding the current generation grid values
 * @param version  char indicating which grid version to print, 'c' current Grid and 'n' next gen grid
 */
void printGrid(Data *shrdData, char version)
{

    for (int i = 0; i < shrdData->rows; i++){
        for (int j = 0; j < shrdData->cols; j++)
            if (version == 'c')
                printf("%d ", shrdData->currGrid[i][j]);
            else if (version == 'n')
                printf("%d ", shrdData->nextGenGrid[i][j]);
            else
                printf("Invalid character");
        printf("\n");
    }
    printf("\n");

}

/**
 * Main program for the life application
 * @param argc the number of arguments passed from commandline
 * @param argv array of character pointers to the argurments entered on the commandline
 */
int main(int argc, char *argv[])
{

    //Initialize the grid struct to hold the grid array
    Data *shrdData = (Data *) malloc (sizeof(Data));

    //set the current generation to 0 for starters
    shrdData->currGen = 0;
    //set the total number of generations from second command line argument
    int totalGens = atoi(argv[2]);

    //create file buffer open to the filename passed as first command line argument
    FILE *fp = fopen(argv[1], "r");

    //int rows;  //number of rows in grid
    //int cols;  //number of columns in grid
    //scan in the first two values from file which are rows and columns of the lifeGrid (M x N)
    fscanf(fp, "%d%d", &shrdData->rows, &shrdData->cols);

    //create the dynamic memory array in the struct using number of rows provided
    shrdData->currGrid = (int **) malloc (shrdData->rows * sizeof(int *));
    shrdData->nextGenGrid = (int **) malloc (shrdData->rows * sizeof(int *));

    //now loop through the rows to create the columns for each in dynamic memory and zero them
    for (int i = 0; i < shrdData->rows; i++){
        shrdData->currGrid[i] = (int *) calloc (shrdData->cols, sizeof(int));
        shrdData->nextGenGrid[i] = (int *) calloc (shrdData->cols, sizeof(int));
    }

    //use the M x N values for the grid to populate the initial values from the input file for startGrid
    for (int i = 0; i < shrdData->rows; i++)
        for (int j = 0; j < shrdData->cols; j++)
            fscanf(fp, "%d", &shrdData->currGrid[i][j]);

    //Print the current generation grid to the console
    printf("Initial grid:\n");
    printGrid(shrdData, 'c');

    int numThreads = shrdData->cols * shrdData->rows;

    //array of threads used to calculate generations
    pthread_t threads[numThreads];
    //set of thread attributes for each worker thread
    pthread_attr_t attr;
    //set the thread attributes
    pthread_attr_init(&attr);

    //Loop for the proper number of generations
    for (int z = 0; z < totalGens; z++){
        //set the m and n variables to 0 before threads update the next generation
        shrdData->m = 0;
        shrdData->n = 0;
        shrdData->currGen = z;

        //loop through the total number of threads to have one for each grid cell to be updated
        for (int i = 0; i < numThreads; i++){

            //set the m and n variables to the current cell value to be updated by the new thread
            shrdData->m = i / shrdData->cols;
            shrdData->n = i % shrdData->rows;
            //create the new thread and pass it the id, attr, the function and data struct
            pthread_create(&threads[i], &attr, genUpdate, shrdData);
        }

        //thread join for each thread ID when each thread completes its task
        for (int i = 0; i < numThreads; i++)
            pthread_join(threads[i], NULL);

        printf("Next Generation Grid #%d:\n", shrdData->currGen + 1);
        printGrid(shrdData, 'n');

        //copy the nextGenGrid data to the currGrid data and zero out the nextGenGrid for next generation
        for(int i = 0; i < shrdData->rows; i++){
            memcpy(shrdData->currGrid[i], shrdData->nextGenGrid[i], shrdData->cols * sizeof(int));
            memset(shrdData->nextGenGrid[i], 0, shrdData->cols * sizeof(int));
        }
    }

    //free the memory for each dynamic array in the Data struct
    for (int i = 0; i < shrdData->rows; i++){
        free(shrdData->currGrid[i]);
        free(shrdData->nextGenGrid[i]);
    }
    free(shrdData->currGrid);
    free(shrdData->nextGenGrid);

    //free memory for the Data struct
    free(shrdData);

    return EXIT_SUCCESS;
}
