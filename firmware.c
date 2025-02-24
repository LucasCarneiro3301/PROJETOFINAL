/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#include "inc/operating_mode.h"

bool mode = true; // True (Modo de Execução) e False (Modo de Simulação)
unsigned short int voltage_mode = 1; // 0 (modo 1.8 V), 1 (modo 3.3 V) e 2 (modo 5 V)
bool confirmation = false;
bool isActive = true; 
bool reset = false;

double voltage = 0.0;
double current = 0.0;
double error = 0.0;
double u = 1.7265e-3; // Valores para u(0), sinal de controle

static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

static bool adc_read_timer(struct repeating_timer *t); // Prototipação da rotina de temporização
void gpio_irq_handler(uint gpio, uint32_t events);

int main() {
    ssd1306_t ssd;
    PIO pio = pio0;
    int sm = 0;
    bool color = true;
    struct repeating_timer timer; // Timer
    
    setup(&ssd,pio,sm);

    ws2812('*');

    add_repeating_timer_us(20, adc_read_timer, NULL, &timer); // Inicializa o temporizador periódico que chama o callback a cada 20 us
    
    gpio_set_irq_enabled_with_callback(BTNJ, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão do Joystick
    gpio_set_irq_enabled_with_callback(BTNA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão A
    gpio_set_irq_enabled_with_callback(BTNB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão B

    uint slice_blue = pwm_gpio_to_slice_num(BLUE); // Slice PWM do pino 12
    uint slice_red = pwm_gpio_to_slice_num(RED); // Slice PWM do pino 13

    while (true) {
        if(!reset) {
            if(mode) operating_mode(&ssd, color, voltage, current, u, voltage_mode, isActive);
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


