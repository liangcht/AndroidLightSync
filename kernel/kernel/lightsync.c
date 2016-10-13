#include <linux/syscalls.h>
#include <linux/lightsync.h>
#include <linux/spinlock_types.h>
#include <linux/slab.h>

static struct light_intensity last_light_intensity;
static LIST_HEAD(events);
static DEFINE_SPINLOCK(events_lock);
static int id_count = 0;

SYSCALL_DEFINE1(set_light_intensity, 
		struct light_intensity __user *, user_light_intensity)
{
	if (get_user(last_light_intensity.cur_intensity, 
		 &(user_light_intensity->cur_intensity)) != 0) {
		return -EFAULT;
	}
	printk("Current light intensity %d\n", 
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
	struct light_event *new_event;
	new_event = (struct light_event *)
		kmalloc(sizeof(struct light_event), GFP_KERNEL); 
	
	if (new_event == NULL) 
		return -ENOMEM;

	init_waitqueue_head(&new_event->wq);
	INIT_LIST_HEAD(&new_event->event_list_head);
	new_event->ref_count = 0;
	
	if (get_user(new_event->req_intensity, 
		     &intensity_params->req_intensity) != 0) 
		return -EFAULT;

	if (get_user(new_event->frequency, &intensity_params->frequency) != 0) 
		return -EFAULT;

	new_event->frequency = 
		(new_event->frequency > WINDOW) ? WINDOW : new_event->frequency;
	spin_lock(&events_lock);	
	new_event->event_id = id_count++;
	list_add(&new_event->event_list_head, &events);
	spin_unlock(&events_lock);
	return 0;
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
