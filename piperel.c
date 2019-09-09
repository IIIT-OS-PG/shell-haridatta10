#include"libh.h"
//executes the commands connected through pipes
void execpipe(int cmd,char *str)
{
int flag=0,b;
b=ioredcheck(str);
if(b==-1)
{
printf("giv a prop name to a file ..\n");
return;
}
if(b==1)
flag=1;
else if(b==2)
flag=2;
else if(b==3)
{
printf("meaning less comm\n");
return;
}
int fd[2];pid_t pid;int errno,z,status,j,u,t,fdl;pid_t id;
int k=cmd+1,i,count;
char fname[1000];
if(flag==1 || flag==2)
{
for(i=0;str[i]!='>';i++);
if(str[i+1]=='>')
{
str[i]='\0';
u=i+2;
while(str[u]==' ')
u++;
strcpy(fname,str+u);
int x=strlen(fname);
fname[x-1]='\0';
}
else
{
str[i]='\0';
u=i+1;
while(str[u]==' ')
u++;
strcpy(fname,str+u);
int x=strlen(fname);
fname[x-1]='\0';
}
}
char *cmdv[k];
getpipestr(cmdv,str);
for(i=0;i<=k-2;i++)
reminitspaces(cmdv[i]);
char c[1000];
for(i=0;i<=k-2;i++)
{
strcpy(c,cmdv[i]);
if(strncmp(c,"grep",4)==0)
remdouquo(c);
count=argcount(cmdv[i]);
char *arr[count+1];
char b[100];
if(count==1)
{
for(j=0;c[j]!=' ';j++);
c[j]='\0';
strcpy(b,c);
arr[0]=c;
arr[1]=NULL;
}
if(count>1)
{
strcpy(b,cmdv[i]);
argtok(arr,b);
}
z=pipe(fd);
if(z<0)
{
strerror(errno);
exit(0);
}
id=fork();
if(id<0)
printf("unable to create a child process..");
if(id>0)
{
z=waitpid(id,&status,0);
if(z<0)
{
strerror(errno);
exit(0);
}
close(fd[1]);
dup2(fd[0],0);
close(fd[0]);
if(i==k-3)
{
i++;
strcpy(c,cmdv[i]);
if(strncmp(c,"grep",4)==0)
remdouquo(c);
count=argcount(cmdv[i]);
char *arr[count+1];
char b[100];
if(count==1)
{
for(j=0;c[j]!=' '&& c[j]!='\0';j++);
c[j]='\0';
strcpy(b,c);
arr[0]=c;
arr[1]=NULL;
}
if(count>1)
{
strcpy(b,c);
argtok(arr,b);
}
if(flag==0)
{
int t=execvp(b,arr);
if(t<-1)
strerror(errno);
}
else if(flag==1)
{
fdl=open(fname,O_RDWR|O_CREAT|O_TRUNC,00777);
dup2(fdl,1);
int t=execvp(b,arr);
if(t<-1)
strerror(errno);
}
else if(flag==2)
{
fdl=open(fname,O_RDWR|O_CREAT|O_APPEND,00777);
close(fd[1]);
dup2(fd[0],0);
close(fd[0]);
dup2(fdl,1);
int t=execvp(b,arr);
if(t<-1)
strerror(errno);
}
}
continue;
}
else
{
close(fd[0]);
dup2(fd[1],1);
close(fd[1]);
int h=execvp(b,arr);
if(h<0)
strerror(errno);
exit(status);
}
}
}

//makes an array of pointers to each individual command
void getpipestr(char **cmdv,char *str)
{
int i=0;
cmdv[i]=strtok(str,"|");
while(cmdv[i]!=NULL)
{
i++;
cmdv[i]=strtok(NULL,"|");
}
}

