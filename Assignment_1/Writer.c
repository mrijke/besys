/*
 * Writer.c
 *
 * A writer creates a task for the reader to read/consume.
 *
 *  Created on: Nov 19, 2009
 *      Author: Daniel+Maarten
 */
#include "MainProgram.h"

/*
 * Creates a Task with random priority, assigns it the task_id from the given Writer
 * and adds it to the TaskQueue specified in the Writer, then it sleeps
 */
void createTask(struct Writer *w){
	int sleepTime = 100;
	//get a random priority
	int prio = (int) ((double)rand() * (long)w->queue->NumPriorities / ((double) RAND_MAX+1));
	//define it, and allocate it some memory
	struct Task *t;
	t = malloc(sizeof(struct Task));
	//set its properties
	t->id = w->task_id;
	w->task_id = w->task_id +1;
	t->nextTask = NULL;
	t->priority = prio;

	//try to get the lock
	pthread_mutex_lock(&mutex);
	//add it to the queue
	addTaskToQueue(w->queue, t);
	//release lock
	pthread_mutex_unlock(&mutex);

	printf("Writer %02d: Created task with priority %d, id:%05d\n",w->id,prio,t->id);

	//and sleep
	if((sleepTime = (int)((prio/w->queue->NumPriorities) * 300)) < 100 )
		sleepTime = 100;
	usleep(sleepTime * 1000);

}

/*
 * Creates Tasks as long is it is allowed to run
*/
void createTasks(struct Writer *w){
	//while the threads are to run
	while(run == TRUE){
		//create a task
		createTask(w);
	}
}
