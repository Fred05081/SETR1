/** @file test.c
 * @brief test.c file user interface:
 * In this script we can create a FIFO with variable size 
 * and interact with it
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
 * 
 * Main has no input arguments.
 * The main has an infinite loop that you can
 * create and interact with a FIFO in particular:
 * 
 * -insert elements
 * 
 * -remove elements
 * 
 * -peep the oldest element present in the FIFO
 * 
 * -know the size of the FIFO
  
 * @return main() always returns 0
 */
int main(void)
{

	int choice; // Escolher um dos 5 módulos
	int siz;
	int ins;
	printf ("FIFO IMPLEMENTATION\n\n");

	while(1){
	
		printf("1. Init\n2. Insert\n3. Remove\n4. Peep\n5.Length of array\n 6.Exit\n");
		printf("Enter your choice:");
		scanf("%d",&choice);

		switch(choice)
		{
			case 1:
				printf("Size of FIFO: \n");
				scanf("%d",&siz);
				MyFIFOInit(siz);
			break;
			case 2:
				printf("Enter the element to be inserted:\n");
				scanf("%d",&ins);
				MyFIFOInsert(ins);
			break;
			case 3:
				MyFIFORemove();
			break;
			case 4:
				MyFIFOPeep();
			break;
			case 5:
				MyFIFOSize();
			break;
			case 6:
				return 0;
			break;
			default:
			printf("Sorry, invalid choice!\n");
			break;
		}
	}
	
	return 0;
}
	

