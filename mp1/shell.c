/** @file shell.c */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "log.h"
#include <time.h>
#include <sys/time.h>
extern char **environ;
log_t Log;


/**
 * Starting point for shell.
 */
int main(int argc, char ** argv) {
	
	ssize_t line_ct;
	size_t line_size = 0;
	char *line = NULL;
	char *temp = NULL;
	char *CEP = "Command executed by pid=%d\n";	
	
   int timesignal=0;

   struct timeval starttime;
  	struct timeval endtime;

	
	FILE *f;
	signal(SIGINT, SIG_IGN);
	log_init(&Log);	
	if (argc==2 && !strcmp(argv[1],"-h")) 
		printf("Shell by weishi3\n");
		
		if (argc==2 && !strcmp(argv[1],"-t")) {
		timesignal=1;
		}
		
	if (argc==2 && !strcmp(argv[1],"=")) {
		int j=0;
	while (environ[j])
	puts(environ[j++]);
		}
	
if (argc==3 && !strcmp(argv[1],"-f")) {
		f=fopen(argv[2],"r");
if (f==NULL) return -1;
char boof[4096];//?
while (fgets(boof,sizeof(boof),f)){
system(boof);

}
		
	
		}
	
	while(1) {
	
		int andflag=0;
		pid_t pid = getpid();

		char *CWD = getcwd(0,0);

		printf("(pid=%d)%s$ ", getpid(), CWD);
		fflush(stdout);//
		 line_ct = getline(&line, &line_size, stdin);
		
		line[--line_ct] = '\0';

		if (line_ct <= 0) continue;
		
		
		
		
		if ( !strcmp(line, "!#")) {			//Prints all commands stored in reverse
			int i;					//chronological order
			for (i = 0; i <=Log.end-1; i++) {
				printf("%s\n", Log.the_log[i]);
			}	  					
			printf(CEP, pid);//i.e. The shell should be ready to take the next command before the given command has finished running
			continue;

	
		} else if (!strncmp(line, "!", 1)) {		//Searches log for command, executes command if	
								//exists
			temp = log_search(&Log, line+1);
			
			if (temp != NULL) {
				printf("%s matches %s\n", line+1, temp);
				temp = strdup(temp);
				line = temp;
 				//log_push(&Log, temp);
				//printf(CEP, pid);
			} else {
				printf("No Match\n");
				free(CWD);
				continue;
			}

		}
		 
		if (strcmp(line, "exit") == 0) {
			printf(CEP, pid);
			log_push(&Log, line);
			free(line);
			free(CWD);
			
			log_destroy(&Log);
			break;


		} else if (!strncmp(line, "cd ", 3)) {
			
			if (chdir(line + 3) == -1) {
				printf("%s: No such file or directory\n", line + 3);
			}
			printf(CEP, pid);
			log_push(&Log, line);
			free(CWD);
			continue;

		} else {
			log_push(&Log, line);
			
			
		int k=0;
		while (line[k]!='\0') k++;
		if (line[k-1]=='&') {
		andflag=1;
		line[k-1]='\0';
		}
			gettimeofday(&starttime, 0);
			pid_t child_pid = fork();
			/*char *command[500];
			char *t;
			
			int i;
			for (i = 0; i < 500; i++) {
				command[i] = NULL;
			}*/
			
			if (child_pid == 0) {

				pid_t cpid = getpid();
				printf(CEP, cpid);

		/*		char delimiters[] = " !";	
				int j = 0;

				char *line_temp = line;
				
				while ((t = strsep(&line_temp, delimiters))) {
					command[j] = t;
					j++;
				}

				
				execvp(command[0], command);
				printf("%s: not found\n", line);
				exit(1);*/
				
				char* seg=strtok(line," ");
				int i=0;
				char* temp[1024];
				while(seg!=NULL){
					char* arg=strdup(seg);
					temp[i]=arg;
					i++;
					seg=strtok(NULL," ");				
				}
				char* argv[i];
				argv[i]=NULL;
				for(;i>=0;i--)
					argv[i-1]=temp[i-1];
				if(execvp(argv[0], argv)==-1)
					printf("%s: not found\n", line);
		
				exit(0);			

			} else {
				if (!andflag){
					waitpid(child_pid, NULL, WUNTRACED);
					
					}
		
			
				
			}
			gettimeofday(&endtime, 0);
			long elapsed = (endtime.tv_sec-starttime.tv_sec)*1000000 + endtime.tv_usec-starttime.tv_usec;
			if (timesignal)
				printf("Execution took %ld microseconds.\n", elapsed) ;
			
			
		}

		free(CWD);	
		//free(line);	

	}
	//free(line);
	//log_destroy(&Log);
	return 0;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
/*
extern char **environ;
log_t Log;


int main(int argc, char ** argv) {
	
	ssize_t line_ct;
	size_t line_size = 0;
	char *line = NULL;
	char *temp = NULL;
	char *CEP = "Command executed by pid=%d\n";	
	int andflag=0;
   int timesignal=0;

   struct timeval starttime;
  	struct timeval endtime;

	
	FILE *f;
	signal(SIGINT, SIG_IGN);
	log_init(&Log);	
	if (argc==2 && !strcmp(argv[1],"-h")) 
		printf("Shell by weishi3\n");
		
		if (argc==2 && !strcmp(argv[1],"-t")) {
		timesignal=1;
		}
		
	if (argc==2 && !strcmp(argv[1],"=")) {
		int j=0;
	while (environ[j])
	puts(environ[j++]);
		}
	
if (argc==3 && !strcmp(argv[1],"-f")) {
		f=fopen(argv[2],"r");
if (f==NULL) return -1;
char boof[4096];//?
while (fgets(boof,sizeof(boof),f)){
system(boof);

}
		
	
		}
	
	while(1) {
	
		
		pid_t pid = getpid();

		char *CWD = getcwd(0,0);

		printf("(pid=%d)%s$ ", getpid(), CWD);
		fflush(stdout);//
		 line_ct = getline(&line, &line_size, stdin);
		
		line[--line_ct] = '\0';

		if (line_ct <= 0) continue;
		
		int k=0;
		while (line[k]!='\0') k++;
		if (line[k-1]=='&') andflag=1;
		
		
		
		if ( !strcmp(line, "!#")) {			//Prints all commands stored in reverse
			int i;					//chronological order
			for (i = Log.end-1; i >= 0; i--) {
				printf("%s\n", Log.the_log[i]);
			}	  					
			printf(CEP, pid);//i.e. The shell should be ready to take the next command before the given command has finished running
			continue;

	
		} else if (!strncmp(line, "!", 1)) {		//Searches log for command, executes command if	
								//exists
			temp = log_search(&Log, line+1);
			
			if (temp != NULL) {
				printf("%s matches %s\n", line+1, temp);
				temp = strdup(temp);
				line = temp;
 				//log_push(&Log, temp);
				//printf(CEP, pid);
			} else {
				printf("No Match\n");
				free(CWD);
				continue;
			}

		}
		 
		if (strcmp(line, "exit") == 0) {
			printf(CEP, pid);
			log_push(&Log, line);
			free(line);
			free(CWD);
			
			log_destroy(&Log);
			break;


		} else if (!strncmp(line, "cd ", 3)) {
			
			if (chdir(line + 3) == -1) {
				printf("%s: No such file or directory\n", line + 3);
			}
			printf(CEP, pid);
			log_push(&Log, line);
			free(CWD);
			continue;

		} else {
			log_push(&Log, line);
			gettimeofday(&starttime, 0);
			pid_t child_pid = fork();
			char *command[500];
			char *t;
			
			int i;
			for (i = 0; i < 500; i++) {
				command[i] = NULL;
			}
			
			if (child_pid == 0) {

				pid_t cpid = getpid();
				printf(CEP, cpid);

				char delimiters[] = " !";	
				int j = 0;

				char *line_temp = line;
				
				while ((t = strsep(&line_temp, delimiters))) {
					command[j] = t;
					j++;
				}

				
				execvp(command[0], command);
				printf("%s: not found\n", line);
				exit(1);

			} else {
				//if (!andflag){
					waitpid(child_pid, NULL, WUNTRACED);//}
		

				
			}
			gettimeofday(&endtime, 0);
			long elapsed = (endtime.tv_sec-starttime.tv_sec)*1000000 + endtime.tv_usec-starttime.tv_usec;
			printf("Execution took %ld microseconds.\n", elapsed) ;
			
			
		}

		free(CWD);	
			

	}
	return 0;
	}
	*/
