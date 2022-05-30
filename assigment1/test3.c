/** @file test3.c
 * @brief test3.c file brief decription 
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
    int var= MyFIFOPeep();
    printf ("elemento mais antigo: %d \n",var);//devolver o valor 1
    int rem =MyFIFORemove();
    printf ("elemento removido: %d \n",rem);//removeu o 1
    int var2= MyFIFOPeep();
    printf ("elemento mais antigo 2: %d \n",var2);//o elemnto mais antigo passa a ser o 2
    int total= MyFIFOSize();
    printf ("total de elementos: %d \n",total); //tem de devolver 2
    int rem2 =MyFIFORemove();
    int rem3 =MyFIFORemove();
    int rem4 =MyFIFORemove();
    int total3= MyFIFOSize();
    printf ("total de elementos: %d \n",total3); //tem de devolver 0
    MyFIFOInsert(10);
    int total2= MyFIFOSize();
    printf ("total de elementos: %d \n",total2); //tem de devolver 1

	return 0;
}