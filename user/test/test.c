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

	low.req_intensity = 100*100;
	low.frequency = 10;

	medium.req_intensity = 1000*100;
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


		if (pid[i] < 0) {

		} else if (pid[i] == 0) {
			if (syscall(__NR_light_evt_wait, evt[i % 3]) == 0) {
				printf("%d wake up from event %d!\n", i, i % 3);
			} else {
				printf(strerror(errno));
			}
			exit(EXIT_SUCCESS);
		}
	}
	sleep(5);
	syscall(__NR_light_evt_destroy, evt[0]);	
	int status;
	do {
		wait(&status);
	} while ((!WIFEXITED(status) && !WIFSIGNALED(status)) || n-- > 0);
	return 0;
}
