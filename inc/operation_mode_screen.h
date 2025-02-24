/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#ifndef OPERATING_H
#define OPERATING_H

#include "setup.h"
#include "ws2812.h"

void operating_mode(ssd1306_t *ssd, bool color, double voltage, double current, double u, double volt_mode) {
    char str[10];  // Buffer para armazenar a string

    (voltage <= (volt_mode+volt_mode*0.1) && (current <= 0.5+0.5*0.1)) ? ws2812('*') : ws2812('x');

    ssd1306_fill(ssd, !color); // Limpa ou mostra a tela
    ssd1306_rect(ssd, 0, 0, 128, 64, color, !color); // Retângulo padrão

    sprintf(str, "%.2f", voltage);

    ssd1306_draw_string(ssd, str, 10, 10);
    ssd1306_draw_char(ssd, 'V', 45, 10);

    sprintf(str, "%.2f", current);

    ssd1306_draw_string(ssd, str, 75, 10);
    ssd1306_draw_char(ssd, 'A', 110, 10);

    sprintf(str, "%.2f", (u >= 0) ? 100*u : 0.0);

    ssd1306_draw_string(ssd, "PWM:", 10, 30);   
    ssd1306_draw_string(ssd, str, 45, 30);
    ssd1306_draw_char(ssd, '%', (u >= 0.1) ? 88 : 80, 30);

    sprintf(str, "%.2f", volt_mode);

    ssd1306_draw_string(ssd, "REF:", 10, 50);   
    ssd1306_draw_string(ssd, str, 45, 50);
    ssd1306_draw_char(ssd, 'V', 80, 50);

    ssd1306_send_data(ssd); // Atualiza o display 

    // Atualiza o duty cycle baseado no controle u (0 a 1)
    (u >= 0.0) ? pwm_set_gpio_level(BLUE, (uint16_t)(u * (WRAP + 1))) : pwm_set_gpio_level(BLUE, 0);
}

#endif