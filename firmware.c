/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#include "inc/mode_selection_screen.h"

unsigned short int screen = 0; // 0 (Tela de Seleção de Modo: Operação), 1 (Tela de Seleção de Modo: Simulação), 2 (Tela de Seleção de Modo: Seleção de Tensão) e 3 (Tela de Seleção de Modo: Gravação)
unsigned short int option = 1; // 0 (modo 1.8 V), 1 (modo 3.3 V) e 2 (modo 5 V)
bool confirmation = false;
bool isActive = true; 
bool reset = false;

double volt_mode = 3.3;
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
    bool stop = false;
    bool color = true;
    struct repeating_timer timer; // Timer
    bool timer_active = false;
    
    setup(&ssd,pio,sm);

    ws2812('*');

    //add_repeating_timer_us(20, adc_read_timer, NULL, &timer); // Inicializa o temporizador periódico que chama o callback a cada 20 us
    
    gpio_set_irq_enabled_with_callback(BTNJ, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão do Joystick
    gpio_set_irq_enabled_with_callback(BTNA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão A
    gpio_set_irq_enabled_with_callback(BTNB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão B

    uint slice_blue = pwm_gpio_to_slice_num(BLUE); // Slice PWM do pino 12
    uint slice_red = pwm_gpio_to_slice_num(RED); // Slice PWM do pino 13

    while (true) {
        if ((screen == 1 || screen == 4) && !timer_active) {
            add_repeating_timer_us(20, adc_read_timer, NULL, &timer);
            timer_active = true;
        } else if (!(screen == 1 || screen == 4 || screen == 5) && timer_active) {
            cancel_repeating_timer(&timer);
            timer_active = false;
        }

        if(!(screen == 4)) pwm_set_gpio_level(BLUE, 0);
        mode_selection_screen(&ssd, color, voltage, current, u, volt_mode, screen, &stop);
    }
}



// Retorna a leitura de um determinado canal ADC
uint16_t select_adc_channel(unsigned short int channel) {
    adc_select_input(channel);
    return adc_read();
}

// Rotina de temporização. A cada 20 us é lido o valor analogico do joystick
bool adc_read_timer(struct repeating_timer *t) {
    voltage = (select_adc_channel(1) * 7.0) / 4095.0; // Eixo X (0 - 4095).
    current = (select_adc_channel(0) * 1.0) / 4095.0; // Eixo Y (0 - 4095).
    error = 20e-6*(volt_mode-voltage);
    u = (voltage <= (volt_mode+volt_mode*0.1) && current <= (0.5+0.5*0.1)) ? -0.1273*current + 0.0725*voltage + 27.7076*error : 0.0;
    return true;
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // Obtém o tempo atual em microssegundos
  
    if (current_time - last_time > 2e5) { // 200 ms de debouncing
        last_time = current_time; 
        if(gpio == BTNA) {
            ws2812('*');
            screen = (screen >= 0 && screen <= 3) ? (screen + 1)%4 : screen - 4;
        } else if(gpio == BTNJ) {
            
        } else if(gpio == BTNB) {
            if(screen==2) {
                option = (option + 1)%3;
                volt_mode = (option==0) ? 1.8 : (option==1) ? 3.3 : 5.0;
            } else if(screen >= 0 && screen <= 3) screen = screen + 4;
            //(screen==0) ? printf("Modo de Operação!!!\n\n")  : (screen==1) ? printf("Modo de Simulação!!!\n\n") : (screen==2) ? printf("Modo de Seleção de Tensão!!!\n\n") : 
            //(screen==3) ? printf("Modo de Gravação!!!\n\n") : ;
        }
    }
}


