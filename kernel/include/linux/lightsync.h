#ifndef _LIGHTSYNC_H
#define _LIGHTSYNC_H 
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

