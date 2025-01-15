#include <stdio.h>
#include "pico/stdlib.h"

#define ROW0 2 // Linha 0 do teclado
#define ROW1 3 // Linha 1 do teclado
#define ROW2 4 // Linha 2 do teclado
#define ROW3 5 // Linha 3 do teclado
#define COL0 6 // Coluna 0 do teclado
#define COL1 7 // Coluna 1 do teclado
#define COL2 8 // Coluna 2 do teclado
#define COL3 9 // Coluna 3 do teclado

#define LED_RED 13   // LED Vermelho (RGB)
#define LED_GREEN 11 // LED Verde (RGB)
#define LED_BLUE 12  // LED Azul (RGB)

#define BUZZER 10 // Buzzer

// Funções principais
void setup_gpio();
void scan_keypad();
void control_leds(int key);
void control_buzzer(int key);

int main() {
    stdio_init_all(); // Inicializa a comunicação serial para debug
    setup_gpio();     // Configura os GPIOs

    printf("Sistema iniciado...\n");

    while (true) {

    }

    return 0;
}

// Configuração dos GPIOs
void setup_gpio() {

}

// Escanear o teclado matricial
void scan_keypad() {

}

// Controle de LEDs RGB
void control_leds(int key) {

}

// Controle do Buzzer
void control_buzzer(int key) {

}
