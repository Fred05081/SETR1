/** @file test4.c
 * @brief test4.c file brief decription 
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