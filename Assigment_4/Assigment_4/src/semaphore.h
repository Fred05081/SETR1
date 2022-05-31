/** @file semaphore.h
 * @brief The system to implement does a basic processing of an analog signal. It reads the input voltage
from an analog sensor, digitally filters the signal and outputs it using a semaphore.
 *
 * Contains the functions needed to process the analog signal
 * @author Frederico Moreira, Ana Sousa, Pedro Rodrigues
 * @date 31 May 2022
 * @bug No known bugs.
 */

/**
 * @brief Main funtion: Initialize semaphores
 *
 * @code
 * void main(void) {
    printf("\n\r Illustration of the use of shmem + semaphores\n\r");
    
    k_sem_init(&sem_ab, 0, 1);
    k_sem_init(&sem_bc, 0, 1);
    
 
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
    
 * @endcode
 * @param NO_args without arguments
 * @return No returns
 */
void main(void); 

/**
 * @brief Read the adc value and save it.
 * @code
 * 
void thread_A_code(void *argA , void *argB, void *argC)
{
    
    int64_t fin_time=0, release_time=0;

    int err=0;
    
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
    NRF_SAADC->TASKS_CALIBRATEOFFSET = 1;

    while(1) {
        
        err=adc_sample();
        if(err) {
            printk("adc_sample() failed with error code %d\n\r",err);
        }
        else {
            if(adc_sample_buffer[0] > 1023) {
                printk("adc reading out of range\n\r");
            }
            else {
                
                ab=adc_sample_buffer[0];
            }
        }

        printk("Thread A set ab value to: %d \n",ab);  
        
        k_sem_give(&sem_ab);
       
      
        fin_time = k_uptime_get();
        if( fin_time < release_time) {        
            k_msleep(release_time - fin_time);            
            release_time += thread_A_period;
        }
    }
}
 * @endcode
 * @param arg3 void *argA , void *argB, void *argC.
 * @return No returns  
 */
void thread_A_code(void *argA , void *argB, void *argC);
/**
 * @brief calculates the average of 10 values read from the adc and if the value is outside 10% it is rejected.
 * 
 * @code
 *void thread_B_code(void *argA , void *argB, void *argC)
{    

    int Array_dados[len_dados]={0};
    int k=0;

    printk("Thread B init (sporadic, waits on a semaphore by task A)\n");
    while(1) {

        int sumador=0,somador_2=0,media=0, media_filtered=0;
        int contador=0;
       
        k_sem_take(&sem_ab,  K_FOREVER);
        
        printk("Task B read ab value: %d\n",ab);

        Array_dados[0]= ab;
        Array_dados[(k+1)%10]= Array_dados[(k)%10];
        k=k+1;
    
       
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

        bc=media;
        printk("Thread B set bc value to: %d\n",bc);  
        k_sem_give(&sem_bc);        
  }
}
 * @endcode
 * 
 * @param arg3 void *argA , void *argB, void *argC.
 * @return No returns
 */

void thread_B_code(void *argA , void *argB, void *argC);

/**
 * @brief Sets the PWM DC value to the average of the samples got from ADC module in thread A*
 * @code
 *void thread_C_code(void *argA , void *argB, void *argC)
{
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
        k_sem_take(&sem_bc, K_FOREVER);
        ret=0;

       
        ret = pwm_pin_set_usec(pwm0_dev, BOARDLED1,
		      pwmPeriod_us,(unsigned int)((pwmPeriod_us*bc)/1023), PWM_POLARITY_NORMAL);
        if (ret) {
            printk("Error %d: failed to set pulse width\n", ret);
            return;
        }
                       
        printk("Task C - PWM: %u % \n", (unsigned int)(((pwmPeriod_us*bc)/1023)/10));  
    }
}
 * @endcode
 * @param arg3 void *argA , void *argB, void *argC.
 * @return No returns
 */

void thread_C_code(void *argA , void *argB, void *argC);

/**
 * @brief Takes one adc_sample
 * @code
 *static int adc_sample(void)
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

 * @endcode
 * @param NO_args without arguments
 * @return Read ADC_sample value (static int)
 */
static int adc_sample(void);
