/*
 * Reader.h
 *
 *  Created on: Nov 19, 2009
 *      Author: Daniel+Maarten
 */

#ifndef READER_H_
#define READER_H_
#include <pthread.h>

/* Reader structure*/
struct Reader {
	/* The TaskQueue is should read of*/
	struct TaskQueue *tq;
	/*The ID the Readers has, is used when printing to stdout*/
	int id;
};

void executeNextTask(struct Reader *);

#endif /* READER_H_ */
