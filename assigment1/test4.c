/** @file test4.c
 * @brief test4.c Insert a value when the FIFO is Full
 *
 * In this script we want to know what happens when the FIFO are 
 * already full and we want insert a new element
 *  
 * 
 * @author Ana Sousa, Frederico Moreira, Pedro Rodrigues
 * @date 22 March 2022
 * @bug No known bugs.
 */

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include "MY_FIFO.h"


/**
 * @brief Brief decription of main().
 *
 * Here it goes the long description of main()
 * main has no input arguments.
 * It then prints the result and returns.
 * 
 * @return main() always returns 0
 */
int main(void)
{

	int choice=5; // Escolher um dos 5 módulos
	
	printf ("FIFO IMPLEMENTATION\n");
    printf("inserir o numero de elementos:\n");
	//choice =scanf("%d",&choice);
    MyFIFOInit(choice);
    MyFIFOInsert(1);
    MyFIFOInsert(2);
    MyFIFOInsert(3);
    MyFIFOInsert(4);
    MyFIFOInsert(5);
    MyFIFOInsert(6);
    MyFIFOInsert(6);
    

	return 0;
}