#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <assert.h>

#include <pthread.h>

#include "queue.h"

const char* TYPE1 = "heart_beat";
const char* TYPE2 = "blood_sugar";
const char* TYPE3 = "body_temp";
long timer[40];
long ending[10];
int closed[40];
//the wearable server socket, which all wearables connect to
int wearable_server_fd;



int arraycount = 0;



int thread_count = 0;

//a lock for your queue sctructure... (use it)
pthread_mutex_t queue_lock_;

pthread_cond_t cv;
//a queue for all received data... 
queue_t receieved_data_;


typedef struct SampleData {

	char type_[50];
	int data_;

} SampleData;

int compare (const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

int selector1(void *arg) {
	
    SampleData* data = (SampleData*) arg;
    return strcmp(data->type_, TYPE1) == 0;
}

int selector2(void *arg) {

    SampleData* data = (SampleData*) arg;
    return strcmp(data->type_, TYPE2) == 0;
}

int selector3(void *arg) {
	
    SampleData* data = (SampleData*) arg;
    return strcmp(data->type_, TYPE3) == 0;
}

/**
Used to write out the statistics of a given results set (of timestamp_entry's).
To generate the result set see queue_gather(). fd is the file descriptor to
which the information is sent out. The type is the type of data that is written out
(TYPE1, TYPE2, TYPE3). results is the array of timestamp_entrys, and size is 
the size of that array. NOTE: that you should call method for every type 
(TYPE1, TYPE2, TYPE3), and then write out the infomration "\r\n" to signify that
you have finished sending out the results.
*/
void write_results(int fd, const char* type, timestamp_entry* results, int size) {
    long avg = 0;
    int i;

    char buffer[1024];
    int temp_array[size];
    sprintf(buffer, "Results for %s:\n", type);
    sprintf(buffer + strlen(buffer), "Size:%i\n", size);
    for (i = 0;i < size;i ++) {
        temp_array[i] = ((SampleData*)(results[i].data_))->data_;
        avg += ((SampleData*)(results[i].data_))->data_;
    }

    qsort(temp_array, size, sizeof(int), compare);

    if (size != 0) {
    	sprintf(buffer + strlen(buffer), "Median:%i\n", (size % 2 == 0) ?
            (temp_array[size / 2] + temp_array[size / 2 - 1]) / 2 : temp_array[size / 2]);
    } else {
        sprintf(buffer + strlen(buffer), "Median:0\n");
    }

    sprintf(buffer + strlen(buffer), "Average:%li\n\n", (size == 0 ? 0 : avg / size));
    write(fd, buffer, strlen(buffer));
}

/**
Given an input line in the form <timestamp>:<value>:<type>, this method 
parses the infomration from the string, into the given timestamp, and
mallocs space for SampleData, and stores the type and value within
*/
void extract_key(char* line, long* timestamp, SampleData** ret) {
	*ret = malloc(sizeof(SampleData));
	sscanf(line, "%zu:%i:%[^:]%:\\.*", timestamp, &((*ret)->data_), (*ret)->type_);
}


void* wearable_processor_thread(void* args) {
	int socketfd = *((int*)args);

	long timestamp;
	SampleData* ret;
	
	pthread_mutex_lock(&queue_lock_);
	
	int temp=thread_count;
	closed[temp]=0;
	thread_count++;
	
	pthread_mutex_unlock(&queue_lock_);
	//Use a buffer of length 64!
	//TODO read data from the socket until -1 is returned by read
	
	char buffer[64];
	while (recv(socketfd, buffer, 64, 0) > 0){

		extract_key(buffer, &timestamp, &ret);
		timer[temp]=timestamp;
		pthread_cond_broadcast(&cv);
		pthread_mutex_lock(&queue_lock_);
		queue_insert(&receieved_data_, timestamp, (void*)ret);
		pthread_mutex_unlock(&queue_lock_);
		
		/*
		while(endtimearray[i] <= (unsigned long) timestamp){
			i++;
			printf("%d\n", i);
			if(endtimearray[i] > (unsigned long) timestamp){
				//sock_count--;
				i = 0;
				//pthread_cond_broadcast(&cv);
				continue;	
			}
		}*/
	
		
	}

	pthread_mutex_lock(&queue_lock_);
	
	closed[temp]=1;
	pthread_cond_broadcast(&cv);
	
	pthread_mutex_unlock(&queue_lock_);



	close(socketfd);
	return NULL;
}



void* user_request_thread(void* args) {
	int socketfd = *((int*)args);
	int i, ret;


	//TODO rread data from the socket until -1 is returned by read
	//Requests will be in the form
	//<timestamp1>:<timestamp2>, then write out statiticcs for data between
	//those timestamp ranges
	char buffer2[64];	

	while(recv(socketfd, buffer2, 64, 0) > 0){//not recev

		unsigned long start = 0;
		unsigned long end = 0;

		//read(socketfd, buffer2, 64);

		sscanf(buffer2, "%lu:%lu", &start, &end);   // get start and end time
		
		
     ending[arraycount]=end;

		
		
	
		
		
			
		
			

	
		pthread_mutex_lock(&queue_lock_);
		
		int l=0;
		
			for (l;(l<40);l++){
			  // if (closed[l]==-1||timer[l]==0) break;
			  // if (l=thread_count) break;
			  
		       if  ( (ending[arraycount] >(unsigned long) timer[l])&&(closed[l]==0)) {
		       	l--;
		       	pthread_cond_wait(&cv, &queue_lock_);
		       }
				
			
			}
			
			
			
			
			
			
			
			
			
		
		
		pthread_mutex_unlock(&queue_lock_);	


		int s1;
		int s2;
		int s3;

		//void write_results(int fd, const char* type, timestamp_entry* results, int size)

		timestamp_entry * mytime1 = queue_gather(&receieved_data_, start, end, selector1, &s1);	
		//printf("%d\n", s1);
		

		timestamp_entry * mytime2 = queue_gather(&receieved_data_, start, end, selector2, &s2);		
		//printf("%d\n", s2);
		

		timestamp_entry * mytime3 = queue_gather(&receieved_data_, start, end, selector3, &s3);		
		//printf("%d\n", s3);

		write_results(socketfd, TYPE1, mytime1, s1);

		write_results(socketfd, TYPE2, mytime2, s2);    // fail there......

		write_results(socketfd, TYPE3, mytime3, s3);		

		write(socketfd, "\r\n", 2);
		
		
			
		arraycount++;
		
		
			
	}	
	
	close(socketfd);
	return NULL;
}

//IMPLEMENT!
//given a string with the port value, set up a 
//serversocket file descriptor and return it
int open_server_socket(const char* port) {
	//TODO
	int s, sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; /* IPv4 only */
    hints.ai_socktype = SOCK_STREAM; /* TCP */
    hints.ai_flags = AI_PASSIVE;

    s = getaddrinfo(NULL, port, &hints, &result);
   

    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
    }

  //  int optval = 1;
   // setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));   // make it reuseable

    if (bind(sockfd, result->ai_addr, result->ai_addrlen) != 0)
    {
        perror("bind()");
        exit(1);
    }

    if (listen(sockfd, 40) != 0)
    {
        perror("listen()");
        exit(1);
    }

	return sockfd;
}

void signal_received(int sig) {
	close(wearable_server_fd);
	//TODO close server socket, free anything you dont free in main


}

int main(int argc, const char* argv[]) {
	int k=0;
	for (k=0;k<40;k++) {
	timer[k]=(double)0;
	closed[k]=-1;}
	if (argc != 3) {
		printf("Invalid input size - usage: wearable_server <wearable_port> <request_port>\n");
		exit(EXIT_FAILURE);
	}

	//TODO setup sig handler for SIGINT
	signal(SIGINT, signal_received);
	
	int request_server_fd = open_server_socket(argv[2]);
	wearable_server_fd = open_server_socket(argv[1]);

	pthread_t request_thread;
    
    //HERE WE SET UP THE REQUEST CLIENT
	int request_socket = accept(request_server_fd, NULL, NULL);
	pthread_create(&request_thread, NULL, user_request_thread, &request_socket);
    //HERE WE CLOSE THE SERVER AFTER WE HAVE ACCEPTED OUR ONE CONNECITON
	close(request_server_fd);

	queue_init(&receieved_data_);
	pthread_mutex_init(&queue_lock_, NULL);

	pthread_cond_init(&cv, NULL);

	//TODO accept continous requests
	int * wear_sock = (int*)malloc(sizeof(int)*40);        //malloc

	int threadcount = 0;  // 
	int sockcount=0;
	pthread_t * wearthread = (pthread_t*)malloc(sizeof(pthread_t)*40);  // create threads for wearable_processor

	//puts("loopbegin");

	while((wear_sock[sockcount] = accept(wearable_server_fd, NULL, NULL)) != -1){
		//puts("loop");
		//printf("%d\n", sock_count);
		//wear_sock[sock_count] = accept(wearable_server_fd, NULL, NULL);
		

		/*if(wear_sock[sock_count] == -1){
			break;			
		}*/
		//puts("threads");
		pthread_create(&wearthread[sockcount], NULL, wearable_processor_thread, &wear_sock[sockcount]);
		//puts("here");
		
		pthread_mutex_lock(&queue_lock_);
	

		
		pthread_mutex_unlock(&queue_lock_);
		sockcount++;
		threadcount++; 
		
		//printf("%d\n", sock_count);
	}

	close(wearable_server_fd);

	

	//TODO join all threads we spawned from the wearables

		int i;
	for (i = 0; i < threadcount; i++){
		pthread_join(wearthread[i], NULL);
	}

	

	pthread_join(request_thread, NULL);
	queue_destroy(&receieved_data_, 1);

	return 0;
}
