/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#ifndef SIMULATING_H_H
#define SIMULATING_H_H

#include "setup.h"
#include "ws2812.h"

#define T 20e-6 // 1/50kHz
#define NUM_ITERATIONS 500 // Número de iterações da simulação

void simulator_mode(ssd1306_t *ssd, bool color, double voltage, double current, double mode, bool *stop, unsigned short int setpoint) {
    char str[10];  // Buffer para armazenar a string
    double x1 = current*setpoint, x3 = T*(mode-voltage), x2 = voltage*setpoint, u = 1.7265e-3;

    double a[3] = {((mode==5.0) ? 0.15918 : (mode==3.3) ? 0.14995 : 0.12732), ((mode==5.0) ? 0.061113 : (mode==3.3) ? 0.064964 : 0.072473), ((mode==5.0) ? 27.473 : (mode==3.3) ? 27.541 : 27.708)};
    double b[3] = {((mode==5.0) ? 0.90158 : (mode==3.3) ? 0.90626 : 0.91774), ((mode==5.0) ? 0.011051 : (mode==3.3) ? 0.0089964 : 0.0049409), ((mode==5.0) ? 13.913 : (mode==3.3) ? 13.948 : 14.032)};
    double c[3] = {((mode==5.0) ? 0.18934 : (mode==3.3) ? 0.18934 : 0.18932), ((mode==5.0) ? 0.98933 : (mode==3.3) ? 0.98465 : 0.97318), ((mode==5.0) ? 1.391 : (mode==3.3) ? 1.3922 : 1.395)};
    double d[3] = {((mode==5.0) ? 1.9293e-06 : (mode==3.3) ? 1.9292e-06 : 1.929e-06), ((mode==5.0) ? 1.9897e-05: (mode==3.3) ? 1.9849e-05 : 1.9732e-05), 0.99999};

    if(!*stop) {
        printf("%f\n",mode);
        printf("Tempo(s),x1,x2,u\n");
        for (int k = 0; k < NUM_ITERATIONS; k++) {
            double tempo = k * T;
    
            u = - a[0]*x1 + a[1]*x2 + a[2]*x3;
        
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
    
            double x1_nxt = b[0]*x1 - b[1]*x2 + b[2]*x3;
            double x2_nxt = c[0]*x1 + c[1]*x2 + c[2]*x3;
            double x3_nxt = -d[0]*x1 - d[1]*x2 + d[2]*x3 + mode*T;
    
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