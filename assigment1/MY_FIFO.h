/** @file MY_FIFO.h
 * @brief FIFO means First In First Out 
 *
 * Contém as funções necessárias para criar um FIFO assim como adicionar
 * ou remover elementos e saber qual o ultimo elemento.
 * @author Frederico Moreira, Ana Sousa, Pedro Rodrigues
 * @date 22 March 2022
 * @bug No known bugs.
 */

#ifndef _MY_FIFO_h
#define _MY_FIFO_h


/**
 * @brief Initialize a FIFO with size <b>tamanho</b>.
 *
 * A função inicializa um FIFO ("Array") com tamanho do argumento de entrada
 * <b>tamanho</b> e não retorna nada 
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
 * @return Não retorna nada.
 */
void MyFIFOInit(int tamanho);

/**
 * @brief Insert an element in the FIFO.
 *
 * Esta função adiciona um determinado elemento inserido pelo utilizador na
 * posição certa do FIFO. Tem assim como argumento o elemento a adicionar ao fifo
 * e não retorna nada
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
 * @param add elemento a adicionar ao FIFO.
 * @return Não retorna nada.
 */

void MyFIFOInsert(int add);

/**
 * @brief remover o ultimo elemento inserido.
 * Esta função remove o elemento mais antigo inserido no FIFO e devolve
 * -1 se o FIFO estiver vazio
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
 * @return retorna -1 se nao existir elementos 
 */
int MyFIFORemove(void);

/**
 * @brief Apenas ver o elemento mais antigo do FIFO.
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
 * @param NO_args sem argumentos
 * @return Retorna o valor mais antigo do FIFO 
 */
int MyFIFOPeep(void);

/**
 * @brief numero total de elemntos
 *Esta função devovlve o numero total de elentos que o FIFO
contém no determinado momento e devolve este valor
 * @code
 *int MyFIFOSize(void)
{
    int size;
    size = head - tail;
    //printf("FIFO Size:  %d", size);
    return size;
}
 * @endcode
 * @param no_args nao tem argumentos
 * @param arg2 Description of the second parameter of the function.
 * @return retorna o numero de elemontos total do FIFO
 */
int MyFIFOSize(void);
#endif