#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<malloc.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
void overwio(char **,int);
void appenio(char **,int);
void inpuio(char **,int);
int ioredcheck(char *);
int argcount(char *);
void setprompt();
void reminitspaces(char *);
int chkpipe(char *);
void chngdir(char*,char*);
void argtok(char*[],char*);
void execpipe(int,char *);
void getpipestr(char **,char *);
void remdouquo(char *);
char *pwd,hname[100],*uname;
int flag,fd;
char prevdir[1000];
int main()
{
pid_t pid; int status,j,count,i,k,temp,h,errno,p,z,c;
char *str=(char *)malloc(sizeof(char)*100);
pwd=getenv("PWD");
gethostname(hname,100);
uname=getenv("USERNAME");strcpy(prevdir,pwd);
char t[100]="\0";
int y=getuid();
if(y>0)
printf("%s@%s:%s$ ",uname,hname,pwd);
while(fgets(str,100,stdin))
{
flag=0;
str[strlen(str)-1]='\0';
reminitspaces(str);
if(strcmp(str,"exit")==0)
exit(0);
if(strncmp(str,"cd",2)==0)
{
chngdir(str,pwd);
setprompt();
continue;
}
char *s=(char *)malloc(sizeof(char)*25);
if(strncmp(str,"echo",4)==0)
{
strcpy(s,str);
s=s+4;int f=0;
for(i=0;s[i]==' ';i++);
if(s[i]=='$')
{
if(strcmp("PATH",s+i+1)==0)
{
printf("%s",getenv("PATH"));f=1;
}
else if(strcmp("HOME",s+i+1)==0)
{
printf("%s",getenv("HOME"));f=1;
}
else if(strcmp("USER",s+i+1)==0)
{
printf("%s",getenv("USER"));f=1;
}
if(f==0)
printf("%s",s);
setprompt();
continue;
}
}
if(strncmp(str,"grep",4)==0)
remdouquo(str);
pid=fork();
if(pid<0)
{
printf("couldnt create a process");
exit(0);
}
if(pid==0)
{
k=strlen(str);
str[k]=' ';
str[k+1]='\0';
if(k==0)
{
exit(0);
}
c=chkpipe(str);
if(c>0)
{
int cmd=c+1;
execpipe(cmd,str);
exit(0);
}
//if no pipe..
count=argcount(str);
char *arr[count+1];
if(count==1)
{
for(i=0;str[i]!=' ';i++);
str[i]='\0';
arr[0]=str;
arr[1]=NULL;
}
char temp[100];
strcpy(temp,str);
if(count>1)
argtok(arr,str);
z=ioredcheck(temp);
if(z==-1)
{
printf("no such file..");
exit(0);
}
if(z==1)
overwio(arr,count);
else if(z==2)
appenio(arr,count);
else if(z==3)
inpuio(arr,count);
int j=execvp(str,arr);
if(j==-1)
perror(str);
exit(0);
}
if((i=waitpid(pid,&status,0))<0)
perror(str);
if(flag==1)
close(fd);
setprompt();
}
return 1;
}
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
//cd
void chngdir(char *str,char *pwd)
{
int errno,i;
str=str+2;
for(i=0;str[i]==' ';i++);
if(str[i]=='\0' || (str[i]=='~' && str[i+1]=='\0'))
{
strcpy(prevdir,pwd);
char *s;
chdir(s=getenv("HOME"));
strcpy(pwd,s);
return;
}
if(str[i]=='-' && str[i+1]=='\0')
{
char temp[100];
strcpy(temp,pwd);
chdir(prevdir);
strcpy(pwd,prevdir);
strcpy(prevdir,temp);
return;
}
for(int j=i;str[j]!='\0';j++)
{
if(str[j]==' ')
{
printf("too many arguments");
return;
}
}
if(str[i]=='/')
{
int c=0;
int x=chdir(str+i);
if(x==-1)
{
printf("no such directory");
return;
}
else
{
strcpy(prevdir,pwd);
strcpy(pwd,str+i);
return;
}
}
else
{
if(str[i]=='.' && str[i+1]=='\0')
{
strcpy(prevdir,pwd);
return;
}
if(strcmp(str+i,"..")==0 && strcmp(pwd,"/")==0)
{
strcpy(prevdir,pwd);
return;
}
char *temp;
if(str[i]=='~')
{
char *t=getenv("HOME");
str=str+i+1;
strcat(t,str);
strcpy(str,t);
temp=str;
}
else
{
temp=strdup(pwd);
if(strcmp(temp,"/")!=0)
strcat(temp,"/");
strcat(temp,str+i);
}
int x=chdir(temp);
if(x==-1)
printf("no such directory");
else
{
strcpy(prevdir,pwd);
int l;
if(strcmp(str+i,"..")==0)
{
l=strlen(pwd);
for(;pwd[l]!='/';l--);
pwd[l]='\0';
if(l==0)
{
pwd[l]='/';pwd[l+1]='\0';
}
return;
}
strcpy(pwd,temp);
}
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
//sets the prompt
void setprompt()
{
int y=getuid();
char s[100];
getcwd(s,100);
if(y==0)
{
char *uname2=getenv("USERNAME");
printf("\n%s@%s:%s# ",uname2,hname,s);
}
if(y>0)
{
printf("\n%s@%s:%s$ ",uname,hname,s);
}
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
//output redirect
void overwio(char **arr,int count)
{
int k;
flag=1;
fd=open(arr[count-1],O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
arr[count-2]=0;
k=dup2(fd,1);
if(k==-1)
printf("There is some pblm with d file..");
}
//appending 
void appenio(char **arr,int count)
{
int k;
flag=1;
fd=open(arr[count-1],O_RDWR|O_CREAT|O_APPEND,S_IRWXU);
arr[count-2]=0;
k=dup2(fd,1);
if(k==-1)
printf("error in appending to a file");
}
//input redirect
void inpuio(char **arr,int count)
{
arr[count-2]=arr[count-1];
arr[count-1]=0;
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
//executes the commands connected through pipes
void execpipe(int cmd,char *str)
{
int fd[2];pid_t pid;int errno,z,status,j,t;pid_t id;
int k=cmd+1,i,count;
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
for(j=0;c[j]!=' ';j++);
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
close(fd[1]);
dup2(fd[0],0);
close(fd[0]);
execvp(b,arr);
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







