#include "shell.h"
//int lineno;
char *envp1[10000];
char *line;

//char *histpath=getcwd(NULL,MAX_BUFFER_SIZE);
int main(int argc,char *argv[],char *envp[])
{
	int it;
	setupSignalHandlers();
	//strcpy(*envp1,*envp);
	for (it = 0; envp1[it] != NULL; it++)
	{    
		strcpy(envp[it],envp[it]);
		 printf("\n%s", envp1[it]);
	}
	//printf("%s\n",histpath );
	readInput();
	return 0;
}

void readInput()
{
	char line[MAX_BUFFER_SIZE];
	
	do
	{
		printf("My_Shell:%s$ ",getcwd(NULL,MAX_BUFFER_SIZE));
		parseLine(getString(line,MAX_BUFFER_SIZE));
	}while(TRUE);
}



char *copyString(char *source) {
	char *dest = NULL;
	if (source) {
		dest = malloc(strlen(source));
		strcpy(dest, source);
	}
	return dest;
}

void executeCommands() {
	//printf("in exe ==%s==\n",commands[0]->command );
	if (currentStatus == STOPPED) {
		currentStatus = RUNNING;
		return;
	}
	pid_t pid, *childProcesses;
	int i, fds[(commandCount - 1) * 2];
	openPipes(commandCount ? &fds[0] : NULL);
	childProcesses = malloc(sizeof(pid_t) * commandCount);
	for (i = 0; i < commandCount; i++)
	 {
		if (!strcmp(commands[i]->command, "exit")) 
		{
			exit(EXIT_SUCCESS);
		}
		else if (!strcmp(commands[i]->parameters[0], "cd")) {
			if (chdir((!commands[i]->parameterCount) || !strcmp(commands[i]->parameters[1], "~") ? getenv("HOME") : commands[i]->parameters[1])) {
				PRINT_ERROR;
				return;
			}
		}
		else {//printf("hello %d %d\n",commands[i]->inputIndex,i);
			pid = fork();
			if (pid == -1) {
				PRINT_ERROR;
			}
			else if (pid){
				childProcesses[i] = pid;
			}
			else  {
				if (commands[i]->inputIndex != DEFAULT) 
				{
					dup2(fds[commands[i]->inputIndex], fileno(stdin));
				}
				if (commands[i]->outputIndex != DEFAULT) 
				{
					dup2(fds[commands[i]->outputIndex], fileno(stdout));
				}
				closePipes(commandCount ? &fds[0] : NULL);
				if(!strcmp((commands[i]->parameters[0]),"export"))
				{
					exportfunc(*commands[i]->parameters);
				}
				if(!strcmp((commands[i]->parameters[0]),"history"))
				{
					char *c;
					int y,j,k=1;
					if(commands[i]->parameterCount >0)
					{
						c=commands[i]->parameters[1];
						y=atoi(c);
						j=lineno-y;
					}
					else
						j=0;
						FILE *file = fopen (path, "r" );
			   			if ( file != NULL )
			  			{
			      			char line [1024]; 
			      			while ( fgets ( line, sizeof line, file ) != NULL) 
			      			{  
			      				//strcpy(bang_command,line);
			      				if(k>=j)
			         			{
			         				printf("%d %s",k,line );
			         				k++;
			         			}
			         			else
			         					k++;
			      			}

			      			fclose ( file );
		   					}
						}
						if(!strcmp((commands[i]->parameters[0]),"echo"))
						{
							if(commands[i]->parameterCount==0)
							{
								printf("\n");
							}
							else if(commands[i]->parameterCount>0)
							{
								int cnt=1;
								while(cnt<=commands[i]->parameterCount)
								{
									if(commands[i]->parameters[cnt][0]=='$')
									{
										//printf("jhj\n");

										char temp1[1000];
										//int 
										int len,l=0,m=0;
										len=strlen(commands[i]->parameters[cnt]);
										//printf("%s\n",commands[i]->parameters[cnt] );
										while(l<len)
										{
											if(commands[i]->parameters[cnt][m]=='$')
												m++;
											else
											{
												temp1[l]=commands[i]->parameters[cnt][m];
												l++;
												m++;
											}
											

										}
										temp1[l-1]='\0';
										//printf("%s",temp1 );
										if(getenv(temp1)!=NULL)
										{
											printf("%s",getenv(temp1) );
										}
										cnt++;
										continue;
										

									}
									if(commands[i]->parameters[cnt][0]=='"')
									{

										char temp[1000];
										//int 
										int len,l=0,m=0;
										len=strlen(commands[i]->parameters[cnt]);
										//printf("%s\n",commands[i]->parameters[cnt] );
										while(l<len)
										{
											if(commands[i]->parameters[cnt][m]=='"')
												m++;
											else
											{
												temp[l]=commands[i]->parameters[cnt][m];
												l++;
												m++;
											}
											

										}
										temp[l-1]='\0';
										strcpy(commands[i]->parameters[cnt],temp);
										
									}
									printf("%s ",commands[i]->parameters[cnt]);
									cnt++;
								}
									printf("\n");
							}

						}
						//if(strcmp((commands[i]->parameters[0]),"echo")!=0)
						else
						execvp(commands[i]->parameters[0], commands[i]->parameters); 	
						
						//	PRINT_ERROR;
							exit(EXIT_FAILURE);
					
					
				
			}
		}
	}
	closePipes(commandCount ? &fds[0] : NULL);
	waitChildProcesses(&childProcesses[0]);
	free(childProcesses);
}

void freeCommands() {
	int i, j;
	for (i = 0; i < commandCount; i++) {
		for (j = 0; j < commands[i]->parameterCount+1; j++) {
			free(commands[i]->parameters[j]);
		}
		free(commands[i]->command);
		free(commands[i]);
	}
	free(commands);
}



void openPipes(int *fds) {
	if (fds) {
		int i;
		for (i = 0; i < (commandCount - 1) * 2; i += 2) {
			pipe(fds + i);
		}
	}
}
void closePipes(int *fds) {
	if (fds) {
		int i;
		for (i = 0; i < (commandCount - 1) * 2; i++) {
			close(fds[i]);
		}
	}
}

int parseCommandParameters(char *command, char **parsedParameters) {
    int count = 0;
    char *process = command;
    process = strtok(process, " \t\"");
    while (process) {
   		parsedParameters[count++] = copyString(process);
        process = strtok(NULL, " \t");
    }
    parsedParameters[count] = NULL;
    return count-1;
}

int parseCommands(char *line, char **commandList) {
	int count = 0;
    char *strCommand = line;
    strCommand = strtok(strCommand, "|");
     //printf("%s\n",strCommand );

    while (strCommand) {
    	commandList[count++] = strCommand;
    	strCommand = strtok(NULL, "|");
    }

    commandList[count] = NULL;
    return count;
}

void parseLine(char *line) {
	
	commandInfo *currentCommand;
	char *commandList[MAX_COMMAND_COUNT];
	if (feof(stdin)) {
		printf("exit\n");
		exit(EXIT_SUCCESS);
	}
	else {
		commandCount = parseCommands(line, commandList);
		//printf("%d\n",commandCount );
   		 if((flagin==1 || flagout==1) && commandCount==1)
    	{
    		//printf("==%s==\n",line );
    		//printf("in\n");
    		singleiofunc(line);
    		//readIO(line,commandCount);
    		//flagin=0;
    		
    		//printf("back\n");
    	}
    	/*else if((flagin==1 || flagout==1) && commandCount>1)
    	{
    		printf("out\n");
    		pipes_and_redirection(line);
    		//piping(line,"hist");	
    		//readIO(line,commandCount);
    		//flagout=0;
    		printf("back\n");
    	}*/
    	else
    	{
		commands = malloc(sizeof(commandInfo)*commandCount);
		int i;
		for (i = 0; i < commandCount; i++) {
			currentCommand = malloc(sizeof(commandInfo));
			currentCommand->command = copyString(commandList[i]);
			char *commandCopy = copyString(currentCommand->command);
			currentCommand->parameterCount = parseCommandParameters(commandCopy, currentCommand->parameters);
			currentCommand->inputIndex  = i == 0 ? DEFAULT : (2 * i - 2);
			currentCommand->outputIndex = i == commandCount - 1 ? DEFAULT : (2 * i + 1);
			free(commandCopy);
			commands[i] = currentCommand;
		}
		//printf("back %s\n",currentCommand->command);
		if(flagin==0 && flagout==0)
		   executeCommands();
		//printCommands();
		freeCommands();
	}
	}
}
void setupSignalHandlers() {
	struct sigaction signalActionSIGINT_SIGSTP;
	memset (&signalActionSIGINT_SIGSTP, 0, sizeof (signalActionSIGINT_SIGSTP));
	signalActionSIGINT_SIGSTP.sa_handler = &signalActionSIGINT_SIGSTPHandler;
	sigaction(SIGINT,  &signalActionSIGINT_SIGSTP, NULL);  // Ctrl + C signal
	sigaction(SIGTSTP, &signalActionSIGINT_SIGSTP, NULL);  // Ctrl + Z signal
}

void signalActionSIGINT_SIGSTPHandler(int signalNumber) {
	currentStatus = STOPPED;
	printf("\n");
}

void waitChildProcesses(pid_t *childProcesses) {
	int i;
	if (childProcesses) {
		for (i = 0; i < commandCount; i++) 
		{
			int status;
			pid_t waitReturnedValue = waitpid(childProcesses[i], &status, 0);
			if ((waitReturnedValue == -1) && (errno != ECHILD) && (!WIFEXITED(status) || WEXITSTATUS(status) != 0)) {
				fprintf(stderr, "\n-> Interrupted process killed (pid = %d).\n", childProcesses[i]);
				kill(childProcesses[i], SIGTERM);
			}
		}
		currentStatus = RUNNING;
	}
}

