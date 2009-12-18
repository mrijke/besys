/*
 * TaskQueue.c
 *
 * The TaskQueue consists of multiple arrays of Tasks, ordered by their priority
 *  Created on: Nov 12, 2009
 *      Author: Daniel+Maarten
 */
#include "MainProgram.h"

/*
 * Adds the given Task to the given TaskQueue by adding it to the tail
 * of the same-priority linked list queue
 */
void addTaskToQueue(struct TaskQueue *tq, struct Task *new){
	int prio = new->priority;
	//get the fist task in the queue
	struct Task *temp = &(tq->tasks[prio]);

	//if the first task is NULL (it doesn't exist)
	if(tq->tasks[prio].nextTask == NULL){
		//just assign the giver task to the front of the queue
		tq->tasks[prio].nextTask = new;
	} else { //if there is a queue,
		//loop trough it
		while(temp->nextTask != NULL){
			//get the last one in the queue
			temp = temp->nextTask;
		}
		//and add the given task to the end of the queue
		temp->nextTask = new;
	}
}

/* Prints the given TaskQueue with a half-neat formatting */
void printQueue(struct TaskQueue *tq){
	int i;
	struct Task *temp;
	for(i=0;i < tq->NumPriorities;i=i+1 ){
		//get the first task in the queue (can be NULL)
		temp = tq->tasks[i].nextTask;
		printf("Priority: %d\n",i);

		//if there is at least 1 task, print them, else print that the queue is empty
		if(tq->tasks[i].nextTask != NULL)
		{
			while(temp!=NULL)
			{
				printf("\tID:%05d, priority:%d\n",temp->id,temp->priority);
				temp = temp->nextTask;
			}
		}
		else
			printf("\tThis one is empty\n");
	}
}

/* Creates a TaskQueue with the given number of priorities and returns the pointer to it */
struct TaskQueue* createQueue(int numPriorities){
	struct TaskQueue *result;
	int i;
	//allocate memory for the TaskQueue
	result = malloc(sizeof(struct TaskQueue));
	//Create the 'array' for the different priorities
	result->tasks =  malloc(numPriorities*sizeof(struct Task));
	for(i=0;i < numPriorities;i+=1){
		result->tasks[i].nextTask = NULL;
	}
	result->NumPriorities = numPriorities;

	return result;
}

/*
 * Fetches the first Task with the given priority of the given TaskQueue and returns this
 * It will return NULL if there's no Task with that priority
 */
struct Task* popTaskQueue(struct TaskQueue *tq, int priority){
	//If the queue belonging to the given priority isnt empty
	if(tq->tasks[priority].nextTask != NULL){
		struct Task *result;
		//get the next task in the queue
		result = tq->tasks[priority].nextTask;
		//make the second task the first
		tq->tasks[priority].nextTask = (tq->tasks[priority].nextTask)->nextTask;
		return result;
	}
	return NULL;
}

/* Clears the given TaskQueue, that is, it frees all the Tasks in the TaskQueue */
void clearTaskQueue(struct TaskQueue *tq){
	int i;
	struct Task *temp,*temp2;
	//All the priorities should be emptied
	for(i=0;i < tq->NumPriorities;i=i+1 ){
		temp = tq->tasks[i].nextTask;
		//free the tasks while we haven't reached the end
		while(temp!=NULL)
		{
			//Save temp in temp2 because otherwise we'd lose temp->nextTask
			temp2 = temp;
			temp = temp->nextTask;
			free(temp2);
		}
	}
}

/* Returns the total amount of tasks in a given queue */
int numTasksInQueue(struct TaskQueue *tq){
	int result = 0;
	int i;
	struct Task *temp;
	for(i=0;i < tq->NumPriorities;i++){
		//get the first task in the queue (can be NULL)
		temp = tq->tasks[i].nextTask;
		while(temp!=NULL)
			{
				result++;
				temp = temp->nextTask;
			}
	}
	return result;
}
