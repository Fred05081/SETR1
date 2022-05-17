/** @file vending_machine.h
 * @brief The vending machine accepts a subset of coins and allows the user to browse available
products, buy one product and return the credit
 *
 * Contains the functions needed to create a vending machine.
 * @author Frederico Moreira, Ana Sousa, Pedro Rodrigues
 * @date 17 May 2022
 * @bug No known bugs.
 */

/**
 * @brief If button 1 is pressed, activate the 10 cents flag 
 *
 * 
 * @code
 * void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
 {    
    Flag_10_Cent = 1;
  }
 * @endcode
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns
 */

void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/**
 * @brief If button 2 is pressed, activate the 20 cents flag 
 *
 * @code
 * void but2press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    Flag_20_Cent = 1;
}
 * @endcode
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns
 */

void but2press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/**
 * @brief If button 3 is pressed, activate the 50 cents flag 
 *
 * @code
 * void but3press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
    {    
    Flag_50_Cent = 1;
    }
 * @endcode
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns
 */
void but3press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/**
 * @brief If button 4 is pressed, activate the 1 eur flag 
 * @code
 * 
void but4press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    Flag_100_Cent = 1;
}
 * @endcode
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns  
 */
void but4press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
/**
 * @brief If button 5 is pressed, activate the Up flag 
 * 
 * @code
 *void but5press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    Up_Flag = 1;
}
 * @endcode
 * 
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns
 */

void but5press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/**
 * @brief If button 6 is pressed, activate the Down flag 
 * @code
 *void but6press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    
    Down_Flag = 1;
}
 * @endcode
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns
 */

void but6press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/**
 * @brief If button 7 is pressed, activate the check_credit_flag 
 * 
 * @code
 *void but7press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    
    Check_credit_Flag = 1;
}
 * @endcode
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns
 */

void but7press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/**
 * @brief If button 8 is pressed, activate the return flag
 * 
 * @code
 *void but8press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{    
    Return_Flag = 1;  
}

 * @endcode
 * @param arg3 const struct device *dev, struct gpio_callback *cb, uint32_t pins.
 * @return No returns
 */

void but8press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

