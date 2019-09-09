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
int ioredcheck(char *);
int argcount(char *);
void setprompt();
void reminitspaces(char *);
int chkpipe(char *);
void argtok(char*[],char*);
void execpipe(int,char *);
void getpipestr(char **,char *);
void remdouquo(char *);

