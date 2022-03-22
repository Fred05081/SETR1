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
 * res = function1(param1, param2);
 * printf("res=%d\n",res);
 * @endcode
 * @param tamanho size of the FIFO.
 * @return Não retorna nada.
 */
int MyFIFOInit(int tamanho);

/**
 * @brief Insert an element in the FIFO.
 *
 * Esta função adiciona um determinado elemento inserido pelo utilizador na
 * posição certa do FIFO. Tem assim como argumento o elemento a adicionar ao fifo
 * e não retorna nada
 * @code
 * res = function1(param1, param2);
 * printf("res=%d\n",res);
 * @endcode
 * @param add elemento a adicionar ao FIFO.
 * @return Não retorna nada.
 */

int MyFIFOInsert(int add);

/**
 * @brief remover o ultimo elemento inserido.
 * Esta função remove o elemento mais antigo inserido no FIFO e devolve
 * -1 se o FIFO estiver vazio
 * @code
 * res = function1(param1, param2);
 * printf("res=%d\n",res);
 * @endcode
 * @param No_param No parameters
 * @return retorna -1 se nao existir elementos 
 */
int MyFIFORemove(void);

/**
 * @brief Apenas ver o elemento mais antigo do FIFO.
 * @code
 * res = function1(param1, param2);
 * printf("res=%d\n",res);
 * @endcode
 * @param NO_args sem argumentos
 * @return Retorna o valor mais antigo do FIFO 
 */
int MyFIFOPeep(void);

/**
 * @brief Brief decription of function1 prototype.
 *
 * Here it goes the long description of fucntion 1.
 * Description of what the function does. Usually this part describes 
 * whatr the fiunction does and its interface, namelly input 
 * arguments and return value.
 * Verbatim text can also be used:
 * @verbatim 
 * This is my email:pbrp@ua.pt
 * @endverbatim
 * You can also include an example of usage:
 * @code
 * res = function1(param1, param2);
 * printf("res=%d\n",res);
 * @endcode
 * You can uset HTML tags e.g. to make <b>bold text</b>
 * @param arg1 Description of the first parameter of the function.
 * @param arg2 Description of the second parameter of the function.
 * @return Describe the function return value 
 */
int MyFIFOSize(void);