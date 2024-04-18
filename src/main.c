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

#define PUSH_BUTTON_1 GPIO_NUM_25
#define PUSH_BUTTON_2 GPIO_NUM_26
#define PUSH_BUTTON_3 GPIO_NUM_27

#define PUSH_BUTTON_1_MASK (1ULL << PUSH_BUTTON_1)
#define PUSH_BUTTON_2_MASK (1ULL << PUSH_BUTTON_2)
#define PUSH_BUTTON_3_MASK (1ULL << PUSH_BUTTON_3)

#define NUM_OF_DATA 15


gpio_config_t LED_GPIOConfig;     // struct to define GPIO config
gpio_config_t PB_GPIOConfig;

void LED_switch(int *_LED1, int *_LED2, int *_LED3, bool _LED1_state, bool _LED2_state, bool _LED3_state);
void config_gpio(void);

void Led1_task(void *parameter);
void Led2_task(void *parameter);
void Led3_task(void *parameter);

void Send_to_led1(void *parameters);
void Send_to_led2(void *parameters);
void Send_to_led3(void *parameters);

// create an array to pass values to the LED strips. Each value corressponds to the number of blinks to be made.
int blink_led1_values[NUM_OF_DATA] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int blink_led2_values[NUM_OF_DATA] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int blink_led3_values[NUM_OF_DATA] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

QueueHandle_t myQueue1;
QueueHandle_t myQueue2;
QueueHandle_t myQueue3;


const TickType_t xDelay = (500 / portTICK_PERIOD_MS);
const TickType_t xDelay2 = (500 / portTICK_PERIOD_MS);
const TickType_t xDelay3 = (500 / portTICK_PERIOD_MS);


void app_main() {
    
    config_gpio();

    myQueue1 = xQueueCreate(10, sizeof(int));
    myQueue2 = xQueueCreate(10, sizeof(int));
    myQueue3 = xQueueCreate(10, sizeof(int));

    xTaskCreate(Led1_task, "led1_task", 1048, NULL, 1, NULL);
    xTaskCreate(Led2_task, "led2_task", 1048, NULL, 1, NULL);
    xTaskCreate(Led3_task, "led3_task", 1048, NULL, 1, NULL);
    
    xTaskCreate(Send_to_led1, "send command to led1", 1048, NULL, 2, NULL);
    xTaskCreate(Send_to_led2, "send command to led2", 1048, NULL, 2, NULL);
    xTaskCreate(Send_to_led3, "send command to led3", 1048, NULL, 2, NULL);

    while (1)
    {
        // Empty
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
    PB_GPIOConfig.pin_bit_mask  = (PUSH_BUTTON_1_MASK | PUSH_BUTTON_2_MASK | PUSH_BUTTON_3_MASK);
    PB_GPIOConfig.mode          = GPIO_MODE_INPUT;
    PB_GPIOConfig.pull_up_en    = GPIO_PULLUP_ENABLE;
    PB_GPIOConfig.pull_down_en  = GPIO_PULLDOWN_DISABLE;
    PB_GPIOConfig.intr_type     = GPIO_INTR_DISABLE;
    gpio_config(&PB_GPIOConfig);

    // gpio_set_direction(LED1_RED, GPIO_MODE_OUTPUT);
    // gpio_set_direction(LED3_GREEN, GPIO_MODE_OUTPUT);
}


void Led1_task(void *parameter){
    int led_blink = 0;
    int i;

    while (1)
    {
        if (xQueueReceive(myQueue1, &led_blink, 0) == pdTRUE)
        {
            for (i = 0; i < led_blink; i++)
            {
                gpio_set_level(LED1_RED, 1);
                vTaskDelay(xDelay);
                gpio_set_level(LED1_RED, 0);
                vTaskDelay(xDelay);
                // gpio_set_level(LED1_RED, 1);
                // vTaskDelay(xDelay);
                // gpio_set_level(LED1_RED, 0);
                // vTaskDelay(xDelay);
                // gpio_set_level(LED1_RED, 1);
                // vTaskDelay(xDelay);
                // gpio_set_level(LED1_RED, 0);
                // vTaskDelay(xDelay);
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
        if (xQueueReceive(myQueue2, &led_blink, 0) == pdTRUE)
        {
            for (i = 0; i < led_blink; i++)
            {
                gpio_set_level(LED2_ORANGE, 1);
                vTaskDelay(xDelay2);
                gpio_set_level(LED2_ORANGE, 0);
                vTaskDelay(xDelay2);
                // gpio_set_level(LED2_ORANGE, 1);
                // vTaskDelay(xDelay);
                // gpio_set_level(LED2_ORANGE, 0);
                // vTaskDelay(xDelay);
                // gpio_set_level(LED2_ORANGE, 1);
                // vTaskDelay(xDelay);
                // gpio_set_level(LED2_ORANGE, 0);
                // vTaskDelay(xDelay);
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
        if (xQueueReceive(myQueue3, &led_blink, 0) == pdTRUE)
        {
            for (i = 0; i < led_blink; i++)
            {
                gpio_set_level(LED3_GREEN, 1);
                vTaskDelay(xDelay3);
                gpio_set_level(LED3_GREEN, 0);
                vTaskDelay(xDelay3);
                // gpio_set_level(LED3_GREEN, 1);
                // vTaskDelay(xDelay);
                // gpio_set_level(LED3_GREEN, 0);
                // vTaskDelay(xDelay);
                // gpio_set_level(LED3_GREEN, 1);
                // vTaskDelay(xDelay);
                // gpio_set_level(LED3_GREEN, 0);
                // vTaskDelay(xDelay);
            }
        }
        else
        {
            led_blink = 0;
        }
        
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


void Send_to_led1(void *parameters){
    int i = 0;

    while (1)
    {
        if (gpio_get_level(PUSH_BUTTON_1) == 0)
        {
            if (i < NUM_OF_DATA)
            {
                xQueueSend(myQueue1, &blink_led1_values[i], 10);
                vTaskDelay(200/portTICK_PERIOD_MS);     // Debounce delay
                i++;
            }else
            {
                i = 0;
            }
        }
        //vTaskDelay(10);
    }
}


void Send_to_led2(void *parameters){
    int i = 0;

    while (1)
    {
        if (gpio_get_level(PUSH_BUTTON_2) == 0)
        {
            if (i < NUM_OF_DATA)
            {
                xQueueSend(myQueue2, &blink_led2_values[i], 10);
                vTaskDelay(200/portTICK_PERIOD_MS);     // Debounce delay
                i++;
            }else
            {
                i = 0;
            }
        }
        //vTaskDelay(10);
    }
}


void Send_to_led3(void *parameters){
    int i = 0;

    while (1)
    {
        if (gpio_get_level(PUSH_BUTTON_3) == 0)
        {
            if (i < NUM_OF_DATA)
            {
                xQueueSend(myQueue3, &blink_led3_values[i], 10);
                vTaskDelay(200/portTICK_PERIOD_MS);     // Debounce delay
                i++;
            }else
            {
                i = 0;
            }
        }
        //vTaskDelay(10);
    }
}