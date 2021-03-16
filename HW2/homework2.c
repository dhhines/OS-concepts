/**
 * @author David Hines (dhhines)
 * @file homework2.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//global variable initialized to value of 5
int global = 5;

//function declaration for foo()
void foo();

int main() {

    int id = -2;

    //prints 5 to console
    printf("main(): global = %d\n" , global); // global = D (parent data region)

    //prints 3 to console
    //prints 1 to console
    foo();  // staticInt = D (parent data region); localInt = C (parent stack)

    //prints 4 to console
    //prints 1 to console
    foo();  // staticInt = D (parent data region); localInt = C (parent stack)

    //child process is created from copy of parent process image
    id = fork();

    //this would only be executed in parent process
    if (id != 0) {
        global++;
        //wait for child process to complete
        wait(NULL);
    }

    global++;
    //CHILD -> prints 6 to console
    //PARENT -> prints 7 to console
    printf( "main(): global = %d\n" , global) ; //CHILD = B (child data region); PARENT = D (parent data region)

    return(EXIT_SUCCESS);
}

void foo()
{
    static int staticInt = 3; // D (parent data region)
    int localInt = 1; // C (parent stack)

    printf("foo(): staticInt = %d \n" , staticInt) ; // D (parent data region)

    printf("foo(): localInt = %d \n" , localInt) ; // C (parent stack)
    staticInt++;
    localInt++;
}
