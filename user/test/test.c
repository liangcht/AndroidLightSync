#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include "light.h"
static void clean_events(int sig);
static int evt[3];

int main(void)
{
	struct event_requirements low;
	struct event_requirements medium;
	struct event_requirements high;

	low.req_intensity = 1000*100;
	low.frequency = 10;

	medium.req_intensity = 2000*100;
	medium.frequency  = 10;

	high.req_intensity = 10000*100;
	high.frequency = 5;

	int n = 1000;
	evt[0] = syscall(__NR_light_evt_create, &low);
	evt[1] = syscall(__NR_light_evt_create, &medium);
	evt[2] = syscall(__NR_light_evt_create, &high);

	pid_t pid[1000];
	signal(SIGALRM, clean_events);
	alarm(60);
	int i;

	for (i = 0; i < n; i++) {
		pid[i] = fork();
		int cat = i % 3;

		if (pid[i] < 0) {
			printf("%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		} else if (pid[i] == 0) {
			if (syscall(__NR_light_evt_wait, evt[cat]) == 0) {
				printf("%d detected a ", getpid());
				if (cat == 0)
					printf("low intensity event\n");
				else if (cat == 1)
					printf("medium intensity event\n");
				else
					printf("high intensity event\n");
			} else {
				printf("%s\n", strerror(errno));
			}
			exit(EXIT_SUCCESS);
		}
	}
	printf("finish creating children\n");
	int status;
	pid_t w_pid;
	while (1) {
		w_pid = wait(&status);
		if (w_pid < 0 && errno == ECHILD)
			break;
	}
	return 0;
}

void clean_events(int sig)
{
	printf("Begin destroying events...\n");
	syscall(__NR_light_evt_destroy, evt[0]);
	syscall(__NR_light_evt_destroy, evt[1]);
	syscall(__NR_light_evt_destroy, evt[2]);
}
