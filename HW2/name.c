/**
 * @author David Hines (dhhines)
 * @file name.c
 *
 * Simple C program to demonstrate Unix system calls for creating and synchronizing processes.
 *
 * The program will display the author's last name and first name where the first name is
 * displayed using a parent process and the last name is displayed using a child process.
 *
 * The name format will be Last, First
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    //initializing char pointers to string literals
    char *fname = "David";
    char *lname = "Hines";

    //calling the fork() system call and capturing the return value as integer
    //the parent process would receive an integer value that is the process ID
    //of the child process just created.  The child process would receive a value
    //of 0 as the return value.
    pid_t pid = fork();

    /*  Note:  From this point on there are two processes running this exact code.
        The parent and child processes have copies of the parent address space which
        includes the stack, data and heap of the parent process.  Both of the processes
        continue execution at the same point.  However, we can differentiate between
        the two processes by checking the value of the pid variable */

    if (pid < 0){ //capture fork failure and print error message to stderr
        fprintf(stderr, "Fork call failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { // this will be the child process if pid == 0
        //child process continues execution here and prints last name
        printf("%s, ", lname);

        //child process exits with a status code of 0
        //this terminates the child process
        exit(0);
    }
    else {  //continue execution as the parent process and then wait for child termination

        //wait system call tells parent process to wait for child to complete
        wait(NULL);

        //now parent process can continue execution
        //parent process prints the first name and newline character
        printf("%s\n", fname);
    }

    //parent process exits gracefully
    return(EXIT_SUCCESS);
}
