/*************************************
* Lab 2 Exercise 1
* Name: Daryl Tan
* Student Id: A0132821R
* Lab Group: B17
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for wait()

int main()
{
    int nChild;

    //Read the number of child
    scanf("%d", &nChild);

    //Spawn child processes
    int childIds[nChild + 1];
    for (int i = 1; i <= nChild; i++) {
        int result = fork();
        if (result == 0) {
            sleep(rand() % 10);
            printf("Child %d[%d]: Hello!\n", i, getpid());
            return 0;
        } else {
            childIds[i] = result;
        }
    }
    //Wait on child processes in order

    for (int i = 1; i <= nChild; i++) {
        int status;
        waitpid(childIds[i], &status, 0);

        printf("Parent: Child %d[%d] done\n", i, childIds[i]);
    }
    return 0;
}
