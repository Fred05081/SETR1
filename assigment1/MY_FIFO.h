/** @file MY_FIFO.h
 * @brief FIFO means First In First Out 
 *
 * Contains the functions needed to create a FIFO as well as add
 * or remove elements and it know what the last element. 
 * @author Frederico Moreira, Ana Sousa, Pedro Rodrigues
 * @date 22 March 2022
 * @bug No known bugs.
 */

#ifndef _MY_FIFO_h
#define _MY_FIFO_h


/**
 * @brief Initialize a FIFO with size <b>tamanho</b>.
 *
 * The function initializes a FIFO ("Array") with input argument size
 * <b>tamanho</b> and it doesn't return anything
 * Example of usage:
 * @code
 * if (tamanho > MAX_SIZE)
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
 * @endcode
 * @param tamanho size of the FIFO.
 * @return it doesn't return anything.
 */
void MyFIFOInit(int tamanho);

/**
 * @brief Insert an element in the FIFO.
 *
 * This function adds a certain element inserted by the user at the 
 * rigth position of the FIFO.It also has the element to add to the FIFO as an arguement
 * and doesn't return anything
 * @code
 * void MyFIFOInsert(int add)
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
 * @endcode
 * @param add element to add to FIFO.
 * @return it doesn't return anything.
 */

void MyFIFOInsert(int add);

/**
 * @brief remove the last inserted element.
 * This function removes the oldest element inserted in the FIFO and returns 
 * -1 if the FIFO is empty
 * @code
 * int MyFIFORemove(void)
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
 * @endcode
 * @param No_param No parameters
 * @return return -1 if there is no element 
 */
int MyFIFORemove(void);

/**
 * @brief Only see oldest FIFO element.
 * @code
 * 
int MyFIFOPeep(void)
{
    int num;

    num = fifo_array[tail % size_T];
    //printf("O elemento mais antigo é %d",num);
    return num;
}
 * @endcode
 * @param NO_args without arguments
 * @return Return the oldest FIFO value  
 */
int MyFIFOPeep(void);

/**
 * @brief Total number of elements
 * This function returns the total numbers that the FIFO
 * contains at the given time and returns this value
 * @code
 *int MyFIFOSize(void)
{
    int size;
    size = head - tail;
    //printf("FIFO Size:  %d", size);
    return size;
}
 * @endcode
 * @param no_args without arguments
 * @param arg2 Description of the second parameter of the function.
 * @return Returns the total number of FIFO elements. 
 */
int MyFIFOSize(void);
#endif
