#include"libh.h"
//checks if there is a redirection op
int ioredcheck(char *str)
{
int i=0,k;
for(i=0;str[i]!='\0';i++)
{
if(str[i]=='>')
{
if(str[i+1]=='>')
{
k=2;break;
}
else
{
k=1;
break;
}
}
if(str[i]=='<')
{
k=3;
break;
}
}
if(str[i]=='\0')
k=0;
if(k==2)
{
i=0;
while(str[i]!='>')
i++;
i=i+2;
}
if(k==1)
{
i=0;
while(str[i]!='>')
i++;
i=i+1;
}
if(k==3)
{
i=0;
while(str[i]!='<')
i++;
i=i+1;
}
while(str[i]==' ')
i++;
for(;str[i]!='\0';i++)
{
if(str[i]==' ')
{
if(str[i+1]!='\0')
return -1;
else
return k;
}
}
}
//checking fr a pipe
int chkpipe(char *str)
{
int count=0;
for(int i=0;str[i]!='\0';i++)
if(str[i]=='|')
count++;
return count;
}

