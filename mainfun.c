#include"libh.h"
void overwio(char **,int);
void appenio(char **,int);
void inpuio(char **,int);
void chngdir(char*,char*);
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

