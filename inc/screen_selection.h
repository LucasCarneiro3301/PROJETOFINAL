/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#ifndef SELECTION_H
#define SELECTION_H

#include "setup.h"
#include "ws2812.h"
#include "operation_mode_screen.h"
#include "simulation_screen.h"

// Função para desenhar um retângulo no display
void rect(ssd1306_t *ssd, bool color) {
    ssd1306_fill(ssd, !color); // Limpa ou preenche a tela
    ssd1306_rect(ssd, 0, 0, 128, 64, color, !color); // Desenha um retângulo padrão
}

// Tela de seleção do modo de operação
void operation_mode_select_screen(ssd1306_t *ssd) {
    rect(ssd, true);
    ssd1306_draw_string(ssd, "MODO DE", 35, 20);
    ssd1306_draw_string(ssd, "OPERACAO", 31, 30);
    ssd1306_send_data(ssd); // Atualiza o display 
}

// Tela do modo simulador, exibe tensão, corrente e referência
void simulator_mode_screen(ssd1306_t *ssd, bool color, double voltage, double current, double volt_mode, bool *stop) {
    char str[10];  // Buffer para armazenar a string

    *stop = false; // Garante que a variável stop é inicializada como falso

    rect(ssd, color);
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

// Tela de seleção de tensão
void voltage_selection_mode_screen(ssd1306_t *ssd, bool color, double volt_mode) {
    char str[10];  // Buffer para armazenar a string

    rect(ssd, color);
    ssd1306_draw_string(ssd, "TENSAO:", 40, 20);

    sprintf(str, "%.2f", volt_mode);
    ssd1306_draw_string(ssd, str, 43, 40);
    ssd1306_draw_char(ssd, 'V', 78, 40);

    ssd1306_send_data(ssd); // Atualiza o display 
}

// Tela do modo de gravação
void bootloader_mode_screen(ssd1306_t *ssd) {
    rect(ssd, true);
    ssd1306_draw_string(ssd, "MODO DE", 35, 20);
    ssd1306_draw_string(ssd, "GRAVACAO", 31, 30);
    ssd1306_send_data(ssd); // Atualiza o display
}

// Tela de solicitação de conexão USB para gravação
void connection_request_screen(ssd1306_t *ssd) {
    printf("Saindo para o modo de gravação...\n\n");

    ssd1306_fill(ssd, false); // Limpa a tela
    ssd1306_draw_string(ssd, "INICIE A", 28, 20); 
    ssd1306_draw_string(ssd, "CONEXAO USB", 24, 40);
    ssd1306_send_data(ssd); // Envia os dados para o display

    reset_usb_boot(0,0); // Sai para o modo de gravação
}

// Seleciona qual tela exibir com base no valor da variável 'screen'
void mode_selection_screen(ssd1306_t *ssd, bool color, double voltage, double current, double u, double volt_mode, unsigned short int screen, unsigned short int setpoint, bool *stop) {
    (screen == 0) ? operation_mode_select_screen(ssd) : 
    (screen == 1) ? simulator_mode_screen(ssd, color, voltage, current, volt_mode, stop) : 
    (screen == 2) ? voltage_selection_mode_screen(ssd, color, volt_mode) : 
    (screen == 3) ? bootloader_mode_screen(ssd) : 
    (screen == 4) ? operating_mode(ssd, color, voltage, current, u, volt_mode) : 
    (screen == 5) ? simulation_screen(ssd, color, voltage, current, volt_mode, stop, setpoint) : 
    connection_request_screen(ssd);
}

#endif
