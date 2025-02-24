/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#ifndef SELECTING_H
#define SELECTING_H

#include "setup.h"
#include "ws2812.h"
#include "operation_mode_screen.h"
#include "simulator_mode_screen.h"

void rect(ssd1306_t *ssd, bool color) {
    ssd1306_fill(ssd, !color); // Limpa ou mostra a tela
    ssd1306_rect(ssd, 0, 0, 128, 64, color, !color); // Retângulo padrão
}

void operation_mode_select(ssd1306_t *ssd) {
    rect(ssd,true);
    ssd1306_draw_string(ssd, "MODO DE", 35, 20);
    ssd1306_draw_string(ssd, "OPERACAO", 31, 30);
    ssd1306_send_data(ssd); // Atualiza o display 
}

void simulation_mode_select(ssd1306_t *ssd, bool color, double voltage, double current, double volt_mode, bool *stop) {
    char str[10];  // Buffer para armazenar a string

    *stop = false;

    rect(ssd,color);
    ssd1306_draw_string(ssd, "SIMULADOR", 30, 10);

    sprintf(str, "%.2f", voltage);

    ssd1306_draw_string(ssd, str, 10, 30);
    ssd1306_draw_char(ssd, 'V', 45, 30);

    sprintf(str, "%.2f", current);

    ssd1306_draw_string(ssd, str, 75, 30);
    ssd1306_draw_char(ssd, 'A', 110, 30);

    sprintf(str, "%.2f", volt_mode);

    ssd1306_draw_string(ssd, "REF:", 20, 50);   
    ssd1306_draw_string(ssd, str, 65, 50);
    ssd1306_draw_char(ssd, 'V', 100, 50);

    ssd1306_send_data(ssd); // Atualiza o display 
}

void voltage_selection_mode_select(ssd1306_t *ssd, bool color, double volt_mode) {
    char str[10];  // Buffer para armazenar a string

    rect(ssd,color);
    ssd1306_draw_string(ssd, "TENSAO:", 40, 20);

    sprintf(str, "%.2f", volt_mode);

    ssd1306_draw_string(ssd, str, 43, 40);
    ssd1306_draw_char(ssd, 'V', 78, 40);

    ssd1306_send_data(ssd); // Atualiza o display 
}

void bootloader_mode_select(ssd1306_t *ssd) {
    char str[10];  // Buffer para armazenar a string

    rect(ssd,true);
    ssd1306_draw_string(ssd, "MODO DE", 35, 20);
    ssd1306_draw_string(ssd, "GRAVACAO", 31, 30);
    ssd1306_send_data(ssd); // Atualiza o display
}

void bootloader_mode_screen(ssd1306_t *ssd) {
    printf("Saindo para o modo de gravação...\n\n");

    ssd1306_fill(ssd, false); // Limpa a tela
    ssd1306_draw_string(ssd, "INICIE A", 28, 20); 
    ssd1306_draw_string(ssd, "CONEXAO USB", 24, 40);
    ssd1306_send_data(ssd); // Envia os dados para o display

    reset_usb_boot(0,0); // Sai para o modo de gravação
}

void mode_selection_screen(ssd1306_t *ssd, bool color, double voltage, double current, double u, double volt_mode, unsigned short int screen, unsigned short int setpoint, bool *stop) {
    (screen==0) ? operation_mode_select(ssd) : (screen==1) ? simulation_mode_select(ssd, color, voltage, current, volt_mode, stop) : (screen==2) ? voltage_selection_mode_select(ssd, color, volt_mode) : 
    (screen==3) ? bootloader_mode_select(ssd) : (screen==4) ? operating_mode(ssd, color, voltage, current, u, volt_mode) : (screen==5) ? simulator_mode(ssd, color, voltage, current, volt_mode, stop, setpoint) : bootloader_mode_screen(ssd);
}

#endif