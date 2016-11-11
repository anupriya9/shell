#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void exportfunc(char str[100])
{ 



//char str[80] = "export x";
   const char s[3] = " \n=";
   char *token;
int pid;
char *name,*value;
   char *para[6]={NULL,NULL,NULL,NULL,NULL,NULL};
char *arr[3]={"printenv",NULL};
int i=0;

   /* get the first token */
   token = strtok(str, s);
   
   /* walk through other tokens */
   while( token != NULL ) 
   {
     // printf( "%s\n", token );
    para[i]=token;
      token = strtok(NULL, s);
	
i++;
   }

if(para[1]==NULL && para[2]==NULL)
{
pid=fork();
if(pid==0){
execvp(arr[0],arr);
}
}
if(para[1]!=NULL && para[2]==NULL)
{
para[2]=getenv(para[1]);
setenv(para[1],para[2],0);
//execvp(arr[0],arr);
}

if(para[1]!=NULL && para[2]!=NULL)
{
name=para[1];
value=para[2];
//printf("%s %s",name,value);
//printf("%d",setenv(name,value,1));
setenv(name,value,1);

}


}
