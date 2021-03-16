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
 * Compile commands: gcc -Wall -g -std=c99 find.c -o find *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//initial capacity of the line buffer
#define INIT_CPCTY 10
//standard multiplier for increasing size of line buffer
#define STD_INCRMT 2
//maximum word size
#define MAX_BUFFER 200
//number of possible processes
#define NUM_PROCS 5

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
void searchAndPrint(char const *word, char const *input)
{
    FILE *fp = fopen(input, "r");
    if (!fp){
        fprintf(stderr, "Can't open file %s\n", input);
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
            //printf("word and tempword values %s %s\n", word, tempWord);
            if (strcmp(word, tempWord) == 0){
                printf("PID: %d ", getpid());
                fprintf(stdout, "%s:  %s\n", word, linetext);
                break;
            }
        }
        cursor = 0;
    }

    fclose(fp);
}

/**
 * Main program for the find application
 * @param argc the number of arguments passed from commandline
 * @param argv array of character pointers to the argurments entered on the commandline
 */
int main(int argc, char *argv[])
{
    //array of process ID variables
    pid_t pid[NUM_PROCS];

    //the word provided for search
    char *word = argv[1];

    //loop through all provided text files and spawn processes
    //parent process will always break and wait after loop for child
    for (int i = 0; i < argc - 2; i++){

        pid[i] = fork();

        if (pid[i] == 0){
            searchAndPrint(word, argv[i + 2]);
        }
        else {
            break;
        }
    }

    wait(NULL);

    return EXIT_SUCCESS;
}
