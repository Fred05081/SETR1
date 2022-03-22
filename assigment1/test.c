/** @file test.c
 * @brief test.c file brief decription 
 *
 * Follows the detailed description of MY_FIFO.c. It is separated from 
 * the brief one by a blank line.
 * In this case test.c is the file that contains the main() function. 
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

	int choice; // Escolher um dos 5 módulos
	

	printf ("FIFO IMPLEMENTATION\n\n");

	do{
	
		printf("1. Init\n2. Insert\n3. Remove\n4. Peep\n5.Length of array\n\n");
		printf("Enter your choice:");
		scanf("%d",&choice);

		switch(choice)
		{
			case 1:
				
				MyFIFOInit();
			break;
			case 2:
				//printf("Enter the element to be inserted:");
				MyFIFOInsert();
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
				exit(0);
			break;
			default:
			printf("Sorry, invalid choice!\n");
			break;
		}
	} while(choice!=5);
	
	return 0;
}
	

