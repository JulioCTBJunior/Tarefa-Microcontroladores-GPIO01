#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define ROWS 4
#define COLS 4
uint8_t row_pins[ROWS] = {16, 17, 28, 18};  
uint8_t col_pins[COLS] = {19, 20, 4, 9};  


#define LED_GREEN 11
#define LED_BLUE 12  
#define LED_RED 13                         
#define BUZZER_PIN 21

const char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void init_gpio() {
   
    for (int i = 0; i < ROWS; i++) {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_OUT);
        gpio_put(row_pins[i], 1);
    }

    for (int i = 0; i < COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
        gpio_pull_up(col_pins[i]);
    }

    uint8_t led_pins[] = {LED_RED, LED_GREEN, LED_BLUE};
    for (int i = 0; i < 3; i++) {
        gpio_init(led_pins[i]);
        gpio_set_dir(led_pins[i], GPIO_OUT);
        gpio_put(led_pins[i], 0);
    }

    
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0); 
}

char scan_keypad() {
    for (int row = 0; row < ROWS; row++) {
        gpio_put(row_pins[row], 0); 
        for (int col = 0; col < COLS; col++) {
            if (gpio_get(col_pins[col]) == 0) { 
                gpio_put(row_pins[row], 1);
                return keys[row][col]; 
            }
        }
        gpio_put(row_pins[row], 1); 
    }
    return 0; 
}

void handle_key_press(char key) {
    switch (key) {
        case '1':
             // Acende o LED vermelho
            
        case '2':
             // Apaga o LED vermelho
            
        case '3':
             gpio_put(LED_BLUE, 1); // acendendo LED Azul
             break;            
        case '4':
             gpio_put(LED_BLUE, 0); // acendendo LED Azul
             break;         
        case '5':
             // Acende o LED verde
            
        case '6':
             // Apaga o LED verde
            
        case '7':
             // Liga o buzzer
            
        case '8':
             // Desliga o buzzer
            

        case '#':
            gpio_put(LED_RED, 1);
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_BLUE, 1);
            gpio_put(BUZZER_PIN, 1); // Acende todos os LEDs e liga o buzzer
            break;
          
        case 'A':
            // Acende o LED vermelho enquanto pressionado
            
        case 'B':
             // Acende o LED verde enquanto pressionado
            
        case 'C':
             gpio_put(LED_BLUE, 1); // acendendo LED Azul
             break;
        case 'D':
            gpio_put(LED_RED, 1);
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_BLUE, 1);// Acende todos enquanto pressionado
            break;
        default:
            printf("Tecla não atribuída: %c\n", key);
            break;
    }
}

void handle_key_release(char key) {
    switch (key) {
        case 'A':
             // Apaga o LED vermelho ao soltar
            
        case 'B':
             // Apaga o LED verde ao soltar
            
        case 'C':
            gpio_put(LED_BLUE, 0); // acendendo LED Azul
            break;            
        case 'D':
            gpio_put(LED_RED, 0);
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_BLUE, 0); // Apaga todos ao soltar
            break;
        case '*':
            gpio_put(LED_RED, 0);
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_BLUE, 0);
            gpio_put(BUZZER_PIN, 0);// Apaga todos os LEDs e desliga o buzzer
            break;
        default:
            break;
    }
}

int main() {
    stdio_init_all();
    init_gpio();

    char last_key = 0; // Variável para armazenar a última tecla pressionada

    while (1) {
        char key = scan_keypad();

        if (key && key != last_key) { 
            printf("Tecla pressionada: %c\n", key);
            handle_key_press(key);
            last_key = key;
        }

        if (!key && last_key) { 
            handle_key_release(last_key);
            last_key = 0; 
        }
    }
}
