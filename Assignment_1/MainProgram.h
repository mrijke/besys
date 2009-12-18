/*
 * MainProgram.h
 *
 * The main header file containing all other includes.
 *
 *  Created on: Nov 26, 2009
 *      Author: Daniel+Maarten
 */

#ifndef OPPUR_H_
#define OPPUR_H_
#define FALSE 0
#define TRUE (-1)

#include "Writer.h"
#include "Reader.h"
#include "TaskQueue.h"
#include "Task.h"
#include "small_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <getopt.h>

/* define the global mutex, to be used by the readers and writers */
pthread_mutex_t mutex;
/* The pseudo-boolean, used by the threads to check when they may exit */
int run;

void *writerRunner(void*);
void *readerRunner(void*);
void cleanUp(void);

#endif /* OPPUR_H_ */
