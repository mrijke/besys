/*
 * Writer.h
 *
 *  Created on: Nov 19, 2009
 *      Author: Daniel+Maarten
 */

#ifndef WRITER_H_
#define WRITER_H_
#include <pthread.h>

/* Writer Structure */
struct Writer {
	/* The TaskQueue the Writer should write in to */
	struct TaskQueue *queue;
	/* The ID for the next Task being created */
	int task_id;
	/* The ID to identify this Writer with */
	int id;
};

void createTask(struct Writer*);
void createTasks(struct Writer*);

#endif /* WRITER_H_ */
