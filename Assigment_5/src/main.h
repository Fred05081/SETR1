/** @file main.h
 * @brief Application to control the light intensity of a given region. The
system comprises a light sensor, an illumination system and a Human-Machine Interface. The
system can operate in two modes:
• Automatic: programmable via the terminal. Should allow setting On/Off periods and the
corresponding light intensity;
• Manual: interface via the DevKit buttons. Allows to turn the system On/Off (when in “Off”
the system operates in automatic mode), via two of the buttons. The other two buttons allow
to set (increase/decrease) the desired intensity
 *
 * @author Frederico Moreira, Ana Sousa, Pedro Rodrigues
 * @date 21 June 2022
 * @bug No known bugs.
 */

/**
 * @brief Main funtion: Initialize semaphores and configure GPIO_PIN (UART)
 *
 * @code
 * 
 void main(void) {
  int err=0;    


    printf("\n\r Illustration of the use of shmem + semaphores\n\r");
    int ret = 0;
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT1, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT2, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT4, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 1 \r", ret);
	return;
    }
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT1, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT2, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT4, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT1 pin \r", ret);
	return;
    }

    gpio_init_callback(&but1_cb_data, but1press_cbfunction, BIT(BOARDBUT1));
    gpio_add_callback(gpio0_dev, &but1_cb_data);

    gpio_init_callback(&but2_cb_data, but2press_cbfunction, BIT(BOARDBUT2));
    gpio_add_callback(gpio0_dev, &but2_cb_data);

    gpio_init_callback(&but4_cb_data, but4press_cbfunction, BIT(BOARDBUT4));
    gpio_add_callback(gpio0_dev, &but4_cb_data);
  
      
    err=0;
    uint8_t welcome_mesg[] = "UART demo: Type a few chars in a row and then pause for a little while ...\n\r"; 
    uint8_t rep_mesg[TXBUF_SIZE];

 
    
    k_sem_init(&sem_a1a, 0, 1);
    k_sem_init(&sem_ab, 0, 1);
    k_sem_init(&sem_bc, 0, 1);
     k_sem_init(&sem_bd, 0, 1);
    
    
     thread_A1_tid = k_thread_create(&thread_A1_data, thread_A1_stack,
        K_THREAD_STACK_SIZEOF(thread_A1_stack), thread_A1_code,
        NULL, NULL, NULL, thread_A1_prio, 0, K_NO_WAIT);

    thread_A_tid = k_thread_create(&thread_A_data, thread_A_stack,
        K_THREAD_STACK_SIZEOF(thread_A_stack), thread_A_code,
        NULL, NULL, NULL, thread_A_prio, 0, K_NO_WAIT);

    thread_B_tid = k_thread_create(&thread_B_data, thread_B_stack,
        K_THREAD_STACK_SIZEOF(thread_B_stack), thread_B_code,
        NULL, NULL, NULL, thread_B_prio, 0, K_NO_WAIT);

    thread_C_tid = k_thread_create(&thread_C_data, thread_C_stack,
        K_THREAD_STACK_SIZEOF(thread_C_stack), thread_C_code,
        NULL, NULL, NULL, thread_C_prio, 0, K_NO_WAIT);

    thread_D_tid = k_thread_create(&thread_D_data, thread_D_stack,
        K_THREAD_STACK_SIZEOF(thread_D_stack), thread_D_code,
        NULL, NULL, NULL, thread_D_prio, 0, K_NO_WAIT);

     
    return;
}     
 * @endcode
 * @param NO_args without arguments
 * @return No returns
 */
void main(void); 

/**
 * @brief É o valor da ADC que é guardado numa variável global (shared memory between tasks A/B) no nosso Código denominada por “ab” e no final faz give do semáforo AB.
 * @code
 * 
void thread_A_code(void *argA , void *argB, void *argC)
{
    int err=0;
    
    printk("Thread A init\n");
       
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
        k_sem_take(&sem_a1a,  K_FOREVER);
       
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

        printk("Thread A set ab value to: %d ",ab);  
        
        k_sem_give(&sem_ab);
       
       

    }
}

 * @endcode
 * @param arg3 void *argA , void *argB, void *argC.
 * @return No returns  
 */
void thread_A_code(void *argA , void *argB, void *argC);

/**
 * @brief É o valor da ADC que é guardado numa variável global (shared memory between tasks A/B) no nosso Código denominada por “ab” e no final faz give do semáforo AB.
 * @code
 * 
void thread_A1_code(void *argA , void *argB, void *argC)
{
    
    int64_t fin_time=0, release_time=0;

    int err=0;

    printk("Thread A1 init (periodic)\n");

    
    release_time = k_uptime_get() + SAMP_PERIOD_MS;

    while(1) {
        
  
        if (flag_flag==0){
         printk("Modo manual\n");
         printk("Modo key 2 e 4\n");
         k_sem_give(&sem_bc);}
        
        else{
       
           printk("Modo automatico\n");
 
          k_sem_give(&sem_a1a);
          }

        fin_time = k_uptime_get();
        if( fin_time < release_time) {        
            k_msleep(release_time - fin_time);            
            release_time += SAMP_PERIOD_MS;
        }
    }
    
}

 * @endcode
 * @param arg3 void *argA , void *argB, void *argC.
 * @return No returns  
 */
void thread_A1_code(void *argA , void *argB, void *argC);

/**
 * @brief é feito o take do semáforo AB é realizado uma média das últimas 10 amostras calculadas na thread A e é feito um filtro rejeitando todos os valores que estejam abaixo ou acima de 10% da média, sendo que este output é colocado numa variável global (shared memory between tasks B/C) no nosso Código denominada por “cb” e no final faz give do semáforo BC.
 * 
 * @code
 *
void thread_B_code(void *argA , void *argB, void *argC)
{    

    int Array_dados[len_dados]={0};
    int k=0;

    printk("Thread B init (sporadic, waits on a semaphore by task A)\n");
    while(1) {

        int sumador=0,somador_2=0,media=0, media_filtered=0;
        int contador=0;
       
        k_sem_take(&sem_ab,  K_FOREVER);
        
        printk("Task B read ab value: %d\n",ab);

        for(int k=len_dados-1; k>0;k--){
        
        Array_dados[k]= Array_dados[k-1];
        }
        Array_dados[0]= ab;
       
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

        bd=ab;
        printk("Thread B set bc value to: %d\n",bc);  
        k_sem_give(&sem_bd);        
  }
}

 * @endcode
 * 
 * @param arg3 void *argA , void *argB, void *argC.
 * @return No returns
 */

void thread_B_code(void *argA , void *argB, void *argC);

/**
 * @brief 
 * @code
 *
 void thread_C_code(void *argA , void *argB, void *argC)
{
    const struct device *gpio0_dev;        
    const struct device *pwm0_dev;          
    int ret=0;                              
    unsigned int dcValue[]={100,90,80,70,60,50,40,30,20,10,0};   
    unsigned int dcIndex=0;   
    unsigned int pwmPeriod_us = 100;       

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
        

        if(Flag_2) {
            dcIndex++;
            if(dcIndex == 11) 
                dcIndex = 0;
            Flag_2 = 0;
            printk("PWM DC value set to %u %%\n\r",dcValue[dcIndex]);

            ret = pwm_pin_set_usec(pwm0_dev, BOARDLED1,
		      pwmPeriod_us,(unsigned int)((pwmPeriod_us*dcValue[dcIndex])/100), PWM_POLARITY_NORMAL);
            if (ret) {
                printk("Error %d: failed to set pulse width\n", ret);
		return;
            }
        } 
        if(Flag_4) {
            
            if(dcIndex == 0) 
                dcIndex = 11;
            dcIndex--;
            Flag_4 = 0;
            printk("PWM DC value set to %u %%\n\r",dcValue[dcIndex]);

            ret = pwm_pin_set_usec(pwm0_dev, BOARDLED1,
		      pwmPeriod_us,(unsigned int)((pwmPeriod_us*dcValue[dcIndex])/100), PWM_POLARITY_NORMAL);
            if (ret) {
                printk("Error %d: failed to set pulse width\n", ret);
		return;
            }
        }
                       
        printk("Task C - PWM: %u % n", (unsigned int)(((pwmPeriod_us*bc)/1023)/10)); 
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
 *
 void thread_D_code(void *argA , void *argB, void *argC)
{
    const struct device *gpio0_dev;        
    const struct device *pwm0_dev;          
    int ret=0;                              
    unsigned int dcValue[]={100,90,80,70,60,50,40,30,20,10,0};   
    unsigned int dcIndex=0;   
    unsigned int pwmPeriod_us = 100;       

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
        k_sem_take(&sem_bd, K_FOREVER);
        printk("Valor lido para automatico %d\n r",bd);
        ret=0;
        

        if(bd<500) {
            
            ret = pwm_pin_set_usec(pwm0_dev, BOARDLED1,
		      pwmPeriod_us,(unsigned int)(pwmPeriod_us), PWM_POLARITY_NORMAL);
            if (ret) {
                printk("Error %d: failed to set pulse width\n", ret);
		return;
            }
        } 
        else {
            
            ret = pwm_pin_set_usec(pwm0_dev, BOARDLED1,
		      pwmPeriod_us,(unsigned int)(0), PWM_POLARITY_NORMAL);
            if (ret) {
                printk("Error %d: failed to set pulse width\n", ret);
		return;
            }
        } 
        
 }
 }
 
 * @endcode
 * @param NO_args without arguments
 * @param arg3 void *argA , void *argB, void *argC.
 */
void thread_D_code(void *argA , void *argB, void *argC)
/**
 * @brief 
 * @code
 *
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
            printk("adc_sample(): error, must bind to adc first \r");
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

/**
 * @brief If button 1 is pressed, Update Flag 1
 *
 * 
 * @code
 * 
  void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{ 
    Flag_1 = 1;
    flag_flag=!flag_flag;
    printk("but1 \n\r");
}
 * @endcode
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns
 */

void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/**
 * @brief If button 2 is pressed, Update Flag 2
 *
 * 
 * @code
 *  
 void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{   
    Flag_2 = 1;
}
 * @endcode
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns
 */

void but2press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/**
 * @brief If button 4 is pressed, Update Flag 4
 *
 * 
 * @code
 *  
 void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{ 
    Flag_4 = 1;
}
 * @endcode
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns
 */

void but4press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);