//#include "MY_FIFO.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef MAX_SIZE
#define MAX_SIZE 5
static int fifo_array[MAX_SIZE];
static int head;
static int tail;
void MyFIFOInit(int tamanho)
{
    if (tamanho > MAX_SIZE)
    {
        tamanho = MAX_SIZE;
    }
    head = 0;
    tail = 0;
    for (int i = 0; i < tamanho; i++)
    {
        fifo_array[i] = 0;
    }
}

void MyFIFOInsert(int add)
{
    fifo_array[head % MAX_SIZE] = add;
    head++;
}

int MyFIFORemove(void)
{

    if (tail == head)
    {
        return -1;
    }

    else
    {
        fifo_array[tail % MAX_SIZE] = 0;
        tail++;
    }
}

int MyFIFOPeep(void)
{
    int num;

    num = fifo_array[head - 1];

    return num;
}

int MyFIFOSize(void)
{
    int size;
    size = head - tail;
    printf("FIFO Size:  %d", size);
}
#endif