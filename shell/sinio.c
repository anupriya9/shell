#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>



void singleiofunc(char buf[1000])
{
	//printf("%s\n",commandCount );
char str[1000];
int flagin=0,flagout=0;
int x,y,k;
int flag=0;
//int z=0;
char str1[100][100];
char *str2[100];
char filenamein[100];
char filenameout[100];
int in,out;
for(x=0,y=0;buf[x]!='\0'&&buf[x]!='\n';x++)
	{
if(buf[x]=='<')
{
str[y++]=' ';
str[y++]='<';
str[y++]=' ';
//y=y+3;
//x=x+1;
//continue;
}
else if(buf[x]=='>')
{
str[y++]=' ';
str[y++]='>';
str[y++]=' ';
//y=y+3;
//x=x+1;

}
else
{
str[y++]=buf[x];	
}

}
str[y]='\0';
printf("finl=%s=\n",str);
y=0;
for(x=0;str[x]!='\0';x++)
	{
//printf("for the char=%c",str[x]);


	if((str[x]== ' ' && x==0 && flag==0))
		{	
			flag=1;
			
			continue;
			
		}
		if((str[x]== ' ' && flag==0))
		{	
			flag=1;
			y++;k=0;
			continue;
			
		}
		if(str[x]== ' ' && flag==1)
		{
			continue;
		}
		else
		{//printf("coming in else for%cvalue of y=%d\n",str[x],y);
			flag=0;
			str1[y][k]=str[x];
			k++;
		}
	}


if(str[x-1]!=' ')
{
//printf("\nyessssssss");
y++;
}



	
	int word=y;
for(y=0;y<word;y++)
{

if((strcmp(str1[y],"<")) == 0){
				//printf("%s ", str1[y]);
				strcpy(filenamein, str1[y+1]); 
				strcpy(str1[y]," "); 
				strcpy(str1[y+1]," "); 
				printf("filenamein :%s-", filenamein); 
				flagin= 1;
			}
			else if(strcmp(str1[y],">") == 0){
				//printf("%s ", str1[y]);
				strcpy(filenameout, str1[y+1]); 
				strcpy(str1[y]," "); 
				strcpy(str1[y+1]," "); 
				printf("filenameout :%s ", filenameout); 
				flagout = 1;
			}


}


//printf("str--%s--y=%d\n",str1[y],y);

	for(y=0;y<word;y++)
	{
		if(strcmp(str1[y]," ")!=0){//printf("yesss");
		//printf("sub--%s--\n",str1[y]);
		str2[y]=str1[y];}
	}

str2[y]=NULL;
//printf("@%s@\n",str2[1] );



/*
for(y=0;y<word;y++)
{
//if(str2[y]!=NULL)
printf("str2--%s--\n",str2[y]);
}
*/
//printf("flagi=%d flagout=%d",flagin,flagout);
//printf("hi");
if(flagin==1 && flagout==0)
{
//printf("yaaaaa\n"); 
in = open(filenamein, O_RDONLY);
dup2(in, 0);
close(in);
}

else if(flagout==1 && flagin==0)
{
//printf("yaaaaa  out");   
out = open(filenameout, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
//printf("fd%d",in);
dup2(out, 1);
close(out);
}

else
{
//printf("yaaaaa both"); 
//printf("flagi=%d flagout=%d",flagin,flagout);
in = open(filenamein, O_RDONLY);
out = open(filenameout, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
dup2(in, 0);
dup2(out, 1);
close(in);
close(out);
}
if (execvp(*str2,str2)<0)
	{
	printf("eror");
	}

/*if(strcmp(str1[0],"history")==0)
{printhist(filename,buf);}
if(strcmp(str1[0],"echo")==0) {
	echofunc(buf);}
else
{
	
}*/
//exit(0);

}
