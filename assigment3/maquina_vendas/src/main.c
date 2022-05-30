/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

/** @file main.c
 * @brief main.c Insert a coin to take a product.
 *
 * In this script we have a vending machine, which when we insert a coin we adding a credit the machine.
 *  We have the possibility to choose three product: beer, coffee and tuna sandwich.
 *  If we have available credict we can buy one of these products and  if we choose a product and we stay available credit, 
 * we can continue to choose more products. We can too add more coins if necessary. 
*  For return the available credit, we can click the return button and the credit is return.
 *  
 * 
 * @author Ana Sousa, Frederico Moreira, Pedro Rodrigues
 * @date 22 March 2022
 * @bug No known bugs.
 */

/* Includes */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <sys/__assert.h>
#include <string.h>
#include <timing/timing.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* Refer to dts file */
#define GPIO0_NID DT_NODELABEL(gpio0)
/* Refer to dts file */


#define BOARDBUT1 0xb /* Pin at which BUT1 is connected. Addressing is direct (i.e., pin number) */
#define BOARDBUT2 0xc
#define BOARDBUT3 0x18
#define BOARDBUT4 0x19
#define BOARDBUT5 0x03
#define BOARDBUT6 0x04
#define BOARDBUT7 28
#define BOARDBUT8 29


/* Define states*/ 
#define State_0 0
#define credit_return 1
#define check_credit_available 2
#define Next_Product 3
#define Previous_Product 4
#define credit_sum 5

/* Define prices of products*/ 
#define Beer 150
#define Tuna_sandwich 100
#define coffee 50


#define BLINKPERIOD_MS 500 /* Blink period in ms*/ 

/* Int related declarations */
static struct gpio_callback but1_cb_data; /* Callback structure */
static struct gpio_callback but2_cb_data; /* Callback structure */
static struct gpio_callback but3_cb_data; /* Callback structure */
static struct gpio_callback but4_cb_data; /* Callback structure */
static struct gpio_callback but5_cb_data; /* Callback structure */
static struct gpio_callback but6_cb_data; /* Callback structure */
static struct gpio_callback but7_cb_data; /* Callback structure */
static struct gpio_callback but8_cb_data; /* Callback structure */

// Variables that determine which button was selected
volatile int Flag_10_Cent = 0; /* Flag to signal a BUT1 press */
volatile int Flag_20_Cent = 0; /* Flag to signal a BUT1 press */
volatile int Flag_50_Cent = 0; /* Flag to signal a BUT1 press */
volatile int Flag_100_Cent = 0; /* Flag to signal a BUT1 press */
volatile int Up_Flag = 0; /* Flag to signal a BUT1 press */
volatile int Down_Flag = 0; /* Flag to signal a BUT1 press */
volatile int Check_credit_Flag = 0; /* Flag to signal a BUT1 press */
volatile int Return_Flag = 0; /* Flag to signal a BUT1 press */

volatile int NS = 0; /* variable next state*/

volatile int product= 150;/* Product selected by default-- Beer*/

void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Flag_10_Cent = 1;
}

void but2press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Flag_20_Cent = 1;
}

void but3press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Flag_50_Cent = 1;
}

void but4press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Flag_100_Cent = 1;
}
void but5press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Up_Flag = 1;
}

void but6press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Down_Flag = 1;
}

void but7press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Check_credit_Flag = 1;
}

void but8press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Return_Flag = 1;  
}

const struct device *gpio0_dev;         /* Pointer to GPIO device structure */


/* Main function */
/**
 * @brief Brief decription of main().
 *
 * 
 * Main has no input arguments.
 * Our system have eight buttons: the button 1 is the 10 cents;
 * the button 2 is the 20 cents;
 * the button 3 is the 50 cents and the button 4 is the 1 eur;
 *  the button 5 is the up, the button 6 is the down;
 * the button 7 is the selected product and the button 8 is the return credit.
 * 
 * If we insert a 10 cent coin, we are left with a 10 cent credit; 
 * if we add a 20 cent coin, we add 20 cent to the previous credit, that is, we are left with 30 cent and so on.
 *
 * In the up(5)/down(6) buttons, we can select the desired product;
 * where in the terminal we can see the previous, selected and next product.
 * 
 * If credit is available, it is possible to select and take the desired product. 
 * The available credit will be the previously available credit minus the product price.
 * 
 * We can insert the coins and if we don't want to select any product, we can do the return to get the money inserted. 
 * When we select a product and we still have credit available, we can return the change.
  
 * @return main() always returns 0
 */
void main(void) {

    /* Local vars */    
    int money=0; 
    
    /* Bind to GPIO 0 */
    gpio0_dev = device_get_binding(DT_LABEL(GPIO0_NID));

    if (gpio0_dev == NULL) {
        printk("Failed to bind to GPIO0\n\r");        
	return;
    }
    else {
        printk("Bind to GPIO0 successfull \n\r");        
    }

    CONF_BUTT();

    while(1) {
    
    
    switch  (NS){

      case (State_0):
        if (Flag_10_Cent==1 || Flag_20_Cent==1 || Flag_50_Cent==1|| Flag_100_Cent==1) {
          NS=credit_sum;
        }
        if (Return_Flag==1){
        NS=credit_return;
        Return_Flag=0;
        }

        if(Check_credit_Flag==1){/* select product*/
        NS= check_credit_available;
        Check_credit_Flag=0;
        }

        if(Up_Flag==1){/* select product*/
        NS= Next_Product;
        Up_Flag=0;
        }

        if(Down_Flag==1){/* select product*/
        NS= Previous_Product;
        Down_Flag=0;
        }

        break; 
/* ________________________________________________________________________ End of case */
      case(credit_sum):
      if (Flag_10_Cent==1){
        money=money+10;
        Flag_10_Cent=0;
        printk("Total Money: %d.%d EUR\n\r",money/100, money%100);
        
        }
        
        if (Flag_20_Cent==1){
        money=money+20;
        Flag_20_Cent=0;
        printk("Total Money:  %d.%d EUR\n\r",money/100, money%100);
        }
       
        if (Flag_50_Cent==1){
        money=money+50;
        Flag_50_Cent=0;
        printk("Total Money:  %d.%d EUR\n\r",money/100, money%100);
        }
        
        if (Flag_100_Cent==1){
        money=money+100;
        Flag_100_Cent=0;
        printk("Total Money: %d.%d EUR\n\r",money/100, money%100);
        }
        NS= State_0;
      break;
/* ________________________________________________________________________ End of case */

      case (credit_return):
        printk("Credit return: %d.%d EUR\n\r",money/100, money%100);
        money=0;
        NS=0;
        printk("Total Money:  %d.%d EUR\n\r",money/100, money%100);
        break;
/* ________________________________________________________________________ End of case */

      case(check_credit_available):
        if (money >= product){
          money=money-product;
          if(product==150){
            printk("Product Beer dispensed, remaining credit  %d.%d EUR\n\r",money/100, money%100);
          }

          if(product==100){
            printk("Product Tuna Sandwish dispensed, remaining credit  %d.%d EUR\n\r",money/100, money%100);
          }

          if(product==50){
            printk("Product coffee dispensed, remaining credit  %d.%d EUR\n\r",money/100, money%100);
          }
        
          NS= State_0;
          }
        else{

          if (product==150){
            printk("Not enough credit, Product Beer costs  %d.%d EUR, credit is  %d.%d EUR",product/100, product%100,money/100, money%100);
          }
          
          if (product==100){
            printk("Not enough credit, Product Tuna sandwish costs  %d.%d EUR, credit is  %d.%d EUR",product/100, product%100,money/100, money%100);
          }
          
          if (product==50){
            printk("Not enough credit, Product coffee costs  %d.%d EUR, credit is  %d.%d EUR",product/100, product%100,money/100, money%100);
          }
          
          NS= State_0;
          }
        break;
/* ________________________________________________________________________ End of case */

      case(Next_Product):
        printk("\n\r");
        if(product== 150){
          product= 100;
          printk("Next product -> Coffee 0.50 EUR\n\rproduct selected -> Tuna Sandwich -> 1.00 EUR\n\rprevious product -> Beer 1. 50 EUR\n\r");
          printk("Available Credit: %d.%d EUR\n\r",money/100, money%100);
          NS=State_0;
        }
        else if(product== 100){
          product= 50;
          printk("Next product -> Beer 1.50 EUR\n\rproduct selected ->  Coffee 0.50 EUR\n\rprevious product ->Tuna Sandwich -> 1.00 EUR\n\r");
          printk("Available Credit: %d.%d EUR\n\r",money/100, money%100);
          NS=State_0;
        }
        else if(product== 50){
          product= 150;
          printk("Next product ->Tuna Sandwich -> 1.00 EUR\n\rproduct selected -> Beer 1.50 EUR\n\rprevious product -> Coffee 0.50 EUR\n\r");
          printk("Available Credit: %d.%d EUR\n\r",money/100, money%100);
          NS=State_0;
        }
        break;
/* ________________________________________________________________________ End of case */

        case(Previous_Product):
          printk("\n\r");
          if(product== 150){
          product= 50;
          printk("Next product -> Beer 1.50 EUR\n\rproduct selected -> Coffee 0.50 EUR\n\rprevious product -> Tuna Sandwich -> 1.00 EUR\n\r ");
          printk("Available Credit: %d.%d EUR\n\r",money/100, money%100);
          NS=State_0;
        }
        else if(product== 100){
          product= 150;
          printk("Next product ->Tuna Sandwich -> 1.00 EUR\n\rproduct selected ->  Beer 1.50 EUR\n\rprevious product -> Coffee 0.50 EUR\n\r");
          printk("Available Credit: %d.%d EUR\n\r",money/100, money%100);
          NS=State_0;
        }
        else if(product== 50){
          product= 100;
          printk("Next product -> Coffee 0.50 EUR\n\rproduct selected -> Tuna Sandwich -> 1.00 EUR\n\rprevious product ->Beer 1. 50 EUR\n\r");
          printk("Available Credit: %d.%d EUR\n\r",money/100, money%100);
          NS=State_0;
        }
        break;
/* ________________________________________________________________________ End of case */
      default:
        break;
      
      }

    }                               
    return;
} 

void CONF_BUTT()
{
    int ret = 0;
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT1, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT2, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT3, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT4, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT5, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT6, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT7, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT8, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 1 \n\r", ret);
	return;
    }

    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT1, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT2, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT3, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT4, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT5, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT6, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT7, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT8, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT1 pin \n\r", ret);
	return;
    }

    gpio_init_callback(&but1_cb_data, but1press_cbfunction, BIT(BOARDBUT1));
    gpio_add_callback(gpio0_dev, &but1_cb_data);

    gpio_init_callback(&but2_cb_data, but2press_cbfunction, BIT(BOARDBUT2));
    gpio_add_callback(gpio0_dev, &but2_cb_data);

    gpio_init_callback(&but3_cb_data, but3press_cbfunction, BIT(BOARDBUT3));
    gpio_add_callback(gpio0_dev, &but3_cb_data);

    gpio_init_callback(&but4_cb_data, but4press_cbfunction, BIT(BOARDBUT4));
    gpio_add_callback(gpio0_dev, &but4_cb_data);

    gpio_init_callback(&but5_cb_data, but5press_cbfunction, BIT(BOARDBUT5));
    gpio_add_callback(gpio0_dev, &but5_cb_data);

    gpio_init_callback(&but6_cb_data, but6press_cbfunction, BIT(BOARDBUT6));
    gpio_add_callback(gpio0_dev, &but6_cb_data);

    gpio_init_callback(&but7_cb_data, but7press_cbfunction, BIT(BOARDBUT7));
    gpio_add_callback(gpio0_dev, &but7_cb_data);

    gpio_init_callback(&but8_cb_data, but8press_cbfunction, BIT(BOARDBUT8));
    gpio_add_callback(gpio0_dev, &but8_cb_data);
}


/*************************** End of file ****************************/

