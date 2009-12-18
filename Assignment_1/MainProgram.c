/*
 * MainProgram.c
 *
 * The Main Program; USAGE ./assignment1 [-w|--writers #] [-r|--readers #] [-p|--priorities #]
 * 
 *  Created on: Nov 19, 2009
 *      Author: Daniel+Maarten
 */

#include "MainProgram.h"

struct TaskQueue *tq;
pthread_mutex_t mutex;
static int verbose = 0;

/* entry point to the program */
int main(int argc,char **argv){

	int numReaders,numWriters,numPriorities,i,opt;
    int option_index = 0;

	numReaders = 3;
	numWriters = 2;
	numPriorities = 3;
	static struct option long_options[] =
	{
		   {"verbose",    no_argument      , 0, 'v'},
		   {"help",       no_argument      , 0, 'h'},
		   {"writers",    required_argument, 0, 'w'},
		   {"readers",    required_argument, 0, 'r'},
		   {"priorities", required_argument, 0, 'p'},
		   {0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "vhw:r:p:", long_options, &option_index)) != -1) {
	   switch (opt) {
       case 'v':
         verbose = 1;
         break;
	   case 'r':
		   if(sizeof(optarg) >= 3) { //check if argument isn't too big so it will fit in an int variable
			   fprintf(stderr, "Argument is too big.\n");
			   exit(1);
		   }
		   if(is_numeric(optarg)==0){
			   fprintf(stderr,"The given value is not an integer! Please refer to -h or --help for information.\n");
			   exit(1);
		   }
		   numReaders = atoi(optarg);
		   break;
	   case 'w':
            if(sizeof(optarg) >= 3) {//check if argument isn't too big so it will fit in an int variable
               fprintf(stderr, "Argument is too big.\n");
			   exit(1);
            }
		   if(is_numeric(optarg)==0){
		   	   fprintf(stderr,"The given value is not an integer! Please refer to -h or --help for information.\n");
               exit(1);
		   }
		   numWriters = atoi(optarg);
		   break;
	   case 'p':
            if(sizeof(optarg) >= 3) {//check if argument isn't too big so it will fit in an int variable
                fprintf(stderr, "Argument is too big.");
            }
		   if(is_numeric(optarg)==0){
		   	   fprintf(stderr,"The given value is not an integer! Please refer to -h or --help for information.\n");
		   	   exit(1);
		   }
		   numPriorities = atoi(optarg);
		   break;
	   case 'h':
		   printf("\nUSAGE\n    %s [-w|--writers #] [-r|--readers #] [-p|--priorities #]\n",argv[0]);
		   printf("\n\t-w # | --writers #\t Creates # writers. Default: 2");
		   printf("\n\t-r # | --readers #\t Creates # readers. Default: 3");
		   printf("\n\t-p # | --priorities #\t Creates # priorities, needs to be at least 1. Default: 3\n");
		   printf("\n\t-v   | --verbose \t Print more output\n");
		   printf("\nNote 1: The total number of threads is 50, i.e. 1 + 'number of writer' + 'number of readers' should be less than or equal to 50.\n");
		   printf("\nNote 2: The numbers specified should be positive integers.\n");
		   exit(0);
	   case '?': //invalid option
		   printf("\nUSAGE\n    %s [-w|--writers #] [-r|--readers #] [-p|--priorities #]\n",argv[0]);
		   printf("\n\t-w # | --writers #\t Creates # writers. Default: 2");
		   printf("\n\t-r # | --readers #\t Creates # readers. Default: 3");
		   printf("\n\t-p # | --priorities #\t Creates # priorities, needs to be at least 1. Default: 3\n");
		   printf("\n\t-v   | --verbose \t Print more output\n");
		   printf("\nNote 1: The total number of threads is 50, i.e. 1 + 'number of writer' + 'number of readers' should be less than or equal to 50.\n");
		   printf("\nNote 2: The numbers specified should be positive integers.\n");
		   exit(1);
	   }
	}


	if(numWriters+numReaders >49){
		fprintf(stderr,"Too many Readers and/or Writers. Number of total threads is limited to 50.\n");
		exit(1);
	}else if(numPriorities <= 0){
		fprintf(stderr, "Please specify a number larger than zero for the number of priorities.");
		exit(1);
	}

	if(argc==1){
		puts("No parameters given, using default values of 3 readers, 2 writers and 3 priorities");
		usleep(500000);
	} else {
		printf("Starting the program with %d writers, %d readers and %d priorities\n", numWriters, numReaders, numPriorities);
		usleep(500000);
	}

	tq =createQueue(numPriorities);
	//Seeding the random generator
	seedRandom();

	pthread_mutex_init(&mutex,NULL);

	//Initializing the new SIGINT action
	struct sigaction new_sa;
	__sigset_t sigset;
	//No other signals have to be blocked (?)
	sigemptyset(&sigset);
	new_sa.sa_mask = sigset;
	//adding pointer to the new handler
	new_sa.sa_handler = (void *) &cleanUp;
	//Defining as a ONESHOT, so after one call all is reset
	new_sa.sa_flags = SA_ONESHOT;
	//replacing old with new, discarding the old one to NULL
	int res = sigaction(SIGINT,&new_sa,NULL);
	if(res != 0){
		fprintf(stderr,"An error occurred while trying to replace SIGINT handler.\n");
		exit(1);
	}

	//Initializing the writer and reader arrays
	pthread_t **writers;
	writers = (pthread_t**) malloc(numWriters * sizeof(pthread_t* ));
	pthread_t **readers;
	readers = (pthread_t**) malloc(numReaders * sizeof(pthread_t* ));

	//The threads may run
	run = TRUE;

	//Creating threads
	for(i=0;i<numWriters;i++){
		writers[i] = malloc(sizeof(pthread_t));
		pthread_create(writers[i],NULL,(void*)writerRunner,(void *)i);
	}
	for(i=0;i<numReaders;i++){
		readers[i] = malloc(sizeof(pthread_t));
		pthread_create(readers[i],NULL,(void*)readerRunner,(void *)i);
	}

	//Joining threads
	for(i=0;i<numWriters;i++){
		pthread_join(*writers[i],NULL);
	}
	for(i=0;i<numReaders;i++){
		pthread_join(*readers[i],NULL);
	}
	printf("\nDeallocated the Readers and Writers\n");

	printf("\n%i task(s) still in the queue\n", numTasksInQueue(tq));
	if (verbose == 1) {
		printf("Tasks still in the TaskQueue:\n");
		printQueue(tq);
	}
	printf("\nDeallocating all Tasks...\n");
	clearTaskQueue(tq);
	printf("\nDeallocating the TaskQueue...\n");
	free(tq);
	printf("\nDeallocating the Thread arrays ...\n");
	for(i=0;i<numWriters;i++)
		free(writers[i]);
	for(i=0;i<numReaders;i++)
		free(readers[i]);
	free(readers);
	free(writers);

	pthread_mutex_destroy(&mutex);

	printf("\nTerminating program\n");
	return 0;
}

/*The method the writer threads use*/
void *writerRunner(void *arg){
	struct Writer *w;
	w = malloc(sizeof(struct Writer));

	w->id = (int)arg;
	w->task_id = 10000*(int)arg;
	w->queue = tq;

	createTasks(w);

	if (verbose == 1)
			printf("Deallocating Writer%02i memory ...\n",w->id);
	free(w);

	pthread_exit(0);
}

/*The method the reader threads use*/
void *readerRunner(void *arg){
	struct Reader *r;
	r = malloc(sizeof(struct Reader));

	r->tq = tq;
	r->id = (int)arg;

	executeNextTask(r);
	if (verbose == 1)
		printf("Deallocating Reader%02i memory ...\n",r->id);
	free(r);

	pthread_exit(0);
}

/*Method for handling the SIGINT */
void cleanUp(void){
	printf("\n\nSIGINT received, shutting down ...\n\n");
	run = FALSE;
}
