#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED1_RED GPIO_NUM_2
#define LED2_ORANGE GPIO_NUM_4
#define LED3_GREEN GPIO_NUM_16

#define LED_GREEN_MASK (1ULL << LED3_GREEN)
#define LED_RED_MASK (1ULL << LED1_RED)
#define LED_ORANGE_MASK (1ULL << LED2_ORANGE)

#define PUSH_BUTTON_1 GPIO_NUM_12
#define PUSH_BUTTON_1_MASK (1ULL << PUSH_BUTTON_1)

#define NUM_OF_DATA 15


gpio_config_t LED_GPIOConfig;     // struct to define GPIO config
gpio_config_t PB_GPIOConfig;

void LED_switch(int *_LED1, int *_LED2, int *_LED3, bool _LED1_state, bool _LED2_state, bool _LED3_state);
void config_gpio(void);

void Led1_task(void *parameter);
void Led2_task(void *parameter);
void Led3_task(void *parameter);
void Send_to_led(void *parameters);

// create an array to pass values to the LED strips. Each value corressponds to the number of blinks to be made.
int blink_led_values[NUM_OF_DATA] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

QueueHandle_t myQueue;


const TickType_t xDelay = (1000 / portTICK_PERIOD_MS);


void app_main() {
    // int LED1 = LED1_RED;
    // int LED2 = LED2_ORANGE;
    // int LED3 = LED3_GREEN;
    
    config_gpio();

    myQueue = xQueueCreate(10, sizeof(int));

    xTaskCreate(Led1_task, "led1_task", 1048, NULL, 1, NULL);
    xTaskCreate(Led2_task, "led2_task", 1048, NULL, 1, NULL);
    xTaskCreate(Led3_task, "led3_task", 1048, NULL, 1, NULL);
    
    xTaskCreate(Send_to_led, "send delay to led", 1048, NULL, 2, NULL);


    while (1)
    {
        /*if (gpio_get_level(PUSH_BUTTON_1) == 0)
        {
            LED_switch(&LED1, &LED2, &LED3, 1, 0, 0);
            vTaskDelay(xDelay);
            LED_switch(&LED1, &LED2, &LED3, 0, 1, 0);
            vTaskDelay(xDelay);
            LED_switch(&LED1, &LED2, &LED3, 0, 0, 1);
            vTaskDelay(xDelay);
            LED_switch(&LED1, &LED2, &LED3, 0, 0, 0);
        }
        */
    }
    
}


void config_gpio(void){
    // Define LED GPIO CONFIG
    LED_GPIOConfig.pin_bit_mask   = (LED_RED_MASK | LED_ORANGE_MASK | LED_GREEN_MASK);       // GPIO pins that will use this config.
    LED_GPIOConfig.mode           = GPIO_MODE_OUTPUT;
    LED_GPIOConfig.pull_up_en     = GPIO_PULLUP_DISABLE;
    LED_GPIOConfig.pull_down_en   = GPIO_PULLDOWN_DISABLE;
    LED_GPIOConfig.intr_type      = GPIO_INTR_DISABLE;
    gpio_config(&LED_GPIOConfig);

    // Define Push-Button GPIO Config
    PB_GPIOConfig.pin_bit_mask  = (PUSH_BUTTON_1_MASK);             // Pin D12.
    PB_GPIOConfig.mode          = GPIO_MODE_INPUT;
    PB_GPIOConfig.pull_up_en    = GPIO_PULLUP_ENABLE;
    PB_GPIOConfig.pull_down_en  = GPIO_PULLDOWN_DISABLE;
    PB_GPIOConfig.intr_type     = GPIO_INTR_DISABLE;
    gpio_config(&PB_GPIOConfig);

    // gpio_set_direction(LED1_RED, GPIO_MODE_OUTPUT);
    // gpio_set_direction(LED3_GREEN, GPIO_MODE_OUTPUT);
}

/*
void LED_switch(int *_LED1, int *_LED2, int *_LED3, bool _LED1_state, bool _LED2_state, bool _LED3_state){
    gpio_set_level(*_LED1, _LED1_state);
    gpio_set_level(*_LED2, _LED2_state);
    gpio_set_level(*_LED3, _LED3_state);
}
*/

void Led1_task(void *parameter){
    int led_blink = 0;
    int i;

    while (1)
    {
        if (xQueueReceive(myQueue, &led_blink, 0) == pdTRUE)
        {
            for (i = 0; i < led_blink; i++)
            {
                gpio_set_level(LED1_RED, 1);
                vTaskDelay(xDelay);
                gpio_set_level(LED1_RED, 0);
                vTaskDelay(xDelay);
                gpio_set_level(LED1_RED, 1);
                vTaskDelay(xDelay);
                gpio_set_level(LED1_RED, 0);
                vTaskDelay(xDelay);
                gpio_set_level(LED1_RED, 1);
                vTaskDelay(xDelay);
                gpio_set_level(LED1_RED, 0);
                vTaskDelay(xDelay);
            }
        }
        else
        {
            led_blink = 0;
        }
        
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


void Led2_task(void *parameter){
    int led_blink = 0;
    int i;

    while (1)
    {
        if (xQueueReceive(myQueue, &led_blink, 0) == pdTRUE)
        {
            for (i = 0; i < led_blink; i++)
            {
                gpio_set_level(LED2_ORANGE, 1);
                vTaskDelay(xDelay);
                gpio_set_level(LED2_ORANGE, 0);
                vTaskDelay(xDelay);
                gpio_set_level(LED2_ORANGE, 1);
                vTaskDelay(xDelay);
                gpio_set_level(LED2_ORANGE, 0);
                vTaskDelay(xDelay);
                gpio_set_level(LED2_ORANGE, 1);
                vTaskDelay(xDelay);
                gpio_set_level(LED2_ORANGE, 0);
                vTaskDelay(xDelay);
            }
        }
        else
        {
            led_blink = 0;
        }
        
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


void Led3_task(void *parameter){
    int led_blink = 0;
    int i;

    while (1)
    {
        if (xQueueReceive(myQueue, &led_blink, 0) == pdTRUE)
        {
            for (i = 0; i < led_blink; i++)
            {
                gpio_set_level(LED3_GREEN, 1);
                vTaskDelay(xDelay);
                gpio_set_level(LED3_GREEN, 0);
                vTaskDelay(xDelay);
                gpio_set_level(LED3_GREEN, 1);
                vTaskDelay(xDelay);
                gpio_set_level(LED3_GREEN, 0);
                vTaskDelay(xDelay);
                gpio_set_level(LED3_GREEN, 1);
                vTaskDelay(xDelay);
                gpio_set_level(LED3_GREEN, 0);
                vTaskDelay(xDelay);
            }
        }
        else
        {
            led_blink = 0;
        }
        
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


void Send_to_led(void *parameters){
    int i = 0;

    while (1)
    {
        if (gpio_get_level(PUSH_BUTTON_1) == 0)
        {
            if (i < NUM_OF_DATA)
            {
                xQueueSend(myQueue, &blink_led_values[i], 10);
                vTaskDelay(300/portTICK_PERIOD_MS);     // Debounce delay
                i++;
            }else
            {
                i = 0;
            }
        }
        //vTaskDelay(10);
    }
    
}