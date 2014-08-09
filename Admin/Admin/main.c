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
#include "aTasks.h"


int taskManager(char *cmd, char *extendedArgs, int *validLogin, int pid, char *password)
{
    if(strcmp(cmd,"logout") == 0)
    {
        if(logout(validLogin, pid,password))
        {
            *validLogin = 0;
            return 1;
        }
    }
    else if (strcmp(cmd,"addflight") == 0)
    {
        if(addFlight(extendedArgs,pid,password))
            return 1;
    }
    else if (strcmp(cmd,"shutdown") == 0)
    {
        if(shutdown(pid, password))
            return 1;
    }
    else if (strcmp(cmd,"changedate") == 0)
    {
        if(changeDate(extendedArgs,pid,password))
            return 1;
    }
    else if (strcmp(cmd,"addcity") == 0)
    {
        if(addCity(extendedArgs,pid,password))
            return 1;
        
    }
    else if (strcmp(cmd,"delcity") == 0)
    {
        if(delCity(extendedArgs,pid,password))
            return 1;
    }
    else if (strcmp(cmd,"adduser") == 0)
    {
        if(addUser(extendedArgs,pid,password))
            return 1;
    }
    else if (strcmp(cmd,"delflight") == 0)
    {
        if(delFlight(extendedArgs,pid,password))
            return 1;
    }
    else if (strcmp(cmd,"info") == 0)
    {
        if(getInfo(pid,password))
            return 1;
    }
    else if (strcmp(cmd,"getcities") == 0)
    {
        if(getCities(pid,password))
            return 1;
    }
    else if(strcmp(cmd,"seepast") == 0)
    {
        if(getPast(pid,password))
            return 1;
    }
    else if(strcmp(cmd,"getdate") == 0)
    {
        if(getDate(pid,password))
            return 1;
    } else {
        printf("Commands: shutdown | addcity | delcity | seepast | addflight | delflight | changedate | getdate | info | adduser | getcities\n");
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

int getInputFromAdmin(char *cmdsList[], int nArgs[], char *cmd, char *extendedArgs, char *args[], int *validLogin)
{
    char input[50];
    char input2[50];
    char *limpeza;
    char *inputCmd;
    int cmdPosition;
    if(*validLogin == 0)
        printf("Admin > login [password]:\n");
    else
        printf("Admin ~> ");
    
    if(fgets(input, sizeof(input), stdin) != NULL)
    {
        if ((limpeza = strchr(input, '\n')) != NULL)
            *limpeza = '\0';
    }
    strcpy(input2,input);
    inputCmd = strtok(input, " ");
    strcpy(cmd,inputCmd);
    
    if(((strcmp(inputCmd,"login") != 0) && (strcmp(inputCmd,"exit") != 0)) && *validLogin == 0)
    {
        printf("Admin > You are not logged. Valid commands for this state: [login] or [exit]\n");
        return 0;
    }
    
    if(strcmp(inputCmd,"login") == 0 && *validLogin == 1)
    {
        printf("Admin > Session already in use!\n");
        return 0;
    }

    /* Keeps the position of the command to get the number of arguments */
    cmdPosition=cmdValidation(cmdsList,inputCmd);
    
    if((cmdPosition != -1) && (nArgs[cmdPosition] != 0))
    {
        if(!fillArgs(input2,args,nArgs,cmdPosition,extendedArgs))
        {
            return 0;
        }
    }
    else if (cmdPosition == -1)
    {
        printf("Invalid command!\n");
        return 0;
    }
    return 1;
}

int checkInstanceRunning() {
    int serverIsRunning;
    int adminIsRunning;
    
    serverIsRunning = access(npServer, R_OK);
    adminIsRunning = access(npAdmin, R_OK);
    
    if(adminIsRunning != -1)
    {
        printf("Admin session already in use!\n");
        return 0;
    }
    
    if(serverIsRunning == -1)
    {
        printf("Central is not running at the moment!\n");
        return 0;
    }
    
    return 1;
}

int main(){
    /* List of commands */
    char *cmdsList[] = {"login","shutdown","addcity","delcity","seepast","addflight","delflight","changedate","getdate","info","adduser","exit","logout","help","getcities"};
    int nArgs[] = {1,0,1,1,0,4,1,1,0,0,2,0,0,0,0}; /* Number of arguments of each command */
    char cmd[15]; /*Command */
    char *args[5]; /*Arguments*/
    char extendedArgs[40];
    int validLogin = 0;
    int *pValidLogin;
    char password[passwordLen];
    int pid = getpid();
    int loginFE = 0;
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    
    if(!checkInstanceRunning())
        exit(0);

    if(mkfifo(npAdmin, 0644) != 0)
    {
        printf("Error: Creating Admin Pipe.\n");
        exit(0);
    }
    
    signal(SIGUSR1, shutAll);
    
    while(1)
	{
		if(getInputFromAdmin(cmdsList, nArgs, cmd, extendedArgs, args, &validLogin))
 		{
            if(strcmp(cmd,"exit") == 0)
            {
                if(validLogin == 1)
                {
                    logout(pValidLogin, pid, password);
                    shutAll();
                }
                else
                    shutAll();
            } else
                if(strcmp(cmd,"login") == 0)
                {
                    request->len = 0;
                    request->pid = pid;
                    request->userLevel = 1;
                    request->jobRef = 1;
                    strcpy(request->pw, args[0]);
                    
                    response = startCommunication(request,getpid());
                    
                    if(response->valid == 1)
                    {
                        validLogin = 1;
                        strcpy(password,request->pw);
                    }
                    else
                    {
                        printf("Invalid login!\n");
                        if(loginFE < maxloginAttemp)
                        {
                            sleep(5);
                            loginFE++;
                        } else {
                            printf("Too many login attempts! Shutting down.\n");
                            shutAll();
                        }
                        
                    }
                } else {
                    pValidLogin = &validLogin;
                    taskManager(cmd, extendedArgs, pValidLogin, pid, password);
                } 
        }
	}
}
