#include <linux/syscalls.h>
#include <linux/lightsync.h>

static struct light_intensity last_light_intensity;
SYSCALL_DEFINE1(set_light_intensity, 
		struct light_intensity __user *, user_light_intensity)
{
	if (get_user(last_light_intensity.cur_intensity, 
		 &(user_light_intensity->cur_intensity)) != 0) {
		return -EFAULT;
	}
	printk("Current light intensity %d", 
	       last_light_intensity.cur_intensity);
	return 0;
}

SYSCALL_DEFINE1(get_light_intensity, 
		struct light_intensity __user *, user_light_intensity)
{
	if (put_user(last_light_intensity.cur_intensity, 
		 &(user_light_intensity->cur_intensity)) != 0) {
		return -EFAULT;
	}
	return 0;
}

SYSCALL_DEFINE1(light_evt_create, 
		struct event_requirements __user *, intensity_params)
{

}

SYSCALL_DEFINE1(light_evt_wait, int, event_id)
{

}

SYSCALL_DEFINE1(light_evt_signal, 
		struct light_intensity __user *, user_light_intensity)
{

}
 

SYSCALL_DEFINE1(light_evt_destroy, int, event_id) 
{

}
