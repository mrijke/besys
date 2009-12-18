/*
 * TaskQueue.h
 *
 *  Created on: Nov 12, 2009
 *      Author: Daniel+Maarten
 */

#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

/* TaskQueue structure */
struct TaskQueue {
	/* array containing the first tasks of the different Priority Queues; [0] means priority 0, etc. */
	struct Task *tasks;
	/* The number of priorities this TaskQueue has */
	int NumPriorities;
};

void addTaskToQueue(struct TaskQueue*, struct Task*);
void printQueue(struct TaskQueue*);
struct TaskQueue* createQueue(int);
struct Task* popTaskQueue(struct TaskQueue*, int);
void clearTaskQueue(struct TaskQueue*);
int numTasksInQueue(struct TaskQueue*);
#endif /* TASKQUEUE_H_ */
