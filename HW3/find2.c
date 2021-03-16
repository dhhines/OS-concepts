/**
 * @author David Hines (dhhines)
 * @file find.c
 *
 * Program to search files provided on the command line for the provided word.
 * Each file provided will be searched using a separate process.  When the word
 * is found in the provided text files this program will print the word, the line
 * of text in which the word was found and the process ID of the process that
 * performed the search function.
 *
 * Compile commands: gcc -Wall -g -std=c99 find2.c -o find2 -lpthread
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

//initial capacity of the line buffer
#define INIT_CPCTY 10
//standard multiplier for increasing size of line buffer
#define STD_INCRMT 2
//maximum word size
#define MAX_BUFFER 200
//number of possible threads
#define NUM_THREADS 5

struct Commands_Struct {
    int numArgs;
    char **arguments;
    int lastProcessed;
};

typedef struct Commands_Struct Commands;

//void function pointer threads will call as part of pthread_create
void *searchAndPrint(void *param);


/**
 * This function reads a single line of input text from the file stream and then
 * returns the text as a string in a block of dynamically allocated memory.
 *
 * @param *fp pointer to the input file stream to be parsed
 * @return pointer to the dynamic memory block holding the text of a single line
 *  from input file (or user command)
 */
char *readLine(FILE *fp)
{
    //create a resizeable array using malloc and realloc and a capacity integer
    int capacity = INIT_CPCTY;
    char *buffer = (char *) malloc (capacity * sizeof(char) + 1);
    int len = 0;
    char ch;
    int matches = 0;

    while ((matches = (fscanf(fp, "%c", &ch) == 1))){
        if (len >= capacity){
            capacity *= STD_INCRMT;
            buffer = (char *) realloc (buffer, capacity * sizeof(char) + 1);
            if (!buffer){
                free(buffer);
                exit(EXIT_FAILURE);
            }
        }
        if (ch != '\n')
            buffer[len++] = ch;
        else
            break;
    }

    if (matches > 0){
        buffer[len] = '\0';
        return buffer;
    }

    free(buffer);
    return NULL;
}

/**
 * Takes the word provided for the search and the input file name
 * then opens a FILE pointer to the provided input file. The text
 * within the input file is searched for any use of the provided
 * word and when the word is found the line of text containing it
 * is printed to the console.
 *
 * @param word the string of text to find in the input file
 * @param input the filename of the input file to search
 *
 */
void *searchAndPrint(void *param)
{
    Commands *t_cmds = (Commands *)param;

    FILE *fp = fopen(t_cmds->arguments[t_cmds->lastProcessed], "r");
    if (!fp){
        fprintf(stderr, "Can't open file %s\n", t_cmds->arguments[t_cmds->lastProcessed]);
        exit(1);
    }

    //character pointer to the line of text provided by the readLine function call
    char *linetext;

    //character pointer for holding the matched text to compare
    char tempWord[MAX_BUFFER];

    // to keep track of sscanf place in linetext
    int val = 0;
    int cursor = 0;

    //loop through all available text in the input file line by line
    while ((linetext = readLine(fp))){
        while ((sscanf(linetext + cursor, "%s%n", tempWord, &val)) != EOF){
            cursor += val;
            if (strcmp(t_cmds->arguments[0], tempWord) == 0){
                //Note: couldn't figure out how to use the pthread_t value so used integer to
                //identify threads from each other
                printf("TID: %d  ", t_cmds->lastProcessed);
                fprintf(stdout, "%s: %s\n", t_cmds->arguments[0], linetext);
                break;
            }
        }
        cursor = 0;
    }

    fclose(fp);
    pthread_exit(0);
}

/**
 * Main program for the find application
 * @param argc the number of arguments passed from commandline
 * @param argv array of character pointers to the argurments entered on the commandline
 */
int main(int argc, char *argv[])
{
    //array of process ID variables
    pthread_t threads[NUM_THREADS];
    //set of thread attributes for each worker thread
    pthread_attr_t attr;
    //set the thread attributes
    pthread_attr_init(&attr);

    //Initialize the command struct to hold arguments
    Commands *cmds = (Commands *) malloc (sizeof(Commands));
    //Set the number of args to be tracked in the struct using argc-1 value (ignores program name arg)
    cmds->numArgs = argc - 1;
    //Initialize the arguments pointer array to initial capacity
    cmds->arguments = (char **) malloc(sizeof(char *) * cmds->numArgs);
    //Set the last processed to 0 in struct as no commands processed yet
    //this will be used to track which of the files has been processed by threads
    //Note: set to 1 as we are not processing the word provided for search (which is index 0)
    cmds->lastProcessed = 0;

    //create the list of the command line args in dynamic memory
    for(int i = 1; i < argc; ++i) {
        cmds->arguments[i - 1] = malloc(strlen(argv[i]) + 1);
        strcpy(cmds->arguments[i - 1], argv[i]);
    }

    //loop through all provided text files and spawn threads to search the list of files
    for (int i = 0; i < argc - 2; i++){

        //printf("ThreadID: %d  ", i);
        pthread_create(&threads[i], &attr, searchAndPrint, cmds);
        cmds->lastProcessed++;

        //thread join when thread completes task
        pthread_join(threads[i], NULL);
    }

    //free the memory for each argument string
    for (int i = 0; i < cmds->numArgs; i++)
        free(cmds->arguments[i]);

    //free memory for the Command struct
    free(cmds);

    return EXIT_SUCCESS;
}
