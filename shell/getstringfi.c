#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
char path[1000]="./history.txt";
char bang_command[1000];
char new_bang_command[1000];
int lineno;
int flagin=0;
int flagout=0;
void addtoHist(char *command);
char *getString(char *buffer, int length) {
	FILE *fp;
	int k=0,param;
	char *p,line[1000];
	char buff[1000],ch;
	char temp[1000];
	int flag1=0;
	char num[1000];
	int num1;
	temp[1000]='\0';
	int templen=0;
	int len;
	fgets(buffer, length, stdin);
	strcpy(buff,buffer);
	if(strstr(buffer,"<"))
			flagin=1;
		if(strstr(buffer,">"))
			flagout=1;

	param=strlen(buff);
	fp=fopen("history.txt","ab+");
	lineno=0;
	while(!feof(fp))
	{
		ch = fgetc(fp);
		  if(ch == '\n')
		   {
			  lineno++;
		}
	}
	fclose(fp);

	if(param>2 && buff[k]=='!' && buff[k+1]!='!')
	{
		if((buff[k+1]>='a' && buff[k+1]<='z') ||(buff[k+1]>='A' && buff[k+1]<='Z'))
		{
			flag1=1;
			//printf("%c\n",buff[k+1] );
			while(k<param)
			{
				temp[k]=buff[k+1];
				k++;
			}
			templen=strlen(temp);
			temp[templen-1]='\0';
			//printf("hello==%s==\n",temp);
		}
		else
		{
			if(param >3 && buff[k+1]=='-' && buff[k+2]!='\0')
			{
				flag1=2;
				int j1=0;
				while(buff[k+2+j1]!='\n')
				{
					num[j1]=buff[k+2+j1];
					j1++;
				}
				num1=atoi(num);
				/*if(lineno>num1)
					PRINT_ERROR;*/
				num1=lineno-num1;
				//printf("#@%d@\n",num1);

			}
			else if(param >2 && buff[1]!='\n')
			{
				int j2=0;
				flag1=2;
				while(buff[1+j2]!='\n')
				{
					num[j2]=buff[1+j2];
					j2++;
				}
				//printf("hi @@%s@@ #%d#", buff,num[j2-1]);
				num1=atoi(num);


			}

		}

		


	}
	len=strlen(buff);
	buff[len-1]='\0';


	//printf("temp ==%s==\n",temp );
	fp=fopen(path,"r");
	lineno=0;
	while ( fgets ( line, sizeof line, fp ) != NULL) 
	{  
		strcpy(bang_command,line);
		len=strlen(bang_command);
		bang_command[len]='\0';
		//printf("helo ! %s %s %d\n",bang_command ,temp,len);
		if((strncmp(bang_command,temp,templen))==0 && flag1==1)
		{
			//printf("helo ! %s\n",bang_command );
			//printf("inside");
		}
		if(num1==lineno)
		{
			strcpy(new_bang_command,bang_command);

		}
		if(lineno>100)
		{
			lineno=0;
		}
		lineno++;
		//printf("hi\n");
	}
	
	fclose(fp);
	if((strcmp(buff,"!!"))==0)
	{
		return bang_command;
	}
	else
	{
	if(flag1==1)
	{
		addtoHist(bang_command);
		flag1=0;
		printf("%s\n",bang_command );
		
		return bang_command;
	}
	if(flag1==2)
	{
		addtoHist(new_bang_command);
		flag1=0;
		printf("%s\n", new_bang_command);
		return new_bang_command;

	}
	addtoHist(buffer);
	if ((p = strchr(buffer, '\n')) != NULL) {
		*p = '\0';
	}
}
	return buffer;

}
void addtoHist(char *command)
{   
    int fd;
     if(!strstr(command,"!"))
        {   
            fd= open(path,O_WRONLY|O_CREAT|O_APPEND,0700);
            write(fd,command,strlen(command));
            lineno++;
            close(fd);
        }
        command[strlen(command)-1]='\0'; 
}