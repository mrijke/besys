/*
 * Task.h
 *
 *  Created on: Nov 12, 2009
 *      Author: Daniel+Maarten
 */

#ifndef TASK_H_
#define TASK_H_

/* Task Structure */
struct Task {
	/*the ID the task has*/
	int id;
	/*The priority the task has*/
	int priority;
	/*The next task in the linked list*/
	struct Task *nextTask;
};


#endif /* TASK_H_ */
