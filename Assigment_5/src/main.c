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
#include <drivers/uart.h>
/** Number of samples for the average*/
#define len_dados 10

/** Size of stack area used by each thread (can be thread specific, if necessary)*/
#define STACK_SIZE 1024

/** Thread scheduling priority */
#define thread_A1_prio 1
#define thread_A_prio 1
#define thread_B_prio 1
#define thread_C_prio 1
#define thread_D_prio 1

/** Therad periodicity (in ms)*/
#define SAMP_PERIOD_MS 1000
/** ADC definitions and includes*/

#include <hal/nrf_saadc.h>
#define ADC_NID DT_NODELABEL(adc) 
#define ADC_RESOLUTION 10
#define ADC_GAIN ADC_GAIN_1_4
#define ADC_REFERENCE ADC_REF_VDD_1_4
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 40)
#define ADC_CHANNEL_ID 1  

#define ADC_CHANNEL_INPUT NRF_SAADC_INPUT_AIN1        

#define BUFFER_SIZE 1

/** Refer to dts file */
#define GPIO0_NID DT_NODELABEL(gpio0) 
#define PWM0_NID DT_NODELABEL(pwm0) 
#define BOARDLED1 0x0d   

#define BOARDBUT1 0xb /* Pin at which BUT1 is connected. Addressing is direct (i.e., pin number) */      
#define BOARDBUT2 0xc        
#define BOARDBUT2 0xc
#define BOARDBUT4 0x19

/** Create thread stack space */
K_THREAD_STACK_DEFINE(thread_A1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_A_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_B_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_C_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_D_stack, STACK_SIZE);
  
/** Create variables for thread data */
struct k_thread thread_A1_data;
struct k_thread thread_A_data;
struct k_thread thread_B_data;
struct k_thread thread_C_data;
struct k_thread thread_D_data;

/** Create task IDs */
k_tid_t thread_A1_tid;
k_tid_t thread_A_tid;
k_tid_t thread_B_tid;
k_tid_t thread_C_tid;
k_tid_t thread_D_tid;

/** Global vars (shared memory between tasks A/B and B/C, resp) */
int a1a=0;
int ab = 0;
int bc = 0;
int bd = 0;

/** Semaphores for task synch */
struct k_sem sem_a1a;
struct k_sem sem_ab;
struct k_sem sem_bc;
struct k_sem sem_bd;

/** Thread code prototypes */
void thread_A1_code(void *argA, void *argB, void *argC);
void thread_A_code(void *argA, void *argB, void *argC);
void thread_B_code(void *argA, void *argB, void *argC);
void thread_C_code(void *argA, void *argB, void *argC);
void thread_D_code(void *argA, void *argB, void *argC);
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

static struct gpio_callback but1_cb_data;
static struct gpio_callback but3_cb_data;
static struct gpio_callback but2_cb_data;
static struct gpio_callback but4_cb_data;

volatile int Flag_1 = 0;
volatile int Flag_3 = 0;
volatile int Flag_2 = 0;
volatile int Flag_4 = 0;
volatile bool flag_flag = 0;

void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Flag_1 = 1;
    flag_flag=!flag_flag;
    printk("but1 \n\r");
}

void but2press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Flag_2 = 1;
}

void but4press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    /* Update Flag*/
    Flag_4 = 1;
}




const struct device *gpio0_dev;


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




#define FATAL_ERR -1 /* Fatal error return code, app terminates */

#define UART_NID DT_NODELABEL(uart0)    /* UART Node label, see dts */
#define RXBUF_SIZE 60                   /* RX buffer size */
#define TXBUF_SIZE 60                   /* TX buffer size */
#define RX_TIMEOUT 1000                  /* Inactivity period after the instant when last char was received that triggers an rx event (in us) */

/* Struct for UART configuration (if using default valuies is not needed) */
const struct uart_config uart_cfg = {
		.baudrate = 115200,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

/* UAR related variables */
const struct device *uart_dev;          /* Pointer to device struct */ 
static uint8_t rx_buf[RXBUF_SIZE];      /* RX buffer, to store received data */
static uint8_t rx_chars[RXBUF_SIZE];    /* chars actually received  */
volatile int uart_rx_rdy_flag;          /* Flag to signal main() that a message is available */

/* UART callback function prototype */
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data);

#define MAIN_SLEEP_TIME_MS 10 /* Time between main() activations */ 
/** Main function */
uint8_t welcome_mesg[] = "UART demo: Type a few chars in a row and then pause for a little while ...\n\r"; 
uint8_t rep_mesg[TXBUF_SIZE];
void main(void) {
int err=0;    


     gpio0_dev = device_get_binding(DT_LABEL(GPIO0_NID));
    /** Welcome message */
    printf("\n\r Illustration of the use of shmem + semaphores\n\r");
    int ret = 0;
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT1, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT2, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT4, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 1 \n\r", ret);
	return;
    }
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT1, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT2, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT4, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT1 pin \n\r", ret);
	return;
    }

    gpio_init_callback(&but1_cb_data, but1press_cbfunction, BIT(BOARDBUT1));
    gpio_add_callback(gpio0_dev, &but1_cb_data);

    gpio_init_callback(&but2_cb_data, but2press_cbfunction, BIT(BOARDBUT2));
    gpio_add_callback(gpio0_dev, &but2_cb_data);

    gpio_init_callback(&but4_cb_data, but4press_cbfunction, BIT(BOARDBUT4));
    gpio_add_callback(gpio0_dev, &but4_cb_data);
  
  /* Local vars */    
    err=0; /* Generic error variable */
    uint8_t welcome_mesg[] = "UART demo: Type a few chars in a row and then pause for a little while ...\n\r"; 
    uint8_t rep_mesg[TXBUF_SIZE];

 
    /** Create and init semaphores */
    k_sem_init(&sem_a1a, 0, 1);
    k_sem_init(&sem_ab, 0, 1);
    k_sem_init(&sem_bc, 0, 1);
     k_sem_init(&sem_bd, 0, 1);
    
    /** Create tasks */
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

        printk("Thread A set ab value to: %d \n",ab);  
        
        k_sem_give(&sem_ab);
       
       

    }
}

void thread_B_code(void *argA , void *argB, void *argC)
{    
    /** array de dados da adc*/
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
                       
        printk("Task C - PWM: %u % \n", (unsigned int)(((pwmPeriod_us*bc)/1023)/10));   /** Prints dutty-cycle*/
    }
}

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
        printk("Valor lido para automatico %d\n\r",bd);
        ret=0;
        

        if(bd<500) {
            
            ret = pwm_pin_set_usec(pwm0_dev, BOARDLED1,
		      pwmPeriod_us,(unsigned int)(pwmPeriod_us), PWM_POLARITY_NORMAL);
            if (ret) {
                printk("Error %d: failed to set pulse width\n", ret);
		return;
            }
        } 
        else if(bd>500 && bd<900) {
            
            ret = pwm_pin_set_usec(pwm0_dev, BOARDLED1,
		      pwmPeriod_us,(unsigned int)(pwmPeriod_us*0.5), PWM_POLARITY_NORMAL);
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
 
   
    


