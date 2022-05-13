/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/
/*
 * Paulo Pedreiras, 2022/02
 * Simple Digital Output example
 * 
 * Toggles periodically LED1, which is internally connected to P0.13 
 *
 * Base documentation:
 *        
 *      HW info:
 *          https://infocenter.nordicsemi.com/topic/struct_nrf52/struct/nrf52840.html
 *          Section: nRF52840 Product Specification -> Peripherals -> GPIO / GPIOTE
 * 
 *          Board specific HW info can be found in the nRF52840_DK_User_Guide_20201203. I/O pins available at pg 27
 *
 *      Peripherals:
 *          https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/reference/peripherals/gpio.html 
 * 
 * 
 */

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

#define BOARDLED1 0xd /* Pin at which LED is connected. Addressing is direct (i.e., pin number) */
#define BOARDLED2 0xe // pin at which LED is connected.
#define BOARDLED3 0xf
#define BOARDLED4 0x10 
/* Refer to dts file */
//#define PWM0_NID DT_NODELABEL(pwm0) 
#define BOARDBUT1 0xb /* Pin at which BUT1 is connected. Addressing is direct (i.e., pin number) */
#define BOARDBUT2 0xc
#define BOARDBUT3 0x18
#define BOARDBUT4 0x19
#define BOARDBUT5 0x03
#define BOARDBUT6 0x04
#define BOARDBUT7 28
#define BOARDBUT8 29

#define State_0 0
#define credit_return 1
#define check_credit_available 2
#define Up 3
#define Down 4

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
volatile int dcToggleFlag1 = 0; /* Flag to signal a BUT1 press */
volatile int dcToggleFlag2 = 0; /* Flag to signal a BUT1 press */
volatile int dcToggleFlag3 = 0; /* Flag to signal a BUT1 press */
volatile int dcToggleFlag4 = 0; /* Flag to signal a BUT1 press */
volatile int dcToggleFlag5 = 0; /* Flag to signal a BUT1 press */
volatile int dcToggleFlag6 = 0; /* Flag to signal a BUT1 press */
volatile int dcToggleFlag7 = 0; /* Flag to signal a BUT1 press */
volatile int dcToggleFlag8 = 0; /* Flag to signal a BUT1 press */

volatile int NS = 0;
volatile int product= 150;

void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Inform that button was hit*/
    printk("But1 pressed at %d\n\r", k_cycle_get_32());
    
    /* Update Flag*/
    dcToggleFlag1 = 1;
}

void but2press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Inform that button was hit*/
    printk("But2 pressed at %d\n\r", k_cycle_get_32());
    
    /* Update Flag*/
    dcToggleFlag2 = 1;
}

void but3press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Inform that button was hit*/
    printk("But3 pressed at %d\n\r", k_cycle_get_32());
    
    /* Update Flag*/
    dcToggleFlag3 = 1;
}

void but4press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Inform that button was hit*/
    printk("But4 pressed at %d\n\r", k_cycle_get_32());
    
    /* Update Flag*/
    dcToggleFlag4 = 1;
}
void but5press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Inform that button was hit*/
    printk("But5 pressed at %d\n\r", k_cycle_get_32());
    
    /* Update Flag*/
    dcToggleFlag5 = 1;
}

void but6press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Inform that button was hit*/
    printk("But6 pressed at %d\n\r", k_cycle_get_32());
    
    /* Update Flag*/
    dcToggleFlag6 = 1;
}

void but7press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Inform that button was hit*/
    printk("But7 pressed at %d\n\r", k_cycle_get_32());
    
    /* Update Flag*/
    dcToggleFlag7 = 1;
}

void but8press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Inform that button was hit*/
    printk("But8 pressed at %d\n\r", k_cycle_get_32());
    
    /* Update Flag*/
    dcToggleFlag8 = 1;
}

const struct device *gpio0_dev;         /* Pointer to GPIO device structure */

/* Main function */
void main(void) {

    /* Local vars */    
    int money=0;
    int product_Array[3]={150, 100, 50};
    
    /* Bind to GPIO 0 */
    gpio0_dev = device_get_binding(DT_LABEL(GPIO0_NID));
    if (gpio0_dev == NULL) {
        printk("Failed to bind to GPIO0\n\r");        
	return;
    }
    else {
        printk("Bind to GPIO0 successfull \n\r");        
    }


    /* Blink loop */
    CONFIG_LEDS();
    CONFIG_BUTTONS();

    while(1) {
    
    
    switch  (NS){
      case (State_0):

        if (dcToggleFlag1==1){
        money=money+10;
        dcToggleFlag1=0;
        printk("Total Money: %d.%d EUR\n\r",money/100, money%100);
        
        }
        
        if (dcToggleFlag2==1){
        money=money+20;
        dcToggleFlag2=0;
        printk("Total Money:  %d.%d EUR\n\r",money/100, money%100);
        }
       
        if (dcToggleFlag3==1){
        money=money+50;
        dcToggleFlag3=0;
        printk("Total Money:  %d.%d EUR\n\r",money/100, money%100);
        }
        
        if (dcToggleFlag4==1){
        money=money+100;
        dcToggleFlag4=0;
        printk("Total Money: %d.%d EUR\n\r",money/100, money%100);
        }
        if (dcToggleFlag8==1){
        NS=credit_return;
        dcToggleFlag8=0;
        }

        if(dcToggleFlag7==1){/* select product*/
        NS= check_credit_available;
        dcToggleFlag7=0;
        }

        if(dcToggleFlag5==1){/* select product*/
        NS= Up;
        dcToggleFlag5=0;
        }

        if(dcToggleFlag6==1){/* select product*/
        NS= Down;
        dcToggleFlag6=0;
        }
        break;

      case (credit_return):
        printk("Credit return: %d.%d EUR\n\r",money/100, money%100);
        money=0;
        NS=0;
        printk("Total Money:  %d.%d EUR\n\r",money/100, money%100);
        break;

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

      case(Up):
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
        case(Down):
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

      default:
        break;
      
      }
    
    
    
    
    }                          
        
    
    
    return;
} 

void CONFIG_LEDS()
{
    int ret = 0;
    ret = gpio_pin_configure(gpio0_dev, BOARDLED1, GPIO_OUTPUT_ACTIVE);
    ret = gpio_pin_configure(gpio0_dev, BOARDLED2, GPIO_OUTPUT_ACTIVE);
    ret = gpio_pin_configure(gpio0_dev, BOARDLED3, GPIO_OUTPUT_ACTIVE);
    ret = gpio_pin_configure(gpio0_dev, BOARDLED4, GPIO_OUTPUT_ACTIVE);

    if (ret < 0) {
        printk("gpio_pin_configure() failed with error %d\n\r", ret);        
	return;
    }    
}

void CONFIG_BUTTONS()
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
