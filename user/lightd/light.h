#ifndef _LIGHT_H
#define _LIGHT_H

/* Please use the same syscall number as indicated in the homework 3 instruction */

#define __NR_set_light_intensity 244
#define __NR_get_light_intensity 245
#define __NR_light_evt_create 246
#define __NR_light_evt_wait 247
#define __NR_light_evt_signal 248
#define __NR_light_evt_destroy 249

/*
Use this wrapper to pass the intensity to your system call
*/

struct light_intensity{
	int cur_intensity;
};

struct event_requirements {
	int req_intensity;
	int frequency;
};

#endif
