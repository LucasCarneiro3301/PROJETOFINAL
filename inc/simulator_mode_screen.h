/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#ifndef SIMULATING_H_H
#define SIMULATING_H_H

#include "setup.h"
#include "ws2812.h"

#define T 20e-6 // 1/50kHz
#define NUM_ITERATIONS 500 // Número de iterações da simulação

void simulator_mode(ssd1306_t *ssd, bool color, double voltage, double current, double volt_mode, bool *stop) {
    char str[10];  // Buffer para armazenar a string
    double x1 = current, x3 = T*(volt_mode-voltage), x2 = voltage, u = 1.7265e-3;

    if(!*stop) {
        printf("Tempo(s),x1,x2,u\n");
        for (int k = 0; k < NUM_ITERATIONS; k++) {
            double tempo = k * T;
    
            u = -0.14995*x1 + 0.064964*x2 + 27.541*x3;
        
            ssd1306_fill(ssd, !color); // Limpa ou mostra a tela
            ssd1306_rect(ssd, 0, 0, 128, 64, color, !color); // Retângulo padrão
        
            sprintf(str, "%.2f", x2);
        
            ssd1306_draw_string(ssd, str, 10, 10);
            ssd1306_draw_char(ssd, 'V', (x2 < 0) ? 55 : 45, 10);
        
            sprintf(str, "%.2f", x1);
        
            ssd1306_draw_string(ssd, str, 10, 30);
            ssd1306_draw_char(ssd, 'A', (x1 < 0) ? 55 : 45, 30);
        
            sprintf(str, "%.2f", u);
        
            ssd1306_draw_string(ssd, "CONTROLE:", 10, 50);   
            ssd1306_draw_string(ssd, str, 85, 50);
        
            sprintf(str, "%03d", k+1);
          
            ssd1306_draw_string(ssd, str, 100, 5);
    
            ssd1306_send_data(ssd); // Atualiza o display
    
            double x1_nxt = 0.90626 * x1 - 0.0089964 * x2 + 13.948 * x3;
            double x2_nxt = 0.18934 * x1 + 0.98465 * x2 + 1.3922 * x3;
            double x3_nxt = -1.9292e-06 * x1 - 1.9849e-05 * x2 + 0.99999 * x3 + volt_mode*T;
    
            x1 = x1_nxt;
            x2 = x2_nxt;
            x3 = x3_nxt;

            printf("%.6f,%.6f,%.6f,%.6f\n", tempo, x1, x2, u);
            sleep_us(20);
        }
        printf("FIM\n");
    }
    *stop = true;
}

#endif