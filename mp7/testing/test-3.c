#include "test_utility.c"

int STRESS_NUMBER = 1000000;

void main(void)
{
    
    load();
	int i;
	for(i=0; i<STRESS_NUMBER; i++)
    	if (!STD_ADD_SUCCESS(i))
        	fail(3);
    save();
     
    load();
	for(i=0; i<STRESS_NUMBER; i++)
		if (!STD_UPDATE_SUCCESS(i, i+1, 1))
        	fail(3);
    save();
  
    load();
        for(i=0; i<STRESS_NUMBER; i++)
		if (!STD_GET_SUCCESS(i, i+1, 2))
        	fail(3);
    save();
     
    load();
        for(i=0; i<STRESS_NUMBER; i++)
		if (!STD_DELETE_SUCCESS(i))
        	fail(3);
    save();
     
    load();
        for(i=0; i<STRESS_NUMBER; i++)
 		if (!STD_GET_FAILURE(i, i, 2))
        	fail(3);
    save();
    success(3);
}
