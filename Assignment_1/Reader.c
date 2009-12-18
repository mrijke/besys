/*
 * Reader.c
 *
 * A reader reads/consumes a task in the TaskQueue
 * 
 *  Created on: Nov 19, 2009
 *      Author: Daniel+Maarten
 */

#include "MainProgram.h"

/* Executes the next task from the TaskQueue specified in the given Reader
 * That is, it simply pops it, and prints information about it */
void executeNextTask(struct Reader *r){
	struct Task *temp;
	int sleepTime = 200;
	//Initial sleep time, so there's almost 100% sure something to read
	// Not doing this results in a lot of crying by the readers at the start of the program
	// It's random because otherwise they'd read in blocks of 4, kind of boring
	usleep(50000 + (int) ((double)rand() * (long)50000 / ((double) RAND_MAX+1)));

	//while the threads are to run
	while(run == TRUE){
		//get the highest priority
		int prio = r->tq->NumPriorities-1; //HIGHEST

		pthread_mutex_lock(&mutex);
		//try and fetch a task from the queue
		temp = popTaskQueue(r->tq,prio);
		//while you didn't fetch a task, try a lower priority
		while( temp == NULL && prio>0){
			prio--;
			temp = popTaskQueue(r->tq,prio);
		}
		pthread_mutex_unlock(&mutex);

		//if there was a task in the queue
		if(temp != NULL){
			//print the info
			printf("Reader %02i: Read    task with priority %d, id:%05d\n",r->id,temp->priority,temp->id);
			//calculate sleeping time, if sleepTime < 200, then sleepTime = 200
			if((sleepTime = (int)(((temp->priority+1)/r->tq->NumPriorities)* 600))<200);
				sleepTime = 200;
			usleep(sleepTime * 1000);
		//or print that the queue was empty
		}else{
			printf("There were no tasks in the queue!\n");
			//sleep for some time
			usleep(1000 * 300);
		}
		free(temp);
	}

}
