/*
    Aluno: Lucas Carneirod e Araújo Lima
*/

#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include <string.h>

// Definição de tipo da estrutura que irá controlar a cor dos LED's
typedef struct {
    double red;
    double green;
    double blue;
} Color;

// Definição de tipo da matriz de leds
typedef Color Led_Matrix[5][5];

PIO pio = pio0;

int sm = 0;

//Frames fixos
Led_Matrix blank = {
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
};
Led_Matrix lower_row = {
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}}  // Linha 4
};
Led_Matrix upper_row = {
    {{0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}  // Linha 4
};
Led_Matrix upper_lower_row = {
    {{0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}}  // Linha 4
};
Led_Matrix upper_mid_lower_row = {
    {{0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}}  // Linha 4
};
Led_Matrix mid_row = {
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}, {0.1, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},  // Linha 4
};

//Definição da cor escolhida
uint32_t color(double b, double r, double g) {
  return (((unsigned char)(g * 255)) << 24) | (((unsigned char)(r * 255)) << 16) | (((unsigned char)(b * 255)) << 8);
}

//Imprimindo o frame do número na matriz de led
void symbol_printing(Led_Matrix* frame){
    for (int i = 4; i >= 0; i--){
        if(i % 2) {
            for (int j = 0; j < 5; j ++) 
                pio_sm_put_blocking(pio, sm, color((*frame)[i][j].blue, (*frame)[i][j].red, (*frame)[i][j].green));
        }else {
            for (int j = 4; j >= 0; j --)
                pio_sm_put_blocking(pio, sm, color((*frame)[i][j].blue, (*frame)[i][j].red, (*frame)[i][j].green));
        }
    }
}

//Forma o frame do número
void symbol_framer(Led_Matrix* frame,Led_Matrix* base,float r, float g, float b, int row, int col, bool clear, bool print) {
    if(clear) memcpy(*frame, *base, sizeof(Led_Matrix));
    (*frame)[row][col].red = r;
    (*frame)[row][col].blue = b;
    (*frame)[row][col].green = g;
    if(print) symbol_printing(frame); 
}

void symbols(char symbol) {
    Led_Matrix frame;

    if(symbol == '1') {
        symbol_framer(&frame,&lower_row,0.1,0,0,1,1,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,0,2,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,1,2,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,2,2,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,2,false,true);
    } else if(symbol == '2') {
        symbol_framer(&frame,&upper_mid_lower_row,0.1,0,0,1,4,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,0,false,true);
    } else if(symbol == '3') {
        symbol_framer(&frame,&upper_mid_lower_row,0.1,0,0,1,4,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,4,false,true);
    } else if(symbol == '4') {
        symbol_framer(&frame,&mid_row,0.1,0,0,0,0,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,0,4,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,1,0,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,1,4,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,4,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,4,4,false,true);
    } else if(symbol == '5') {
        symbol_framer(&frame,&upper_mid_lower_row,0.1,0,0,1,0,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,4,false,true);
    } else if(symbol == '6') {
        symbol_framer(&frame,&upper_mid_lower_row,0.1,0,0,1,0,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,0,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,4,false,true);
    } else if(symbol == '7') {
        symbol_framer(&frame,&upper_row,0.1,0,0,1,4,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,2,3,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,3,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,4,3,false,true);
    } else if(symbol == '8') {
        symbol_framer(&frame,&upper_mid_lower_row,0.1,0,0,1,0,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,1,4,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,0,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,4,false,true);
    } else if(symbol == '9') {
        symbol_framer(&frame,&upper_mid_lower_row,0.1,0,0,1,0,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,1,4,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,4,false,true);
    } else if(symbol == '0') {
        symbol_framer(&frame,&upper_lower_row,0.1,0,0,1,0,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,1,4,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,2,0,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,2,4,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,0,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,4,false,true);
    } else if(symbol == 'x') {
        symbol_framer(&frame,&blank,0.1,0,0,0,0,true,false);
        symbol_framer(&frame,&blank,0.1,0,0,0,4,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,4,0,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,4,4,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,1,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,3,3,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,2,2,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,1,1,false,false);
        symbol_framer(&frame,&blank,0.1,0,0,1,3,false,true);
    } else if(symbol == '*') {
        symbol_framer(&frame,&blank,0,0,0,0,0,true,true);
    }
}

#endif
