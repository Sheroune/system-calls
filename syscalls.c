#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/wait.h>

int interrupts = 0;

char prqueue[] = "files/lpqres.txt";	// all information about print queue
char result[] = "result.txt";	// final list of files in the print queue 

int flag = 1;
int flag_q = 0;

// interrupt handler function
void interrupt_handler(){
    	interrupts++;
	flag = 0;
	
	if(flag_q == 0 && interrupts != 3)
		printf("\nType 'q' to exit the program or press Ctrl+C 3 times to show number of files in the print queue\n");
	
    	if (interrupts == 3){
		int pipe_descriptors[2];
		pipe(pipe_descriptors);
		
		if(fork() == 0){
			close(pipe_descriptors[0]);
			close(1);
			dup2(pipe_descriptors[1], 1);
			execl("/usr/bin/wc", "wc", "-l", result, NULL);
		}
		else{
			wait(0);
			
			int res = 0;
			char buffer[1000];
			read(pipe_descriptors[0], buffer, 1000);
			
			sscanf(buffer, "%d", &res);
			printf("\nNumber of files in the print queue: %d\n", res);
			exit(0);
		}
    }
}

int main(){
    	// interrupt handling
    	struct sigaction keyboard_interrupt;					
   	keyboard_interrupt.sa_handler = interrupt_handler;		
    	sigemptyset(&keyboard_interrupt.sa_mask); 
  	sigprocmask(0,0,&keyboard_interrupt.sa_mask);
    	keyboard_interrupt.sa_flags = 0;         
    	sigaction(SIGINT, &keyboard_interrupt, 0); 	
	
	char buffer[1000];
	char word[256];
	
	// file descriptors
	int fd_in, fd_out;	
	
	// making parallel process
	if(fork() == 0){
		fd_in = open(prqueue, O_RDONLY);
		dup2(fd_in, 0);
		close(fd_in);
		
		if(fd_in == -1){
			fprintf(stderr, "Can't read file %s", prqueue);
			exit(1);
		}
		
		else{
			fd_out = open(result, O_TRUNC|O_WRONLY|O_CREAT, 0777);	// S_IRWXU flag
			dup2(fd_out, 1);
			close(fd_out);
			
			if(fd_out == -1){
				fprintf(stderr, "Can't write to file %s", result);
				exit(1);
			}
			
			else{									
				fgets(buffer, sizeof(buffer), stdin);				// skipping the first line (headers)
				while(fgets(buffer, sizeof(buffer), stdin)){			
					sscanf(buffer, "%*s%*s%*s%s", word);			// getting names of files in the print queue
					printf("%s\n", word);					// writing this information to file			
				}
			}
		}
	}
	
	else{
		wait(0);
		
		int fd_result;
		fd_result = open(result, O_RDONLY);
		dup2(fd_result, 0);
		close(fd_result);
	
		if(fd_result == -1){
			fprintf(stderr, "Can't read file %s\n\n", result);
			exit(1);
		}
		
		// printing contents of files to the stdout
		while(fgets(word, sizeof(word), stdin)){
			word[strcspn(word, "\n")] = 0;
			printf("Content of file %s:\n", word);
			
			char dir[] = "files/";
			strcat(dir, word);
			int fd = open(dir, O_RDONLY);
			if(fd == -1){
				printf("Can't read file %s\n\n", word);
			}
			else{
				char c;
				while(read(fd, &c, 1) != 0){
					printf("%c", c);
				}
				printf("\n\n");
			}
		}
		close(0);
		
		int test = open("/dev/tty", 0);
		printf("Type 'q' to exit the program or press Ctrl+C 3 times to show number of files in the print queue\n");
		char t[2];
		while(1){
			if(flag == 0)
				flag = 1;
			else{
				read(test, &t, 2);
				t[1] = 0;
				
				if(t[0] == 'q'){
					flag_q = 1;
					break;
				}
			}
		}
		
	}
	return 0;
}
