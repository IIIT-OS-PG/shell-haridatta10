#include"libh.h"
//tokenizing arguments for passing into execvp fun
void argtok(char *arr[],char *str)
{
arr[0]=strtok(str," ");
int i=0;
while(arr[i]!=NULL)
{
i++;
arr[i]=strtok(NULL," ");
}
}
//removes initial whitespaces 
void reminitspaces(char *str)
{
int temp=0;
while(str[temp]==' ')
temp++;
strcpy(str,str+temp);
}
//counts the args in given command
int argcount(char *str)
{
int temp,count=0,i;
for(i=0;str[i]!='\0';i++)
{
if(str[i]==' ')
{
count++;
temp=i+1;
while(str[temp]==' ')
temp++;
i=temp-1;
}
}
return count;
}
//to remove string quo
void remdouquo(char *str)
{
int i,j;
char s[1000];
strcpy(s,str);
i=0;
while(s[i]!='"' && s[i]!='\0')
i++;
if(s[i]=='\0')
return;
j=strlen(s);
while(s[j]!='"' && j!=0)
j--;
if(j!=i)
{
s[i]=s[j]=' ';
strcpy(str,s);
return;
}
else
return;
}

