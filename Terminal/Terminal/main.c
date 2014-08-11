#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include "varConf.h"
#include "ioBound.h"
#include "communication.h"
#include "tTasks.h"


int taskManager(char *cmd, char *extendedArgs, int *validLogin, int pid)
{
    if(strcmp(cmd,"logout") == 0)
    {
        if(logout(validLogin, pid))
        {
            return 1;
        }
    }
    else if (strcmp(cmd,"changepasswod") == 0)
    {
        if(changePassword(extendedArgs, pid))
            return 1;
    }
    else if (strcmp(cmd,"list") == 0)
    {
        flightList(pid);
    }
    else if (strcmp(cmd,"search") == 0)
    {
        if(searchFlight(extendedArgs, pid))
            return 1;
    }
    else if (strcmp(cmd,"book") == 0)
    {
        if(bookFlight(extendedArgs, pid))
            return 1;
    }
    else if(strcmp(cmd,"cancel") == 0)
    {
        if(cancelFlight(extendedArgs, pid))
            return 1;
    } else {
        printf("Commands: logout | changepassword | list | search | book | cancel\n");
    };
    return 0;
}


int fillArgs(char *input, char *args[], int nArgs[], int idArg, char *extendedArgs)
{
    int i,j=0;
    char *temp;
    unsigned long len;
    temp = strtok(input, " ");
    temp = strtok(NULL, " ");
    
    
    for(i=0;i<nArgs[idArg] && temp != NULL;i++)
    {
        len = strlen(temp)+1;
        args[i] = malloc(len);
        strcpy(args[i], temp);
                
        if(i==0)
        {
            strcpy(extendedArgs, temp);
        }
        else
        {
            strcat(extendedArgs, ":");
            strcat(extendedArgs, temp);
        }
        temp = strtok(NULL, " ");
        j++;
    }
    
    if(j < nArgs[idArg])
    {
        printf("Missing arguments!\n");
        return 0;
    }
    
    return 1;
}

int cmdValidation(char *cmdsList[], char *cmd)
{
    /* Valid command - Return the command position */
    int i;
    
    for(i=0;i<numberOfCommands;i++)
    {
        if(strcmp(cmdsList[i],cmd) == 0)
        {
            return i;
        }
    }
    return -1;
}

int getInputFromTerminal(char *cmdsList[], int nArgs[], char *cmd, char *extendedArgs, char *args[], int *validLogin, char *username)
{
    char input[50];
    char input2[50];
    char *cleaning;
    char *inputCmd;
    int cmdPosition;
    if(*validLogin == 0)
        printf("Terminal > login [username] [password]:\n");
    else
        printf("%s ~> ",username);
    
    if(fgets(input, sizeof(input), stdin) != NULL)
    {
        if ((cleaning = strchr(input, '\n')) != NULL)
            *cleaning = '\0';
    }
    
    strcpy(input2,input);
    inputCmd = strtok(input, " ");
    strcpy(cmd,inputCmd);
    
    if(((strcmp(inputCmd,"login") != 0) && (strcmp(inputCmd,"exit") != 0)) && *validLogin == 0)
    {
        printf("Terminal > You are not logged. Valid commands for this state: [login] or [exit]\n");
        return 0;
    }
    
    if(strcmp(inputCmd,"login") == 0 && *validLogin == 1)
    {
        printf("%s > Session already in use!\n",username);
        return 0;
    }
    
    /* Keeps the position of the command to get the number of arguments */
    cmdPosition=cmdValidation(cmdsList,inputCmd);
    
    if((cmdPosition != -1) && (nArgs[cmdPosition] != 0))
    {
        if(!fillArgs(input2,args,nArgs,cmdPosition,extendedArgs))
            return 0;
    }
    else if (cmdPosition == -1)
    {
        printf("Invalid command!\n");
        return 0;
    }
    return 1;
}

int serversRunning() {
    int isRunning;
    isRunning = access(npServer, R_OK);
    if(isRunning == -1)
    {
        printf("Central is not running at the moment!\n");
        return 0;
    } else
        return 1;
}

int main(){
    /* List of commands */
    char *cmdsList[] = {"login","exit","logout","changepassword","list","search","book","cancel","help"};
    int nArgs[] = {2,0,0,2,0,2,2,2,0}; /* Number of arguments of each command */
    char cmd[15]; /*Command */
    char *args[2]; /*Arguments*/
    char extendedArgs[40];
    int validLogin = 0;
    int *pValidLogin;
    char username[usernameLen];
    int pid = getpid();
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    
    signal(SIGUSR1,shutdownTerm);
    
    if(!serversRunning)
        exit(0);
       
    while(1)
	{
		if(getInputFromTerminal(cmdsList, nArgs, cmd, extendedArgs, args, &validLogin, username))
 		{
            if(strcmp(cmd,"exit") == 0)
            {
                if(validLogin == 1)
                {
                    logout(pValidLogin, pid);
                    exit(0);
                }
                else
                    exit(0);
            } else {
                if(strcmp(cmd,"login") == 0)
                {
                    request->len = 0;
                    request->pid = pid;
                    request->userLevel = 2;
                    request->jobRef = 1;
                    strcpy(request->username, args[0]);
                    strcpy(request->pw, args[1]);
                    
                    response = startCommunication(request,getpid());
                    
                    if(response->valid == 1)
                    {
                        validLogin = 1;
                        strcpy(username,request->username);
                    }
                    else
                    {
                        printf("Invalid login!\n");
                    }
                } else {
                    pValidLogin = &validLogin;
                    taskManager(cmd, extendedArgs, pValidLogin, pid);
                }
            }
        }
	}
    
    free(request);
    request = NULL;
}
