/*************************************
* Lab 2 Exercise 3
* Name: Daryl Tan
* Student Id: A0132821R
* Lab Group: B17
*************************************
*************************************
* Lab 2 Exercise 3
* Name: Leow Wen Bin
* Student Id: A0184415E
* Lab Group: B13
*************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

#define MAX_UNWAITED 10
#define MAX_PATH_LENGTH 20
#define MAX_COMMAND_LENGTH 20

char **split(char *input, char *delimiter, int maxTokenNum, int *readTokenNum)
//Assumptions:
//  - the input line is a string (i.e. with NULL character at the end)
//  - the delimiter is a string of possible delimiters, each delimiter is single chracter
//Behavior:
//  - Split up to and include maxTokenNum from the input string
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - Must use the freeTokenArray to free memory after use!
{
    char **tokenStrArr;
    char *tStart;   //start of token
    int i, strSize;

    //allocate token array, each element is a char*
    tokenStrArr = (char **) malloc(sizeof(char *) * maxTokenNum);

    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++) {
        tokenStrArr[i] = NULL;
    }

    //Look for next token as separated by the delimiter
    tStart = strtok(input, delimiter);

    i = 0;
    while (i < maxTokenNum && tStart) {

        strSize = strlen(tStart);

        //Allocate space for token string. +1 for null terminator
        tokenStrArr[i] = (char *) malloc(sizeof(char) * (strSize + 1));

        strcpy(tokenStrArr[i], tStart);

        i++;
        tStart = strtok(NULL, delimiter);
    }

    //return number of token read
    *readTokenNum = i;

    return tokenStrArr;
}

void freeTokenArray(char **strArr, int size) {
    int i;

    //Free each string first
    for (i = 0; i < size; i++) {
        if (strArr[i])
            free(strArr[i]);
        strArr[i] = NULL;
    }

    //Free the entire array
    free(strArr);

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}


int main() {
    char **cmdLineArgs;
    char path[MAX_PATH_LENGTH] = ".";  //default search path
    char userInput[121];

    int tokenNum;
    int result;
    int unwaitedIds[MAX_UNWAITED];
    int numUnwaited = 0;


    //read user input
    printf("YWIMC > ");

    //read up to 120 characters from user input
    fgets(userInput, 120, stdin);

    //split is a helper function to "tokenize" the input
    //see "stringTokenizer.c" program for more sample usage 
    cmdLineArgs = split(userInput, " \n", 7, &tokenNum);

    //At this point you have the user input split neatly into token in cmdLineArg[]

    while (strcmp(cmdLineArgs[0], "quit") != 0) {

        //Figure out which command the user want and implement below
        if (strcmp(cmdLineArgs[0], "showpath") == 0) {
            printf("%s\n", path);
        } else if (strcmp(cmdLineArgs[0], "setpath") == 0) {
            strcpy(path, cmdLineArgs[1]);
            printf("%s\n", path);
        } else if (strcmp(cmdLineArgs[0], "wait") == 0) {
            int pid = atoi(cmdLineArgs[1]);
            waitpid(pid, &result, 0);

            // remove from unwaited
            int found = 0;
            for (int i = 0; i < numUnwaited; i++) {
                if (unwaitedIds[i] == pid) {
                    found = 1;
                }
                if (found && i < MAX_UNWAITED - 1) {
                    // move next unwaited pids to cover up the currently waited one
                    // but if i === last index don't need shift
                    unwaitedIds[i] = unwaitedIds[i + 1];
                }
            }
            if (!found) {
                printf("%d not a valid child pid\n", pid);
            }
            numUnwaited--;
        } else if (strcmp(cmdLineArgs[0], "pc") == 0) {
            printf("Unwaited Child Processes:\n");
            for (int i = 0; i < numUnwaited; i++) {
                printf("%d\n", unwaitedIds[i]);
            }
        } else if (strcmp(cmdLineArgs[0], "result") == 0) {
            printf("%d\n", result >> 8);
        } else if (strcmp(cmdLineArgs[0], "showpath") == 0) {
            printf("%s\n", path);
        } else {
            int slashLength = 1;
            int nullLength = 1;
            int maxTotalCommandLength = MAX_PATH_LENGTH + slashLength + MAX_COMMAND_LENGTH + nullLength;
            char command[maxTotalCommandLength];
            strcpy(command, path);
            strcat(command, "/");
            strcat(command, cmdLineArgs[0]);

            struct stat fileStat;
            if (stat(command, &fileStat)) {
                printf("%s not found\n", command);
            }

            char *lastArg = cmdLineArgs[tokenNum - 1];
            int isBackground = strcmp(lastArg, "&") == 0;
            if (isBackground) {
                // replace & with NULL
                cmdLineArgs[tokenNum - 1] = NULL;
            }
            int childId = fork();
            if (childId == 0) {
                execv(command, cmdLineArgs);
            } else if (isBackground) {
                unwaitedIds[numUnwaited] = childId;
                numUnwaited++;
            } else {
                waitpid(childId, &result, 0);
            }
        }

        //Prepare for next round input

        //Clean up the token array as it is dynamically allocated
        freeTokenArray(cmdLineArgs, tokenNum);

        printf("YWIMC > ");
        fgets(userInput, 120, stdin);
        cmdLineArgs = split(userInput, " \n", 7, &tokenNum);
    }


    printf("Goodbye!\n");

    //Clean up the token array as it is dynamically allocated
    freeTokenArray(cmdLineArgs, tokenNum);


    return 0;

}
