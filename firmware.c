/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/irq.h" 
#include "hardware/pwm.h" 
#include "hardware/timer.h"
#include "pico/bootrom.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "symbols.h"
#include "ws2812.pio.h"

// Pinagem dos componentes
#define I2C_PORT i2c1 // Define que o barramento I2C usado será o "i2c1"
#define I2C_SDA 14 // Define que o pino GPIO 14 será usado como SDA (linha de dados do I2C)
#define I2C_SCL 15 // Define que o pino GPIO 15 será usado como SCL (linha de clock do I2C)
#define address 0x3C // Define o endereço I2C do dispositivo (0x3C é o endereço padrão de muitos displays OLED SSD1306)
#define JOY_Y 26 // Eixo Y do joystick 
#define JOY_X 27 // Eixo X do joystick 
#define BTNJ 22 // Botão do joystick
#define BTNA 5 // Botão A
#define BTNB 6 // Botão B
#define RED 13 // LED vermelho
#define BLUE 12 // LED azul
#define GREEN 11 // LED verde
#define WS2812_PIN 7 // Matriz de LEDs 5x5

#define NUM_PIXELS 25 // 5x5 = 25
#define IS_RGBW false

/// Parâmetros do PWM 
/*
    fpwm = fckl / (di * (wrap + 1)), fpwm = 50KHz (Tpwm = 20us)
    wrap = 2499
    50KHz = 125Mhz/(div*(2499 + 1))
    50KHz = 50KHz/div
    div = 1
*/
#define WRAP 2499 // Wrap
#define DIV 1 // Divisor inteiro

bool mode = true; // True (Modo de Execução) e False (Modo de Simulação)
unsigned short int voltage_mode = 1; // 0 (modo 1.8 V), 1 (modo 3.3 V) e 2 (modo 5 V)
bool confirmation = false;
bool isActive = true; 
bool reset = false;

double voltage = 0.0;
double current = 0.0;
double error = 0.0;
double x1 = 0.0; // Valor de x1(0), estado que corresponde à corrente do indutor
double x2 = 0; // Valor de x2(0), estado que corresponde à tensão de saída
double x3 = 3.3 * 20e-6; // Valor de x3(0), estado que corresponde ao erro do integrador
double u = 1.7265e-3; // Valores para u(0), sinal de controle

static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

void setup(); // Prototipação da função que define os LEDs RGB como saídas e os botões como entradas
void i2c_setup();
void pwm_setup();
void ssd1306_setup(ssd1306_t* ssd);
void ws2812_setup(PIO pio, uint sm); // Prototipação da função que configura a matriz de LEDs 5x5 
void gpio_irq_handler(uint gpio, uint32_t events);
void adc_setup();
static bool adc_read_timer(struct repeating_timer *t); // Prototipação da rotina de temporização

int main() {
    ssd1306_t ssd;
    PIO pio = pio0;
    int sm = 0;
    bool color = true;
    char voltage_str[10], current_str[10], dc_str[10];  // Buffer para armazenar a string
    
    stdio_init_all(); // Inicialização dos recursos de entrada e saída padrão
    adc_setup(); // Inicialização e configuração dos pinos ADC
    setup(); // Inicialização e configuração dos LEDs e botões 
    i2c_setup(); // Inicialização e configuração da comunicação serial I2C 
    pwm_setup(); // Inicialização e configuração do PWM
    ssd1306_setup(&ssd); // Inicializa a estrutura do display
    ws2812_setup(pio, sm); // Inicialização e configuração da matriz de LEDs 5x5

    symbols('*');

    struct repeating_timer timer; // Timer

    absolute_time_t next_time = delayed_by_us(get_absolute_time(), 3e6); // Registra o tempo absoluto a cada 3 seg

    add_repeating_timer_us(20, adc_read_timer, NULL, &timer); // Inicializa o temporizador periódico que chama o callback a cada 20 us
    
    gpio_set_irq_enabled_with_callback(BTNJ, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão do Joystick
    gpio_set_irq_enabled_with_callback(BTNA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão A
    gpio_set_irq_enabled_with_callback(BTNB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão B

    uint slice_blue = pwm_gpio_to_slice_num(BLUE); // Slice PWM do pino 12
    uint slice_red = pwm_gpio_to_slice_num(RED); // Slice PWM do pino 13

    while (true) {
        if(!reset) {
            if(mode) {
                sprintf(voltage_str, "%.2f", voltage);
                sprintf(current_str, "%.2f", current);
                sprintf(dc_str, "%.2f", (u >= 0) ? 100*u : 0.0);

                (
                    (voltage_mode == 0 && (voltage <= (1.8+1.8*0.1))) ||
                    (voltage_mode == 1 && (voltage <= (3.3+3.3*0.1))) ||
                    (voltage_mode == 2 && (voltage <= (5+5*0.1))) &&
                    (current <= 0.5+0.5*0.1)
                ) ? symbols('*') : symbols('x');
                
                ssd1306_fill(&ssd, !color); // Limpa ou mostra a tela
                ssd1306_rect(&ssd, 0, 0, 128, 64, color, !color); // Retângulo padrão

                ssd1306_draw_string(&ssd, voltage_str, 10, 10);
                ssd1306_draw_char(&ssd, 'V', 45, 10);
            
                ssd1306_draw_string(&ssd, current_str, 75, 10);
                ssd1306_draw_char(&ssd, 'A', 110, 10);

                ssd1306_draw_string(&ssd, "PWM:", 10, 30);   
                ssd1306_draw_string(&ssd, dc_str, 45, 30);
                ssd1306_draw_char(&ssd, '%', (u >= 0.1) ? 88 : 80, 30);

                ssd1306_draw_string(&ssd, "MODO:", 10, 50);   
                ssd1306_draw_string(&ssd, (voltage_mode==0) ? "1.8" : (voltage_mode==1) ? "3.3" : "5.0", 55, 50);
                ssd1306_draw_char(&ssd, 'V', 83, 50);

                ssd1306_send_data(&ssd); // Atualiza o display 

                // Ativa o PWM
                pwm_set_enabled(slice_blue, isActive);

                // Atualiza o duty cycle baseado no controle u (0 a 1)
                (u >= 0.0) ? pwm_set_gpio_level(BLUE, (uint16_t)(u * (WRAP + 1))) : pwm_set_gpio_level(BLUE, 0);
            } 
        } else {
            printf("Saindo para o modo de gravação...\n\n");

            ssd1306_fill(&ssd, false); // Limpa a tela
            ssd1306_draw_string(&ssd, "MODO DE", 28, 28); 
            ssd1306_draw_string(&ssd, "GRAVACAO", 24, 40);
            ssd1306_send_data(&ssd); // Envia os dados para o display

            reset_usb_boot(0,0); // Sai para o modo de gravação
        }
    }
}

// Inicializa e configura os LEDs RGB como saída. Inicializa e configura os botões como entradas.
void setup() {
    gpio_init(GREEN);
    gpio_set_dir(GREEN, GPIO_OUT);
    gpio_put(GREEN,false);
  
    gpio_init(BTNA);
    gpio_set_dir(BTNA, GPIO_IN);
    gpio_pull_up(BTNA);  
  
    gpio_init(BTNB);
    gpio_set_dir(BTNB, GPIO_IN);
    gpio_pull_up(BTNB);  

    gpio_init(BTNJ);
    gpio_set_dir(BTNJ, GPIO_IN);
    gpio_pull_up(BTNJ); 
}

// Inicializa e configura os pinos do joystick como periféricos ADC
void adc_setup() {
    adc_init();
    adc_gpio_init(JOY_Y);
    adc_gpio_init(JOY_X);
}

// Inicializa e configura a comunicação serial I2C 
void i2c_setup() {
    i2c_init(I2C_PORT, 4e2 * 1e3); // Inicialização I2C.
  
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Define a função do pino GPIO para I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Define a função do pino GPIO para I2C
    gpio_pull_up(I2C_SDA); // Pull up para linha de dados
    gpio_pull_up(I2C_SCL); // Pull up para linha de clock
}

// Limpa o display
void clear(ssd1306_t* ssd) {
    ssd1306_fill(ssd, false); // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_send_data(ssd);
}

// Inicializa e configura o display
void ssd1306_setup(ssd1306_t* ssd) {
    ssd1306_init(ssd, WIDTH, HEIGHT, false, address, I2C_PORT); 
    ssd1306_config(ssd); 
    ssd1306_send_data(ssd);
    clear(ssd);
}

// Inicializa e configura a matriz de LEDs 5x5 
void ws2812_setup(PIO pio, uint sm) {
    uint offset = pio_add_program(pio, &ws2812_program);
  
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW); //Inicializa a matriz de leds
  }

// Inicializa e configura os pinos 13 e 12 como PWM
void pwm_setup() {
    gpio_set_function(BLUE, GPIO_FUNC_PWM); // Define o pino como PWM
    gpio_set_function(RED, GPIO_FUNC_PWM); // Define o pino como PWM

    uint slice_blue = pwm_gpio_to_slice_num(BLUE); // Obtém o slice
    uint slice_red = pwm_gpio_to_slice_num(RED); // Obtém o slice
    
    pwm_set_clkdiv(slice_blue, DIV); // Define o divisor inteiro de clock
    pwm_set_wrap(slice_blue, WRAP); // Define o wrap
    pwm_set_enabled(slice_blue, true);

    pwm_set_clkdiv(slice_red, DIV); // Define o divisor inteiro de clock
    pwm_set_wrap(slice_red, WRAP); // Define o wrap
    pwm_set_enabled(slice_red, true);
    
}

// Retorna a leitura de um determinado canal ADC
uint16_t select_adc_channel(unsigned short int channel) {
    adc_select_input(channel);
    return adc_read();
}

// Rotina de temporização. A cada 20 us é lido o valor analogico do joystick
bool adc_read_timer(struct repeating_timer *t) {
    voltage = (select_adc_channel(1) * 12.0) / 4095.0; // Eixo X (0 - 4095).
    current = (select_adc_channel(0) * 1.0) / 4095.0; // Eixo Y (0 - 4095).
    if(voltage_mode==0) {
        error = 20e-6*(1.8-voltage);
        u = (voltage <= (1.8+1.8*0.1) && current <= (0.5+0.5*0.1)) ? -0.1273*current + 0.0725*voltage + 27.7076*error : 0.0;
    } else if(voltage_mode==1) {
        error = 20e-6*(3.3-voltage);
        u = (voltage <= (3.3+3.3*0.1) && current <= (0.5+0.5*0.1)) ? -0.1499*current + 0.0650*voltage + 27.5408*error : 0.0;
    } else if(voltage_mode==2) {
        error = 20e-6*(5-voltage);
        u = (voltage <= (5+5*0.1) && current <= (0.5+0.5*0.1)) ? -0.1592*current + 0.0611*voltage + 27.4729*error : 0;
    }
    return true;
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // Obtém o tempo atual em microssegundos
  
    if (current_time - last_time > 2e5) { // 200 ms de debouncing
        last_time = current_time; 
        if(gpio == BTNA) {
            voltage_mode = (voltage_mode + 1)%3;
            if(voltage_mode==0) printf("Modo de Tensão 1.8 V!!!\n\n");
            else if(voltage_mode==1) printf("Modo de Tensão 3.3 V!!!\n\n");
            else if(voltage_mode==2) printf("Modo de Tensão 5 V!!!\n\n");
        } else if(gpio == BTNJ) {
            mode = !mode;
            (mode) ? printf("Modo de Execução!!!\n\n")  : printf("Modo de Simulação!!!\n\n");
            reset = true;
        } else if(gpio == BTNB) {
            if(!mode) confirmation = true;
            else {
                isActive=!isActive;
            }
        }
    }
}


