#include "datastore_control.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_LEN 64

#define KEY_DNE "KEY DOES NOT EXIST"
#define KEY_AE "KEY ALREADY EXISTS"
#define WRONG_REV "REVISION NUMBER DOES NOT MATCH"

#define STD_ADD_SUCCESS(i) STD_ADD(i,"true")
#define STD_ADD_FAILURE(i) STD_ADD(i,KEY_AE)

#define STD_UPDATE_SUCCESS(i,j,k) STD_UPDATE(i,j,k,"true")
#define STD_UPDATE_FAIL_KEY(i,j,k) STD_UPDATE(i,j,k,KEY_DNE)
#define STD_UPDATE_FAIL_REV(i,j,k) STD_UPDATE(i,j,k,WRONG_REV)

#define STD_GET_SUCCESS(i,j,k) STD_GET(i,j,k,"true")
#define STD_GET_FAILURE(i,j,k) STD_GET(i,j,k,KEY_DNE)

#define STD_DELETE_SUCCESS(i) STD_DELETE(i,"true")
#define STD_DELETE_FAILURE(i) STD_DELETE(i, KEY_DNE)



void fail(int i)
{
	printf("test-%d: FAIL\n",i);
	exit(0);

}

void success(int i)
{
	printf("test-%d: SUCCESS\n",i);
	exit(0);

}


int ADD(char * key, char * value, char * success)
{
	jsonreq_t input;
	input.key = key;
	input.value = value;
	jsonres_t output = process_request("/add", input);
	if (strcmp(output.success,success))
		return 0;
	return 1;
}

int STD_ADD(int i, char * success)
{
	char str[BUFFER_LEN];
	sprintf(str, "%d", i);
	return ADD(str,str,success);
}

int UPDATE(char * key, char * value, int rev, char * success)
{
	jsonreq_t input;
	input.key = key;
	input.value = value;
	input.rev = rev;
	jsonres_t output = process_request("/update", input);
	if (strcmp(output.success,success))
		return 0;
	if (!strcmp(success,"true"))
		if (output.rev != (rev + 1))
			return 0;
	return 1;
}

int STD_UPDATE(int i, int j, int k, char * success)
{
	char str1[BUFFER_LEN];
	char str2[BUFFER_LEN];
	sprintf(str1, "%d", i);
	sprintf(str2, "%d", j);
	return UPDATE(str1, str2, k, success);
}


int GET(char * key, char * value, int rev, char * success)
{
	jsonreq_t input;
	input.key = key;
	jsonres_t output = process_request("/get", input);
	if (strcmp(output.success,success))
		return 0;
	if (!strcmp(success,"true"))
	{
		if (strcmp(output.value,value))
			return 0;
		if (rev != -1)
			if (output.rev != rev)
				return 0;
	}
	return 1;
}

int STD_GET(int i, int j, int k, char * success)
{
	char str1[BUFFER_LEN];
	char str2[BUFFER_LEN];
	sprintf(str1, "%d", i);
	sprintf(str2, "%d", j);
	return GET(str1, str2, k, success);
}

int DELETE(char * key, char * success)
{
	jsonreq_t input;
	input.key = key;
	jsonres_t output = process_request("/delete", input);
	if (strcmp(output.success,success))
		return 0;
	return 1;
}

int STD_DELETE(int i, char * success)
{
	char str[BUFFER_LEN];
	sprintf(str, "%d", i);
	return DELETE(str, success);
}
