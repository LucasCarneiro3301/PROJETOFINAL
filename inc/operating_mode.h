/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#ifndef OPERATING_H
#define OPERATING_H

#include "setup.h"
#include "ws2812.h"

void operating_mode(ssd1306_t *ssd, bool color, double voltage, double current, double u, unsigned short int voltage_mode, bool isActive) {
    char voltage_str[10], current_str[10], dc_str[10];  // Buffer para armazenar a string

    (
        (voltage_mode == 0 && (voltage <= (1.8+1.8*0.1))) ||
        (voltage_mode == 1 && (voltage <= (3.3+3.3*0.1))) ||
        (voltage_mode == 2 && (voltage <= (5+5*0.1))) &&
        (current <= 0.5+0.5*0.1)
    ) ? ws2812('*') : ws2812('x');

    sprintf(voltage_str, "%.2f", voltage);
    sprintf(current_str, "%.2f", current);
    sprintf(dc_str, "%.2f", (u >= 0) ? 100*u : 0.0);

    ssd1306_fill(ssd, !color); // Limpa ou mostra a tela
    ssd1306_rect(ssd, 0, 0, 128, 64, color, !color); // Retângulo padrão

    ssd1306_draw_string(ssd, voltage_str, 10, 10);
    ssd1306_draw_char(ssd, 'V', 45, 10);

    ssd1306_draw_string(ssd, current_str, 75, 10);
    ssd1306_draw_char(ssd, 'A', 110, 10);

    ssd1306_draw_string(ssd, "PWM:", 10, 30);   
    ssd1306_draw_string(ssd, dc_str, 45, 30);
    ssd1306_draw_char(ssd, '%', (u >= 0.1) ? 88 : 80, 30);

    ssd1306_draw_string(ssd, "MODO:", 10, 50);   
    ssd1306_draw_string(ssd, (voltage_mode==0) ? "1.8" : (voltage_mode==1) ? "3.3" : "5.0", 55, 50);
    ssd1306_draw_char(ssd, 'V', 83, 50);

    ssd1306_send_data(ssd); // Atualiza o display 
    
    // Ativa o PWM
    pwm_set_enabled(pwm_gpio_to_slice_num(BLUE), isActive);

    // Atualiza o duty cycle baseado no controle u (0 a 1)
    (u >= 0.0) ? pwm_set_gpio_level(BLUE, (uint16_t)(u * (WRAP + 1))) : pwm_set_gpio_level(BLUE, 0);
}

#endif