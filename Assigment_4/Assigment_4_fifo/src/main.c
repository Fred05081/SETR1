/*
 *
 * Zephyr: Simple thread creation example (3)
 * 
 * One of the tasks is periodc, the other two synchronzie via a fifo 
 * 
 * Base documentation:
 *      https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/reference/kernel/index.html
 * 
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/adc.h>
#include <drivers/pwm.h>
#include <sys/printk.h>
#include <sys/__assert.h>
#include <string.h>
#include <timing/timing.h>
#include <stdlib.h>
#include <stdio.h>

/** Number of samples for the average*/
#define len_dados 10

/* Size of stack area used by each thread (can be thread specific, if necessary)*/
#define STACK_SIZE 1024

/* Thread scheduling priority */
#define thread_A_prio 1
#define thread_B_prio 1
#define thread_C_prio 1

/* Therad periodicity (in ms)*/
#define thread_A_period 1000

/** ADC definitions and includes*/
#include <hal/nrf_saadc.h>
#define ADC_NID DT_NODELABEL(adc) 
#define ADC_RESOLUTION 10
#define ADC_GAIN ADC_GAIN_1_4
#define ADC_REFERENCE ADC_REF_VDD_1_4
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 40)
#define ADC_CHANNEL_ID 1  

#define ADC_CHANNEL_INPUT NRF_SAADC_INPUT_AIN1          /** Analog 1 - Port P0.03 */

#define BUFFER_SIZE 1

/** Refer to dts file */
#define GPIO0_NID DT_NODELABEL(gpio0) 
#define PWM0_NID DT_NODELABEL(pwm0) 
#define BOARDLED1 0x0d                                  /** LED 1 */
                

/* Create thread stack space */
K_THREAD_STACK_DEFINE(thread_A_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_B_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_C_stack, STACK_SIZE);
  
/* Create variables for thread data */
struct k_thread thread_A_data;
struct k_thread thread_B_data;
struct k_thread thread_C_data;

/* Create task IDs */
k_tid_t thread_A_tid;
k_tid_t thread_B_tid;
k_tid_t thread_C_tid;

/* Create fifos*/
struct k_fifo fifo_ab;
struct k_fifo fifo_bc;

/* Create fifo data structure and variables */
struct data_item_t {
    void *fifo_reserved;    /* 1st word reserved for use by FIFO */
    uint16_t data;          /* Actual data */
};

/* Thread code prototypes */
void thread_A_code(void *, void *, void *);
void thread_B_code(void *, void *, void *);
void thread_C_code(void *, void *, void *);


/** ADC channel configuration */
static const struct adc_channel_cfg my_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_CHANNEL_ID,
	.input_positive = ADC_CHANNEL_INPUT
};

/** Global vars */
struct k_timer my_timer;
const struct device *adc_dev = NULL;
static uint16_t adc_sample_buffer[BUFFER_SIZE];

/** Takes one sample */
static int adc_sample(void)
{
	int ret;
	const struct adc_sequence sequence = {
		.channels = BIT(ADC_CHANNEL_ID),
		.buffer = adc_sample_buffer,
		.buffer_size = sizeof(adc_sample_buffer),
		.resolution = ADC_RESOLUTION,
	};

	if (adc_dev == NULL) {
            printk("adc_sample(): error, must bind to adc first \n\r");
            return -1;
	}

	ret = adc_read(adc_dev, &sequence);
	if (ret) {
            printk("adc_read() failed with code %d\n", ret);
	}	

	return ret;
}

/* Main function */
void main(void) {

    
     printk("\n\r IPC via FIFO example \n\r");
    
    
    k_fifo_init(&fifo_ab);
    k_fifo_init(&fifo_bc);
        
    
    thread_A_tid = k_thread_create(&thread_A_data, thread_A_stack,
        K_THREAD_STACK_SIZEOF(thread_A_stack), thread_A_code,
        NULL, NULL, NULL, thread_A_prio, 0, K_NO_WAIT);

    thread_B_tid = k_thread_create(&thread_B_data, thread_B_stack,
        K_THREAD_STACK_SIZEOF(thread_B_stack), thread_B_code,
        NULL, NULL, NULL, thread_B_prio, 0, K_NO_WAIT);

    thread_B_tid = k_thread_create(&thread_C_data, thread_C_stack,
        K_THREAD_STACK_SIZEOF(thread_C_stack), thread_C_code,
        NULL, NULL, NULL, thread_C_prio, 0, K_NO_WAIT);

    
    return;

} 

/* Thread code implementation */
void thread_A_code(void *argA , void *argB, void *argC)
{
    
    int64_t fin_time=0, release_time=0;

   
    long int nact = 0;
    int err=0;
    struct data_item_t data_ab;
    
    printk("Thread A init (periodic)\n");

    release_time = k_uptime_get() + thread_A_period;

    
    adc_dev = device_get_binding(DT_LABEL(ADC_NID));
    if (!adc_dev) {
        printk("ADC device_get_binding() failed\n");
    } 
    err = adc_channel_setup(adc_dev, &my_channel_cfg);
    if (err) {
        printk("adc_channel_setup() failed with error code %d\n", err);
    }
    
    /* Thread loop */
    while(1) {
        
                
        printk("\n\nThread A instance %ld released at time: %lld (ms). \n",++nact, k_uptime_get());  

   
        err=adc_sample();
        if(err) {
            printk("adc_sample() failed with error code %d\n\r",err);
        }
        else {
            if(adc_sample_buffer[0] > 1023) {
                printk("adc reading out of range\n\r");
            }
            else {
                
                data_ab.data = adc_sample_buffer[0];
                
            }
        }
        k_fifo_put(&fifo_ab, &data_ab);
        printk("Thread A data in fifo_ab: %d\n",data_ab.data);  
      
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += thread_A_period;

        }
    }

}

void thread_B_code(void *argA , void *argB, void *argC)
{
   /* Local variables */
    int Array_dados[len_dados]={0};
    int k=0; 
    struct data_item_t *data_ab;
    struct data_item_t data_bc;
    

    printk("Thread B init (sporadic, waits on a semaphore by task A)\n");

    while(1) {
        int sumador=0,somador_2=0,media=0, media_filtered=0;
        int contador=0;
        data_ab = k_fifo_get(&fifo_ab, K_FOREVER);
      
 
        for(int k=len_dados-1; k>0;k--){
        
        Array_dados[k]= Array_dados[k-1];
        }
        Array_dados[0]=data_ab->data ;
        
  
       
       for(int i = 0; i < len_dados; i++){
            if(Array_dados[i] != 0){
                sumador = sumador + Array_dados[i];
            }
        }
        media=sumador/len_dados;
        contador=0;
              
        for(int j = 0; j < len_dados; j++){
            if(Array_dados[j] < (media - media*0.1) || Array_dados[j] > (media + media*0.1))
                somador_2=somador_2;
            else{
                somador_2 = somador_2 + Array_dados[j];
                contador =contador +1;   
            }
        }
        if(somador_2 != 0)
            media_filtered=somador_2/contador;
        else 
            media_filtered = 0;

        data_bc.data=media_filtered;
        k_fifo_put(&fifo_bc, &data_bc);
        printk("Thread B set fifo bc value to: %d \n",data_bc.data);     
               
  }
}    
   

void thread_C_code(void *argA , void *argB, void *argC)
{
    /* Local variables */
    long int nact = 0;
    struct data_item_t *data_bc;

    printk("Thread C init (sporadic, waits on a semaphore by task A)\n");


    const struct device *gpio0_dev;         
    const struct device *pwm0_dev;          
    int ret=0;                              
    
    unsigned int pwmPeriod_us = 1000;       

    printk("Thread C init (sporadic, waits on a semaphore by task B)\n");
    
   
    gpio0_dev = device_get_binding(DT_LABEL(GPIO0_NID));
    if (gpio0_dev == NULL) {
        printk("Error: Failed to bind to GPIO0\n\r");        
	return;
    }
        
    pwm0_dev = device_get_binding(DT_LABEL(PWM0_NID));
    if (pwm0_dev == NULL) {
	printk("Error: Failed to bind to PWM0\n r");
	return;
    }

    while(1) {
        data_bc = k_fifo_get(&fifo_bc, K_FOREVER);
        printk("Thread C instance %5ld released at time: %lld (ms). \n",++nact, k_uptime_get());          
        printk("Task C read bc value: %d\n",data_bc->data);
        ret=0;

      
        ret = pwm_pin_set_usec(pwm0_dev, BOARDLED1,
		      pwmPeriod_us,(unsigned int)((pwmPeriod_us*data_bc->data)/1023), PWM_POLARITY_NORMAL);
        if (ret) {
            printk("Error %d: failed to set pulse width\n", ret);
            return;
        }
                       
        printk("Task C - PWM: %u % \n", (unsigned int)(((pwmPeriod_us*data_bc->data)/1023)/10));   
            
  }
}

