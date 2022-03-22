//#include "MY_FIFO.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef MAX_SIZE
#define MAX_SIZE 10
static int fifo_array[];
static int head;
static int tail;
static int size_T;
void MyFIFOInit(int tamanho)
{
    if (tamanho > MAX_SIZE)
    {
        size_T = MAX_SIZE;
    }
    else{

        size_T = tamanho;
    }
    head = 0;
    tail = 0;
    for (int i = 0; i < size_T; i++)
    {
        fifo_array[i] = 0;
    }
}

void MyFIFOInsert(int add)
{
    fifo_array[head % size_T] = add;
    head++;
}

int MyFIFORemove(void)
{
    int const1=0;
    if (tail == head)
    {
        return -1;
        printf("O FIFO está vazio");
    }

    else
    {   const1= fifo_array[tail % size_T];
        fifo_array[tail % size_T] = 0;
        tail++;
        return const1;
    }
}

int MyFIFOPeep(void)
{
    int num;

    num = fifo_array[tail % size_T];
    printf("O elemento mais antigo é %d",num);
    return num;
}

int MyFIFOSize(void)
{
    int size;
    size = head - tail;
    printf("FIFO Size:  %d", size);
}
#endif