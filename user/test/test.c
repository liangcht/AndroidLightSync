#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "light.h"

int main (void)
{
	struct event_requirements low;
	struct event_requirements medium;
	struct event_requirements high;

	low.req_intensity = 1000*100;
	low.frequency = 10;

	medium.req_intensity = 5000*100;
	medium.frequency  = 10;

	high.req_intensity = 10000*100;
	high.frequency = 10;

	int n = 30;
	int evt[3];	
	evt[0] = syscall(__NR_light_evt_create, &low);
	evt[1] = syscall(__NR_light_evt_create, &medium);
	evt[2] = syscall(__NR_light_evt_create, &high);
	//printf("%d,\n %d,\n %d\n", evt[0], evt[1], evt3);
	
	pid_t pid[30];
	int i;
	for (i = 0; i < n; i++) {
		
		pid[i] = fork();
		int cat = i % 3;
	
		if (pid[i] < 0) {
			printf("line 41: %s\n", strerror(errno));
		} else if (pid[i] == 0) {
			if (syscall(__NR_light_evt_wait, evt[cat]) == 0) {
				if (cat == 0) 
					printf("%d detected a low intensity event\n",
					getpid());
				else if (cat == 1) 
					printf("%d detected a medium intensity event\n", 
					getpid());
				else printf("%d detected a high intensity event\n",
					getpid());
			} else {
				printf("line 53: %s\n", strerror(errno));
			}
			exit(EXIT_SUCCESS);
		}
	}
	sleep(5);
	syscall(__NR_light_evt_destroy, evt[0]);
	printf("finish creating children\n");
	int status;
	do {
		wait(&status);
		printf("%d\n", n);
	} while ((!WIFEXITED(status) && !WIFSIGNALED(status)) || --n > 0);
	printf("finish while\n");
	return 0;
}
