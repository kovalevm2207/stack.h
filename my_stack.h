/*to do: дописать DcodeErr в плане параметров побольше
         сделать макросы с функциями проверок (чтобы меньше писать и иметь возможность их отключать)
         */


#ifndef MY_STACK
#define MY_STACK

#include <cstdlib>
#include <stdint.h>
#include "color_print.h"

typedef int stack_t;

const int MAXSIZE = 1000000000;

typedef enum
{
    ALL_OK              = 0 << 0,
    BAD_DATA_PTR        = 1 << 1,
    BAD_SIZE            = 1 << 2,
    BAD_START_CAPACITY  = 1 << 3,
    BAD_STK_CAPACITY    = 1 << 4,
    BAD_STK_PTR         = 1 << 5,
    BAD_VALUE           = 1 << 6
} StackErr_t;

struct stack_s
{
    stack_t* data;
    size_t size;
    size_t capacity;
};

StackErr_t StackCtor_(stack_s* stk, size_t capacity, const char* file, int line);
StackErr_t StackPush_(stack_s* stk, stack_t value, const char* file, int line);
StackErr_t StackPop_(stack_s* stk, stack_t* value, const char* file, int line);
StackErr_t StackDtor_(stack_s* stk, const char* file, int line);

#define StackCtor(stk, capacity) StackCtor_(stk, capacity, __FILE__, __LINE__)
#define StackPush(stk, value) StackPush_(stk, value, __FILE__, __LINE__)
#define StackPop(stk, value) StackPop_(stk, value, __FILE__, __LINE__)
#define StackDtor(stk) StackDtor_(stk, __FILE__, __LINE__)

int verify(stack_s* stk, int STATUS);
void StackDump(stack_s* stk, int STATUS);

#define START_OK if (capacity > MAXSIZE) STATUS |= BAD_START_CAPACITY;  \
    if (stk == NULL) STATUS |= BAD_STK_PTR;                             \
    if (STATUS != ALL_OK) {                                             \
        printf("\n" CHANGE_ON BOLD WITH LIGHT_RED TEXT_COLOR "StackDump() from %s at %s:%d\n" RESET,\
        __func__, file, line);                                          \
        StackDump(stk, STATUS);                                         \
        return (StackErr_t) STATUS;                                     \
    }

#define STACK_OK STATUS = verify(stk, STATUS);                         \
    if (STATUS != ALL_OK) {                                            \
        printf("\n" CHANGE_ON BOLD WITH LIGHT_RED TEXT_COLOR "StackDump() from %s at %s:%d\n" RESET,\
        __func__, file, line);                                         \
        StackDump(stk, STATUS);                                        \
        return (StackErr_t) STATUS;                                    \
    }

StackErr_t StackCtor_(stack_s* stk, size_t capacity, const char* file, int line)
{
    int STATUS = ALL_OK;

    START_OK

    stk->data = (stack_t*) calloc(capacity, sizeof(stack_t));
    stk->size = 0;
    stk->capacity = capacity;

    STACK_OK

    return ALL_OK;
}

StackErr_t StackPush_(stack_s* stk, stack_t value, const char* file, int line)
{
    int STATUS = ALL_OK;
    if (value > MAXSIZE) STATUS |= BAD_VALUE;
    STACK_OK

    stk->data[stk->size++] = value;

    return ALL_OK;
}

StackErr_t StackPop_(stack_s* stk, stack_t* value, const char* file, int line)
{
    int STATUS = ALL_OK;
    if (value == 0) STATUS |= BAD_VALUE;
    STACK_OK

    *value = stk->data[--stk->size];

    return ALL_OK;
}

StackErr_t StackDtor_(stack_s* stk, const char* file, int line)
{
    int STATUS = ALL_OK;
    STACK_OK

    free(stk->data);
    stk->data = NULL;
    stk->size = 0;
    stk->capacity = 0;
    stk = NULL;

    return ALL_OK;
}

int verify(stack_s* stk, int STATUS)
{
    if (stk == NULL) {
       STATUS |= BAD_STK_PTR;
        return STATUS;
    }
    if (stk->data == NULL)              STATUS |= BAD_DATA_PTR;
    if (stk->size > MAXSIZE)            STATUS |= BAD_SIZE;
    if (stk->capacity > MAXSIZE)        STATUS |= BAD_STK_CAPACITY;
    if (stk->size >= stk->capacity)     STATUS |= BAD_SIZE;
    return STATUS;
}

void StackDump(stack_s* stk, int STATUS)
{
    if (STATUS & ALL_OK) {
        printf(CHANGE_ON BOLD WITH GREEN TEXT_COLOR "ALL_OK(%d) ", ALL_OK);
        return;
    }

    printf("stack " CHANGE_ON GREEN TEXT_COLOR "<int>" CHANGE_ON BLUE TEXT_COLOR
           "       [0x%lx]" RESET " ____________________________ ", (uintptr_t)stk);
    printf(CHANGE_ON BOLD WITH LIGHT_RED TEXT_COLOR "ERROR! ");

    if (STATUS & BAD_VALUE)              printf("BAD_VALUE(%d) ",    BAD_VALUE);
    if (STATUS & BAD_START_CAPACITY)     printf("BAD_START_CAPACITY(%d) ",    BAD_START_CAPACITY);
    if (STATUS & BAD_STK_PTR)  {
        printf("BAD_STK_PTR(%d) ", BAD_STK_PTR);
        printf("\n");
        return;
    }

    if (STATUS & BAD_SIZE)                printf("BAD_SIZE(%d) ",     BAD_SIZE);
    if (STATUS & BAD_STK_CAPACITY)        printf("BAD_STK_CAPACITY(%d) ", BAD_STK_CAPACITY);
    if (STATUS & BAD_DATA_PTR)            printf("BAD_DATA_PTR(%d) ", BAD_DATA_PTR);
    printf(RESET "\n");
        printf("{\n");
        printf(CHANGE_ON GREEN TEXT_COLOR "    size       "
               CHANGE_ON BLUE TEXT_COLOR  "%zu" RESET "\n", stk->size);
        printf(CHANGE_ON GREEN TEXT_COLOR "    capacity   "
               CHANGE_ON BLUE TEXT_COLOR  "%zu" RESET "\n", stk->capacity);
        printf(CHANGE_ON GREEN TEXT_COLOR "    data   "
               CHANGE_ON BLUE TEXT_COLOR  "   [0x%lx]" RESET "\n", (uintptr_t)stk->data);
    if (!(STATUS & BAD_DATA_PTR || STATUS & BAD_SIZE || STATUS & BAD_STK_CAPACITY)) {
        printf("    {\n");
        for (size_t i = 0; i < stk->capacity; i++)
            if (i < stk->size) printf(CHANGE_ON YELLOW TEXT_COLOR "       *[%zu] = %d," RESET "\n", i, stk->data[i]);
            else printf("        [%zu] = %d,\n", i, stk->data[i]);
        printf("    }\n");
    }
    printf("}\n");

}

#endif // MY_STACK
