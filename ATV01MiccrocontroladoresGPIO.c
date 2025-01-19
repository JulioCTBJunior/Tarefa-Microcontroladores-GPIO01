#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define ROWS 4
#define COLS 4
uint8_t row_pins[ROWS] = {16, 17, 18, 19};  
uint8_t col_pins[COLS] = {20, 4, 9, 8};  

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
        gpio_put(row_pins[i], 1); // Inicializa em HIGH (linhas inativas)
    }

    for (int i = 0; i < COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
        gpio_pull_up(col_pins[i]);
    }

    uint8_t led_pins[] = { LED_GREEN, LED_BLUE, LED_RED };
    for (int i = 0; i < 3; i++) {
        gpio_init(led_pins[i]);
        gpio_set_dir(led_pins[i], GPIO_OUT);
        gpio_put(led_pins[i], 0); // Inicializa os LEDs apagados
    }

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0); // Inicializa o buzzer desligado
}

char scan_keypad() {
    for (int row = 0; row < ROWS; row++) {
        gpio_put(row_pins[row], 0); // Ativa a linha atual (LOW)
       
        for (int col = 0; col < COLS; col++) {
            if (gpio_get(col_pins[col]) == 0) { // Verifica se a coluna está LOW
               
                gpio_put(row_pins[row], 1); // Desativa a linha antes de retornar
                gpio_put(col_pins[col],1); // Desativa a coluna antes de retornar
                return keys[row][col]; // Retorna a tecla pressionada
            }
        }
        gpio_put(row_pins[row], 1); 
        // Desativa a linha após a varredura
    }
    return 0; // Retorna 0 se nenhuma tecla for pressionada
}

#define BUZZER_PIN 21
#define BUZZER_PWM_WRAP 10000 // Valor de wrap para o PWM (ajuste conforme necessário)

// Frequências das notas musicais (em Hz)
#define NOTE_C4  262 // Dó
#define NOTE_D4  294 // Ré
#define NOTE_E4  330 // Mi
#define NOTE_F4  349 // Fá
#define NOTE_G4  392 // Sol
#define NOTE_A4  440 // Lá
#define NOTE_B4  494 // Si
#define NOTE_C5  523 // Dó (oitava acima)

// Função para gerar um tom no buzzer
void beep(uint32_t frequency, uint32_t duration_ms) {
    if (frequency == 0) {
        gpio_put(BUZZER_PIN, 0); // Desliga o buzzer
        return;
    }

    // Configura o PWM para gerar o tom
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint channel_num = pwm_gpio_to_channel(BUZZER_PIN);

    pwm_set_wrap(slice_num, BUZZER_PWM_WRAP);
    pwm_set_clkdiv(slice_num, 125.0f * 256.0f / frequency); // Ajusta o divisor de clock para a frequência desejada
    pwm_set_chan_level(slice_num, channel_num, BUZZER_PWM_WRAP / 2); // Duty cycle de 50%
    pwm_set_enabled(slice_num, true);

    sleep_ms(duration_ms); // Mantém o tom pelo tempo especificado

    // Desliga o buzzer após o tempo
    pwm_set_enabled(slice_num, false);
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_SIO);
    gpio_put(BUZZER_PIN, 0);
}

void handle_key_press(char key) {
    switch (key) {
        case '1':
            gpio_put(LED_RED, 1); // Acende o LED vermelho
            beep(NOTE_C4, 200); // Toca a nota C4 (Dó)
            break;
        case '2':
            gpio_put(LED_RED, 0); // Apaga o LED vermelho
            beep(100, 50);
            break;
        case '3':
            gpio_put(LED_BLUE, 1); // Acende o LED azul
            beep(NOTE_D4, 200); // Toca a nota D4 (Ré)
            break;
        case '4':
            gpio_put(LED_BLUE, 0); // Apaga o LED azul
            beep(100, 50);
            break;
        case '5':
            gpio_put(LED_GREEN, 1); // Acende o LED verde
            beep(NOTE_E4, 200); // Toca a nota E4 (Mi)
            break;
        case '6':
            gpio_put(LED_GREEN, 0); // Apaga o LED verde
            beep(100, 50);
            break;
        case '7':
            gpio_put(BUZZER_PIN, 1); // Liga o buzzer
            beep(NOTE_F4, 200); // Toca a nota F4 (Fá)
            break;
        case '8':
            beep(100, 50);
            gpio_put(BUZZER_PIN, 0); // Desliga o buzzer
            
            break;
        case '9':
            gpio_put(LED_RED, 1); // Acende o LED vermelho
            gpio_put(LED_BLUE, 1); // Acende o LED azul
            beep(NOTE_G4, 200); // Toca a nota G4 (Sol)
            break;
        case '0':
            gpio_put(LED_RED, 0); // Apaga o LED vermelho
            gpio_put(LED_BLUE, 0); // Apaga o LED azul
            beep(100, 50);
            break;
        case 'A':
            gpio_put(LED_RED, 1); // Acende o LED vermelho
            gpio_put(LED_GREEN, 1); // Acende o LED verde
            beep(NOTE_A4, 200); // Toca a nota A4 (Lá)
            break;
        case 'B':
            gpio_put(LED_RED, 0); // Apaga o LED vermelho
            gpio_put(LED_GREEN, 0); // Apaga o LED verde
            beep(100, 50);
            break;
        case 'C':
            gpio_put(LED_BLUE, 1); // Acende o LED azul
            gpio_put(BUZZER_PIN, 1); // Liga o buzzer
            beep(NOTE_B4, 200); // Toca a nota B4 (Si)
            break;
        case 'D':
            gpio_put(LED_BLUE, 0); // Apaga o LED azul
            beep(100, 50);
            gpio_put(BUZZER_PIN, 0); // Desliga o buzzer
            
            break;
        case '*':
            gpio_put(LED_RED, 1);
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_BLUE, 1);
            gpio_put(BUZZER_PIN, 1); // Acende todos os LEDs e liga o buzzer
            beep(NOTE_C5, 200); // Toca a nota C5 (Dó)
            break;
        case '#':
            gpio_put(LED_RED, 0);
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_BLUE, 0);
            beep(100, 50);
            gpio_put(BUZZER_PIN, 0); // Apaga todos os LEDs e desliga o buzzer
            break;
        default:
            printf("Tecla não atribuída: %c\n", key);
            break;
    }
}

void handle_key_release(char key) {
    switch (key) {
        case 'A':
            gpio_put(LED_RED, 0); // Apaga o LED vermelho ao soltar
            gpio_put(LED_GREEN, 0); // Apaga o LED verde ao soltar
            beep(NOTE_A4, 200); // Toca a nota A4 (Lá)
            break;
        case 'B':
            gpio_put(LED_RED, 0); // Apaga o LED vermelho ao soltar
            gpio_put(LED_GREEN, 0); // Apaga o LED verde ao soltar
            beep(100, 50);
            break;
        case 'C':
            gpio_put(LED_BLUE, 0); // Apaga o LED azul ao soltar
            gpio_put(BUZZER_PIN, 0); // Desliga o buzzer ao soltar
            beep(NOTE_B4, 200); // Toca a nota B4 (Si)
            break;
        case 'D':
            gpio_put(LED_BLUE, 0); // Apaga o LED azul ao soltar
            gpio_put(BUZZER_PIN, 0); // Desliga o buzzer ao soltar
            beep(NOTE_C4, 200); // Toca a nota B4 (Si)
            break;
        case '*':
            gpio_put(LED_RED, 0);
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_BLUE, 0);
            gpio_put(BUZZER_PIN, 0); // Apaga todos os LEDs e desliga o buzzer
            break;
        case '#':
            gpio_put(LED_RED, 1);
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_BLUE, 1);
            gpio_put(BUZZER_PIN, 1);
            beep(NOTE_A4, 50);
            beep(NOTE_B4,50);
            beep(NOTE_C4, 50);
            beep(NOTE_D4, 50);
            beep(NOTE_E4, 50);
            beep(NOTE_F4, 50);
            beep(NOTE_G4, 50);
            beep(NOTE_A4, 50);
            beep(NOTE_B4, 50);
            beep(NOTE_C5, 50);
    
             // Acende todos os LEDs e liga o buzzer
            break;
        default:
            break;
    }
}
int main() {
    stdio_init_all();
    init_gpio();

    char last_key = 0;

    while (1) {
        char key = scan_keypad();

        if (key && key != last_key) { // Nova tecla pressionada
            printf("Tecla pressionada: %c\n", key);
            handle_key_press(key);
            last_key = key;
        }

        if (!key && last_key) { // Tecla liberada
            handle_key_release(last_key);
            last_key = 0;
        }

        sleep_ms(50); // Debounce para evitar múltiplas leituras
    }
}