#include <stdio.h>
#include "my_stack.h"

int main()
{
    stack_s stk1 = {};

    StackCtor(0, 5);
    StackCtor(0, -1);
    StackCtor(0, 1000000000 + 1);
    StackCtor(&stk1, -1);
    StackCtor(&stk1, 1000000000 + 1);
    StackCtor(&stk1, 5);
    printf("capasity = [%zu]\n", stk1.capacity);

    StackPush(0, 10);
    StackPush(&stk1, 10);
    stk1.size = 5;
    StackPush(&stk1, 10);
    stk1.size = 4; // <---------- а вот такое пока что не контрится
    StackPush(&stk1, 10);
    stk1.size = -1;
    StackPush(&stk1, 10);
    stk1.size = 1;
    StackPush(&stk1, 1000000001);
    StackPush(0, 1000000001);
    StackPush(&stk1, 20);
    StackPush(&stk1, 30);

    int test_data[4] = {0};

    StackPop(0, &test_data[0]);
    StackPop(&stk1, 0);
    StackPop(0, 0);
    StackPop(&stk1, &test_data[3]);

    StackDtor(0);
    StackDtor(&stk1);

    return 0;
}
