#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <pthread.h>
#include "wearable.h"

const char* TYPE1 = "heart_beat";
const char* TYPE2 = "blood_sugar";
const char* TYPE3 = "body_temp";


//Our worker threads
void* wearable_thread(void* param) {
	Wearable* wearable = (Wearable*) param;
    unsigned long timestamp = wearable->start_sleep_;
    queue_init(&wearable->queued_results_);

	usleep(wearable->start_sleep_ * 1000); //sleep the start time

    //set up a socket for connection
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int ret;
    if ((ret = getaddrinfo(NULL, wearable_gPort_, &hints, &result)) != 0) {
        fprintf(stderr, "Could not connect to server port %s\n", wearable_gPort_);
        exit(EXIT_FAILURE);
    }
    
    //open connection
    ret = connect(socketfd, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);

    char buffer[64];
    //loop through all data and send it to server
    int i;
    for (i = 0;i < wearable->data_size_;i ++) {
		sprintf(buffer, "%zu:%i:%s:\n", timestamp, wearable->data_[i].data_, wearable->data_[i].type_);
		queue_insert(&wearable->queued_results_, timestamp, &wearable->data_[i]);
        write(socketfd, buffer, 64);
        usleep(wearable->sample_interval_ * 1000); //sleep interval
        timestamp += wearable->sample_interval_;
	}
    close(socketfd);

	return wearable;
}

pthread_t* get_wearables(const char* file_name, int* thread_count, int** sample_times, int* sample_count, int* latest_launch) {
    (*latest_launch) = 0;
    (*thread_count) = 0;
    (*sample_times) = malloc(sizeof(int) * 3);
    (*sample_count) = 0;
	pthread_t* threads = malloc(sizeof(pthread_t));

	char * line = NULL;
    size_t read, len = 0;

    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
    	fprintf(stderr, "Could not find policy file %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    //parse in all wearables and send them over socket
    Wearable* wearable;
    while ((read = getline(&line, &len, fp)) != (size_t) -1) {
    	if (strncmp(line, "BEGIN", 5) == 0) {
    		wearable = malloc(sizeof(Wearable));
    		wearable->data_size_ = 0; wearable->data_ = NULL;
    	} else if (strncmp(line, "END", 3) == 0) {
    		pthread_create(&threads[(*thread_count)], NULL, wearable_thread, wearable);
    		(*thread_count) ++;
	    	threads = realloc(threads, sizeof(pthread_t) * (*thread_count + 1));
    	} else { //data in files is <key>:<value>
    		char* semi;
    		char* key;
    		char* val;
    		key = line;
    		semi = strstr(line, ":");
    		val = semi + 1;
    		*semi = '\0';

            if (strcmp(key, "SAMPLE_INT") == 0) {
                int wait, t1, t2;
                sscanf(val, "%i:%i:%i", &wait, &t1, &t2);

                (*sample_times)[(*sample_count) * 3] = wait;
                (*sample_times)[(*sample_count) * 3 + 1] = t1;
                (*sample_times)[(*sample_count) * 3 + 2] = t2;
                (*sample_count) ++;
                (*sample_times) = realloc(*sample_times, sizeof(int) * (*sample_count + 1) * 3);
    		} else if (strcmp(key, "START") == 0) {
    			wearable->start_sleep_ = atoi(val);
                (*latest_launch) = ((*latest_launch > wearable->start_sleep_) ? *latest_launch : wearable->start_sleep_);
    		} else if (strcmp(key, "INTERVAL") == 0) {
    			wearable->sample_interval_ = atoi(val);
    		} else {
    			wearable->data_ = realloc(wearable->data_, sizeof(SampleData) * (wearable->data_size_ + 1));
    			wearable->data_[wearable->data_size_].type_ = malloc(strlen(key) + 1);
                strcpy(wearable->data_[wearable->data_size_].type_, key);
    			wearable->data_[wearable->data_size_].data_ = atoi(val);
    			wearable->data_size_ ++;
    		}
    	}
    }

    if (line)
        free(line);

    return threads;
}

void free_wearable(Wearable* wearable) {
    free(wearable->data_);
    queue_destroy(&wearable->queued_results_, 0);
    free(wearable);
}
