#include <linux/syscall.h>
#include <linux/lightsync.h>

SYSCALL_DEFINE1(set_light_intensity, struct light_intensity __user *, user_light_intensity)
{

}

SYSCALL_DEFINE1(get_light_intensity, struct light_intensity __user *, user_light_intensity)
{
	
}
SYSCALL_DEFINE1(light_evt_create, struct event_requirements __user *, intensity_params)
{

}

SYSCALL_DEFINE1(light_evt_wait, int, event_id)
{

}

SYSCALL_DEFINE1(light_evt_signal, struct light_intensity __user *, user_light_intensity)
{

}
 

SYSCALL_DEFINE1(light_evt_destroy, int, event_id) 
{

}
