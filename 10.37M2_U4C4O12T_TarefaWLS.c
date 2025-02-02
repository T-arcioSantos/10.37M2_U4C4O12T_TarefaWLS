#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"

#define NUM_PIXELS 25
#define WS2812_PIN 7
#define IS_RGBW false

static PIO pio = pio0;
static uint sm;
static uint offset;

const uint button_A = 5;
const uint button_B = 6;

// Pinos dos LEDs
const uint ledRed_pin = 13;
const uint ledGreen_pin = 11;
const uint ledBlue_pin = 12;

static volatile int num = 0;
static volatile uint a = 1;
static volatile uint b = 1;
static volatile uint last_time = 0;

// Mapeamento dos LEDs para facilitar o desenho dos números
int mapa_leds[25] = {
    24, 23, 22, 21, 20,  
    15, 16, 17, 18, 19,  
    14, 13, 12, 11, 10,  
    5,  6,  7,  8,  9,  
    4,  3,  2,  1,  0   
};

// Matrizes para desenhar os números nos LEDs
// Dígito 0
static bool led_digit0[NUM_PIXELS] = {
    0,1,1,1,0,
    1,0,0,0,1,
    1,0,0,0,1,
    1,0,0,0,1,
    0,1,1,1,0
};

// Dígito 1
static bool led_digit1[NUM_PIXELS] = {
    1,1,1,0,0,
    0,0,1,0,0,
    0,0,1,0,0,
    0,0,1,0,0,
    0,1,1,1,0
};

// Dígito 2
static bool led_digit2[NUM_PIXELS] = {
    1,1,1,1,0,
    0,0,0,0,1,
    0,1,1,1,0,
    1,0,0,0,0,
    1,1,1,1,1
};

// Dígito 3
static bool led_digit3[NUM_PIXELS] = {
    1,1,1,1,0,
    0,0,0,0,1,
    0,1,1,1,0,
    0,0,0,0,1,
    1,1,1,1,0
};

// Dígito 4
static bool led_digit4[NUM_PIXELS] = {
    1,0,0,1,0,
    1,0,0,1,0,
    1,1,1,1,1,
    0,0,0,1,0,
    0,0,0,1,0
};

// Dígito 5
static bool led_digit5[NUM_PIXELS] = {
    1,1,1,1,1,
    1,0,0,0,0,
    1,1,1,1,0,
    0,0,0,0,1,
    1,1,1,1,0
};

// Dígito 6
static bool led_digit6[NUM_PIXELS] = {
    0,1,1,1,0,
    1,0,0,0,0,
    1,1,1,1,0,
    1,0,0,0,1,
    0,1,1,1,0
};

// Dígito 7
static bool led_digit7[NUM_PIXELS] = {
    1,1,1,1,1,
    0,0,0,0,1,
    0,0,0,1,0,
    0,0,1,0,0,
    0,1,0,0,0
};

// Dígito 8
static bool led_digit8[NUM_PIXELS] = {
    0,1,1,1,0,
    1,0,0,0,1,
    0,1,1,1,0,
    1,0,0,0,1,
    0,1,1,1,0
};

// Dígito 9
static bool led_digit9[NUM_PIXELS] = {
    0,1,1,1,0,
    1,0,0,0,1,
    0,1,1,1,1,
    0,0,0,0,1,
    0,1,1,1,0
};

// Vetor de ponteiros para os dígitos
static bool* digitos[10] = {
    led_digit0, led_digit1, led_digit2, led_digit3, led_digit4,
    led_digit5, led_digit6, led_digit7, led_digit8, led_digit9
};

// Protótipos das funções
static inline void put_pixel(uint32_t pixel_grb);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
void set_one_led(bool *desenho, uint8_t r, uint8_t g, uint8_t b);

// Função de callback para a interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);
int main()
{
    stdio_init_all();

    gpio_init(ledRed_pin);
    gpio_set_dir(ledRed_pin, GPIO_OUT);
    gpio_init(ledGreen_pin);
    gpio_set_dir(ledGreen_pin, GPIO_OUT);
    gpio_init(ledBlue_pin);
    gpio_set_dir(ledBlue_pin, GPIO_OUT);

    gpio_init(button_A);
    gpio_set_dir(button_A, GPIO_IN);
    gpio_pull_up(button_A);
    gpio_init(button_B);
    gpio_set_dir(button_B, GPIO_IN);
    gpio_pull_up(button_B);

    sm = pio_claim_unused_sm(pio, true);;
    offset = pio_add_program(pio, &ws2812_program);

    // Inicializa o programa de controle do LED WS2812
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Inicializa o LED com o dígito 0
    set_one_led(digitos[num], 255, 0, 0);

    // Loop infinito para piscar o LED vermelho piscar 5 vezes por segundo
    while (true) {
        gpio_put(ledRed_pin, true);
        sleep_ms(100);
        gpio_put(ledRed_pin, false);
        sleep_ms(100);
    }
}

// Função de callback para a interrupção
void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    printf("A = %d/ B= %d\n", a, b);
    
    // Debounce - verifica se o tempo entre as interrupções é maior que 200ms
    if (current_time - last_time > 200000) // se sim, atualiza o tempo e executa a ação
    {
        last_time = current_time; 

        // Verifica qual botão foi pressionado
        if(gpio == button_A){
            printf("Acionamento button_a = %d\n", a);
            printf("button_a\n");
            a++; //
            num++;
            if(num > 9){
                num = 0;
            }
            // Atualiza o LED com o novo dígito incrementado com cor vermelha
            set_one_led(digitos[num], 255, 0, 0);
        }
        else if(gpio == button_B){ 
            printf("Acionamento button_b = %d\n", b);
            printf("button_b\n");
            b++; 
            num--;
            if(num < 0){
                num = 9;
            }
            printf("a = %d\n", num);
            // Atualiza o LED com o novo dígito decrementado com cor azul
            set_one_led(digitos[num], 0, 0, 255);
        }
    }
}

// Função para enviar um pixel para o LED
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Função para converter um valor RGB para um valor de 32 bits
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Função para desenhar um número nos LEDs
void set_one_led(bool *led_buffer,uint8_t r, uint8_t g, uint8_t b)
{   
    float fator = 0.02;

    uint32_t color = urgb_u32(r * fator, g * fator, b * fator);

    for (int i = 0; i < NUM_PIXELS; i++)
    {
        // Mapeia a posição do LED
        int pos = mapa_leds[i];
        if (led_buffer[pos])
        {
            put_pixel(color); 
        }
        else
        {
            put_pixel(0); 
        }
    }
}