#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define DISPLAY_ADDR 0x3C

// Definições de pinos
#define LED_R 13
#define LED_G 11
#define LED_B 12
#define JOY_X 26
#define JOY_Y 27
#define BUTTON_JOY 22
#define BUTTON_A 5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SQUARE_SIZE 8

bool led_active = true;
bool led_green_state = false;
uint square_x = SCREEN_WIDTH / 2 - SQUARE_SIZE / 2;
uint square_y = SCREEN_HEIGHT / 2 - SQUARE_SIZE / 2;

ssd1306_t display;

// Função para desenhar o quadrado
void draw_square(uint x, uint y) {
    ssd1306_fill(&display, false);
    ssd1306_rect(&display, y, x, SQUARE_SIZE, SQUARE_SIZE, true, false);
    ssd1306_send_data(&display);
}

// Função para configurar o PWM
void setup_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, 255);  // Define o valor máximo do PWM (8 bits)
    pwm_set_enabled(slice_num, true);
}

// Função para atualizar os LEDs
void update_leds(uint16_t joy_x, uint16_t joy_y) {
    if (led_active) {
        // Calcula o desvio em relação ao centro (2048)
        int deviation_x = abs((int)joy_x - 2048);
        int deviation_y = abs((int)joy_y - 2048);

        // Mapeia o desvio para o intervalo de 0 a 255
        uint level_r = deviation_x * 255 / 2048;  // Eixo X controla LED Vermelho
        uint level_b = deviation_y * 255 / 2048;  // Eixo Y controla LED Azul

        // Limita os valores ao intervalo de 0 a 255
        if (level_r > 255) level_r = 255;
        if (level_b > 255) level_b = 255;

        // Desliga os LEDs se o joystick estiver no marco 0 ou 4095
        if (joy_x == 0 || joy_x == 4095) level_r = 0;
        if (joy_y == 0 || joy_y == 4095) level_b = 0;

        // Ajusta o PWM dos LEDs
        pwm_set_gpio_level(LED_R, level_r);
        pwm_set_gpio_level(LED_B, level_b);
    } else {
        // Desliga os LEDs PWM se estiverem inativos
        pwm_set_gpio_level(LED_R, 0);
        pwm_set_gpio_level(LED_B, 0);
    }
}

// Função para controlar o joystick
void joystick_control() {
    // Leitura dos valores do joystick
    adc_select_input(0);
    uint16_t joy_x = adc_read();  // Valor do eixo X do joystick
    
    adc_select_input(1);
    uint16_t joy_y = adc_read();  // Valor do eixo Y do joystick
    
    // Ajusta a posição do quadrado com base nos valores do joystick
    square_x = joy_y * (SCREEN_WIDTH - SQUARE_SIZE) / 4095;          // Usa joy_y para controlar X
    square_y = (4095 - joy_x) * (SCREEN_HEIGHT - SQUARE_SIZE) / 4095; // Usa joy_x para controlar Y

    // Evita que o quadrado ultrapasse os limites da tela
    if (square_x + SQUARE_SIZE > SCREEN_WIDTH) {
        square_x = SCREEN_WIDTH - SQUARE_SIZE;
    }
    if (square_y + SQUARE_SIZE > SCREEN_HEIGHT) {
        square_y = SCREEN_HEIGHT - SQUARE_SIZE;
    }

    // Desenha o quadrado e atualiza os LEDs
    draw_square(square_x, square_y);
    update_leds(joy_x, joy_y);
}

// Função de interrupção para os botões
void button_irq(uint gpio, uint32_t events) {
    static uint32_t last_press_time = 0;
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_press_time < 200) return; // Debounce
    last_press_time = now;
    
    if (gpio == BUTTON_JOY) {
        led_green_state = !led_green_state;
        gpio_put(LED_G, led_green_state);  // Alterna LED verde
    } else if (gpio == BUTTON_A) {
        led_active = !led_active;  // Ativa/desativa LEDs PWM
    }
}

int main() {
    stdio_init_all();
    
    // Inicializa I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // Inicializa o display SSD1306
    ssd1306_init(&display, SCREEN_WIDTH, SCREEN_HEIGHT, false, DISPLAY_ADDR, I2C_PORT);
    ssd1306_config(&display);
    
    // Inicializa pinos para PWM (LEDs RGB)
    setup_pwm(LED_R);
    setup_pwm(LED_B);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    
    // Inicializa os botões
    gpio_init(BUTTON_JOY);
    gpio_set_dir(BUTTON_JOY, GPIO_IN);
    gpio_pull_up(BUTTON_JOY);
    
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    
    // Habilita interrupções nos botões
    gpio_set_irq_enabled_with_callback(BUTTON_JOY, GPIO_IRQ_EDGE_FALL, true, &button_irq);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_irq);
    
    // Inicializa ADC para leitura do joystick
    adc_init();
    adc_gpio_init(JOY_X);
    adc_gpio_init(JOY_Y);
    
    while (true) {
        joystick_control();
        sleep_ms(100);
    }
}