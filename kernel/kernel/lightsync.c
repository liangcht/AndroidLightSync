#include <linux/syscalls.h>
#include <linux/lightsync.h>
#include <linux/spinlock_types.h>
#include <linux/slab.h>

static struct light_intensity last_light_intensity;
static LIST_HEAD(events);
static DEFINE_SPINLOCK(events_lock);
static int id_count = 0;
static int buffer[WINDOW];
static int buffer_index = 0;

static void write_to_buffer(int intensity) {
	buffer[buffer_index++] = intensity;
	buffer_index %=	WINDOW;
}

static bool evaluate_condition(int intensity, int frequency) {
	int i;
	int counter;
	counter = 0;
	for(i = 0; i < WINDOW; i++) {
		if (buffer[i] >= intensity - NOISE)
			counter++;
		if (counter == frequency)
			return true;
	}
	return false;
}

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
	new_event->condition = false;
	spin_lock_init(&new_event->event_lock);
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
	struct light_event *cur;
	DEFINE_WAIT(wait);
	spin_lock(&events_lock);
	list_for_each_entry(cur, &events, event_list_head) {
		if (cur->event_id == event_id) {
			spin_unlock(&events_lock);
			spin_lock(&cur->event_lock);
			cur->ref_count++;
			spin_unlock(&cur->event_lock);
			while (1) {
				spin_lock(&cur->event_lock);
				prepare_to_wait(&cur->wq, 
						&wait, 
						TASK_UNINTERRUPTIBLE);
				if (cur->condition){
					spin_unlock(&cur->event_lock);
					break;
				}
				spin_unlock(&cur->event_lock);
				schedule();
			}
			finish_wait(&cur->wq, &wait);
			spin_lock(&cur->event_lock);
			cur->ref_count--;
			spin_unlock(&cur->event_lock);
			return 0;
		}
	}
	spin_unlock(&events_lock);
	return -EINVAL;
}

SYSCALL_DEFINE1(light_evt_signal, 
		struct light_intensity __user *, user_light_intensity)
{
	int intensity;
	struct light_event *cur;
	if (get_user(intensity, &user_light_intensity->cur_intensity) != 0) {
		return -EFAULT;
	}
	write_to_buffer(intensity);
	spin_lock(&events_lock);
	list_for_each_entry(cur, &events, event_list_head) {
		spin_lock(&cur->event_lock);
		if (evaluate_condition(cur->req_intensity, cur->frequency)) {
			cur->condition = true;
			wake_up_all(&cur->wq);
		} else {
			cur->condition = false;
		}
		spin_unlock(&cur->event_lock);
	}
	spin_unlock(&events_lock);
	return 0;
}
 

SYSCALL_DEFINE1(light_evt_destroy, int, event_id) 
{

}
