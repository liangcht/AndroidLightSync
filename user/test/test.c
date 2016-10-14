#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include "light.h"

int main (void)
{
	struct event_requirements low;
	struct event_requirements medium;
	struct event_requirements high;

	low.req_intensity = 100;
	low.frequency = 10;

	medium.req_intensity = 1000;
	medium.frequency  = 10;

	high.req_intensity = 10000;
	high.frequency = 10;

	int evt1;
	int evt2;
	int evt3;
	
	evt1 = syscall(__NR_light_evt_create, &low);
	evt2 = syscall(__NR_light_evt_create, &medium);
	evt3 = syscall(__NR_light_evt_create, &high);
	printf("%d,\n %d,\n %d", evt1, evt2, evt3);

	return 0;
}
