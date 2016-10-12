#include "test_utility.c"

int STRESS_NUMBER = 1000;

void main(void)
{
    load();
	int i;
	for(i=0; i<STRESS_NUMBER; i++)
    	if (!STD_ADD_SUCCESS(i))
        	fail(2);
	for(i=0; i<STRESS_NUMBER; i++)
		if (!STD_UPDATE_SUCCESS(i, i+1, 1))
        	fail(2);
	/*(for(i=0; i<STRESS_NUMBER; i++)
		if (!STD_GET_SUCCESS(i, i+1, 2))
        	fail(2);
	for(i=0; i<STRESS_NUMBER; i++)
		if (!STD_DELETE_SUCCESS(i))
        	fail(2);
	for(i=0; i<STRESS_NUMBER; i++)
 		if (!STD_GET_FAILURE(i, i, 2))
        	fail(2);*/
    save();
   
    success(2);
    
}
