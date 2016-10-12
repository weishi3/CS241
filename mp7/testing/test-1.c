#include "test_utility.c"

void main(void)
{
    load();
    if (!STD_ADD_SUCCESS(1))
    {
        fail(1);
    }
    save();
    success(1);
}
