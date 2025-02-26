/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#include "inc/screen_selection.h"

/* 
    0 (Tela de Seleção: Modo de Operação), 1 (Tela de Seleção: Simulador), 2 (Tela de Seleção: Seleção de Tensão), 3 (Tela de Seleção: Modo de Gravação)
    4 (Modo de Operação), 5 (Simulação), 7 (Requisição da Conexão USB)

*/
unsigned short int screen = 0;

unsigned short int option = 1; // 0 (modo 1.8 V), 1 (modo 3.3 V) e 2 (modo 5 V)
unsigned short int setpoint = 1; // Quando vale 0, permite simular o sistema com condições iniciais nulas

double volt_mode = 3.3; // Modo de tensão. Define a tensão de referência
double voltage = 0.0; // Valor de tensão lido
double current = 0.0; // Valor de corrente lido
double u = 0.0; // Sinal de controle

static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

static bool adc_read_timer(struct repeating_timer *t); // Prototipação da rotina de temporização
void gpio_irq_handler(uint gpio, uint32_t events); // Prototipação da função de interrupção

int main() {
    ssd1306_t ssd;
    PIO pio = pio0;
    int sm = 0;
    bool stop = false;
    bool color = true;
    struct repeating_timer timer; // Timer
    bool timer_active = false; // Gerencia a temporização
    
    setup(&ssd,pio,sm); // Inicializa todos os componentes

    ws2812('*'); // Limpa a matriz de LEDs
    
    gpio_set_irq_enabled_with_callback(BTNJ, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão do Joystick
    gpio_set_irq_enabled_with_callback(BTNA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão A
    gpio_set_irq_enabled_with_callback(BTNB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Callback de interrupção do Botão B

    while (true) {
        if ((screen == 1 || screen == 4) && !timer_active) {
            add_repeating_timer_us(20, adc_read_timer, NULL, &timer); // Temporização de 20 us. Lê os valores dos eixos do analógico
            timer_active = true;
        } else if (!(screen == 1 || screen == 4) && timer_active) {
            cancel_repeating_timer(&timer); // Desativa a temporização
            timer_active = false;
        }

        if(!(screen == 4)) pwm_set_gpio_level(BLUE, 0); // Se não estiver no modo de operação, desativa o PWM do LED azul
        if(!(screen == 5)) pwm_set_gpio_level(GREEN, 0); // Se não estiver no modo de simulação, desativa o PWM do LED verde
        mode_selection_screen(&ssd, color, voltage, current, u, volt_mode, screen, setpoint, &stop); // Seleciona a tela atual
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
    double error = 20e-6*(volt_mode-voltage); // Calcula o erro do integrador

    // Coeficiente da equação
    double a[3] = {((volt_mode==5.0) ? 0.15918 : (volt_mode==3.3) ? 0.14995 : 0.12732), ((volt_mode==5.0) ? 0.061113 : (volt_mode==3.3) ? 0.064964 : 0.072473), ((volt_mode==5.0) ? 27.473 : (volt_mode==3.3) ? 27.541 : 27.708)};
    u = (voltage <= (volt_mode+volt_mode*0.1) && current <= (0.5+0.5*0.1)) ? -a[0]*current + a[1]*voltage + a[2]*error : 0.0; // Calcula o sinal de controle (duty cycle)
    return true;
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // Obtém o tempo atual em microssegundos
  
    if (current_time - last_time > 2e5) { // 200 ms de debouncing
        last_time = current_time; 
        if(gpio == BTNA) { // Botão A alterna entre diferentes telas ou retorna para as telas de seleção
            ws2812('*');
            screen = (screen >= 0 && screen <= 3) ? (screen + 1)%4 : screen - 4;
        } else if(gpio == BTNJ) {
            if(screen == 1) {setpoint = 0;screen = screen + 4;} // Caso esteja na tela do simulador, realiza uma simulação sob condições iniciais nulas
        } else if(gpio == BTNB) { // Botão B confirma ou alterna uma opção
            setpoint = 1;
            if(screen==2) {
                option = (option + 1)%3;
                volt_mode = (option==0) ? 1.8 : (option==1) ? 3.3 : 5.0;
            } else if(screen >= 0 && screen <= 3) screen = screen + 4;
        }
    }
}


