/** @file datastore_control.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <assert.h>
#include "datastore_control.h"
#include "datastore.h"
pthread_mutex_t lock;
hash_table_t *my_hash_table;
void load()
{
pthread_mutex_init(&lock, NULL);
FILE* output;
output=fopen("data/Data.txt","rw");
char* line=NULL;
size_t len=0;
int a=0;



int size_of_table = 10000;
my_hash_table = create_hash_table(size_of_table);

if (output!=NULL){
	while ((a=getline(&line,&len,output))!=EOF){
	
	char* key=strtok(line," ");
	char* value=strtok(NULL," ");
	char* rev=strtok(NULL," ");
	unsigned long revm=atoi(rev);
	add_string(my_hash_table,key,value,revm);
	}
	free(line);
	fclose(output);	
	
}	


//int size_of_table = 999999;
//my_hash_table = create_hash_table(size_of_table);
}

void save()
{

FILE *fp;
fp=fopen("data/Data.txt","w+");
list_t * temp;


if (fp==NULL){
	fprintf(stderr, "Cannot open file for writing.\n");
	
	
	exit(1);
}
int i;
for (i=0;i<my_hash_table->size;i++){
	temp=my_hash_table->table[i];
	while(temp!=NULL){
			fprintf(fp,"%s %s %lu\n",temp->str,temp->value,temp->rev);

			temp=temp->next;
			
	
	}
	//free(temp);
}

free_table(my_hash_table);

fclose(fp);
pthread_mutex_destroy(&lock);


}

jsonres_t process_request(const char * uri, jsonreq_t request)
{
    jsonres_t response;
    
    if (uri)
    {
    	//add
    	if ( !strcmp(uri, "/add")){
    		if ( lookup_string(my_hash_table, request.key)!=NULL){
    			//printf("%d",DS_HAS_KEY(mytable, request.key));
    			 response.success="KEY ALREADY EXISTS";}
    		else {
    			
    			add_string(my_hash_table, request.key,request.value,1);
    			response.success="true";
			}	    	
		}
		else if ( !strcmp(uri, "/update")){
			if (	lookup_string(my_hash_table, request.key)==NULL)	
				response.success="KEY DOES NOT EXIST";
			else if (request.rev!=(lookup_string(my_hash_table, request.key)->rev)){
				response.success="REVISION NUMBER DOES NOT MATCH";
			}
    		else {
    		pthread_mutex_lock(&lock);
    			update(my_hash_table,request.key,request.value);
    			response.success="true";
    			response.rev=request.rev+1;
    			pthread_mutex_unlock(&lock);
    		}
    	}	
    	else if (!strcmp(uri, "/get")){
    		if (	lookup_string(my_hash_table, request.key)==NULL)	
				response.success="KEY DOES NOT EXIST";
			else {
			response.success="true";
			response.value=(lookup_string(my_hash_table, request.key)->value);
			response.rev=(lookup_string(my_hash_table, request.key)->rev);
			
			}
    	}
    	else if (!strcmp(uri, "/delete")){
    		if (	lookup_string(my_hash_table, request.key)==NULL	)
				response.success="KEY DOES NOT EXIST";
			else {
				 remove_key(my_hash_table, lookup_string(my_hash_table, request.key), request.key);
				//removeele(my_hash_table,request.key);
				response.success="true";
			}
    	}	    
	/*response.rev = 0;
	response.value = "0";
	response.success = "true";*/
	
	
	
	
    }
    return response;


}
