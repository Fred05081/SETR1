#include "MY_FIFO.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef MAX_SIZE
#define MAX_SIZE 10
static int fifo_array[MAX_SIZE];
static int head;
static int tail;
static int size_T;
static int flag=0;
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
    if (flag==0){

        fifo_array[head % size_T] = add;
        head++;
        flag=1;
    }
    else{

        if ((head % size_T) == (tail % size_T) ){
            printf("the oldest element are removed and inserted a new value\n");
            fifo_array[head % size_T] = add;
            head++;
            tail++;
        }
        else{
            fifo_array[head % size_T] = add;
            head++;
        }  
    }
    
    
}

int MyFIFORemove(void)
{
    int const1=0;
    if (tail == head)
    {   printf("O FIFO está vazio\n");
        return -1;
        
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
    printf("O elemento mais antigo é %d\n",num);
    return num;
}

int MyFIFOSize(void)
{
    int size;
    size = head - tail;
    printf("FIFO Size:  %d\n", size);
    return size;
}
#endif