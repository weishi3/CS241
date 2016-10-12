#include "test_utility.c"

void main(void)
{
    load();
	STD_ADD_SUCCESS(1);
	if (STD_UPDATE_SUCCESS(2, 2, 1))
    	fail(4);
	if (STD_UPDATE_SUCCESS(1, 2, 2))
    	fail(4);
	if (STD_GET_SUCCESS(2, 1, 1))
    	fail(4);
	if (STD_GET_SUCCESS(1, 1, 2))
    	fail(4);
	if (STD_DELETE_SUCCESS(2))
    	fail(4);
    save();
    success(4);
}
