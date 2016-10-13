#ifndef _LIGHTSYNC_H
#define _LIGHTSYNC_H 
/*
Use this wrapper to pass the intensity to your system call
*/
#define NOISE 10
#define WINDOW 20

struct light_intensity {
        int cur_intensity;
};
  
struct event_requirements {
        int req_intensity;
        int frequency;
};

struct light_event {
	int req_intensity;
	int frequency;
	int ref_count;
	int event_id;
	wait_queue_head_t wq;
	struct list_head event_list_head;
};
  
#endif

