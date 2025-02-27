# PROJETO FINAL
Repositório para a atividade da unidade 7 e capítulo 1:  Projeto de Sistema Embarcado

__Aluno:__
Lucas Carneiro de Araújo Lima

## ATIVIDADE 
__Descrição:__
Este trabalho tem como objetivo desenvolver um controlador discreto para a regulação da tensão de saída de um conversor buck em condução contínua, utilizando a abordagem do espaço de estados. O microcontrolador RP2040 será responsável por processar os sinais de realimentação de tensão e corrente e gerar um sinal PWM para controlar um transistor MOSFET. Posteriormente, será realizada a prototipação na placa BitDogLab, porém, devido a limitações do protótipo, a simulação substituirá um conversor buck real, reproduzindo suas medições e comportamentos esperados.

<div align="center">
  <img src="https://github.com/user-attachments/assets/bf0fc244-1b9b-4886-9ffa-e20158880269" alt="GIF demonstrativo" width="300"/>
</div>

__Para este trabalho, os seguintes componentes e ferramentas se fazem necessários:__
1) Microcontrolador Raspberry Pi Pico W.
2) Ambiente de trabalho VSCode.
3) LEDs RGB
4) _Display_ SSD1306
5) 3 Botões Pull-Up
6) _Joystick_
7) Ferramenta educacional BitDogLab.
8) Matriz de LEDs 5x5

__O resultado do projeto pode ser assistido através deste link: [U4C8 - Conversores A/D](https://youtu.be/-R63UTq61SA).__

## Instruções de Uso

### 1. Clone o repositório
Abra o terminal e execute o comando abaixo para clonar o repositório em sua máquina:
```bash
git clone https://github.com/LucasCarneiro3301/PROJETOFINAL.git
```

### 2. Configure o ambiente de desenvolvimento
Certifique-se de que o [SDK do Raspberry Pi Pico](https://github.com/raspberrypi/pico-sdk) esteja instalado e configurado corretamente no seu sistema.

### 3. Conexão com a Rapberry Pico
1. Conecte o Raspberry Pi Pico ao seu computador via USB.
2. Inicie o modo de gravação pressionando o botão **BOOTSEL** e **RESTART**.
3. O Pico será montado como um dispositivo de armazenamento USB.
4. Execute através do comando **RUN** a fim de copiar o arquivo `firmware.uf2` para o Pico.
5. O Pico reiniciará automaticamente e executará o programa.

### 4. Observações (IMPORTANTE !!!)
2. Manuseie a placa com cuidado.

## Recursos e Funcionalidades

### 1. Botões

| BOTÃO                            | DESCRIÇÃO                                     | 
|:----------------------------------:|:---------------------------------------------:|
| A                                  | Botão de Navegação                 | 
| B                                  | Botão de Confirmação             | 
| JOYSTICK                                  | Botão de Setpoint              | 

### 2. PWM

O PWM controla o tempo de condução do MOSFET para regular a tensão de saída do conversor buck (representado pelo LED).

### 3. LEDs PWM

Simula o comportamento do sinal de PWM no conversor buck.

### 4. Joystick

O joystick permite variar os valores simulados de tensão de saída e corrente do indutor, que são usados na equação de controle para calcular o ciclo de trabalho do PWM. Cada posição do joystick resulta em um valor diferente de PWM, afetando a intensidade do LED, que simula o chaveamento do transistor MOSFET no conversor buck.

### 5. Display SSD1306

O display exibe informações essenciais do sistema, como valores de tensão, corrente e ciclo de trabalho do PWM, além de alertas visuais e a tensão de referência selecionada. No Modo de Operação, ele apresenta os valores medidos e indica alertas em caso de sobrecarga. No Modo de Simulação, mostra a evolução das variáveis ao longo das iterações. No Modo de Seleção de Tensão, exibe as opções de tensão de referência disponíveis. No Modo de Gravação, sinaliza o processo de reinicialização do programa.

### 6. Interrupções

O programa implementa interrupção nos botões A, B e Joystick. Dessa forma, não é necessário ler continuamente o estado de cada botão.

### 7. Deboucing

O programa implementa tratamento de _boucing_ (via _software_). Dessa forma, evita leituras falsas de botões causadas por oscilações elétricas, garantindo que cada pressionamento seja registrado apenas uma vez

### 8. Matriz 5x5

A matriz de LEDs exibe alertas visuais no sistema. Quando a tensão ou a corrente ultrapassa os limites máximos estabelecidos, um ‘X’ vermelho aparece na matriz, indicando uma falha e que o PWM foi desativado. Esse alerta reforça a segurança do sistema ao informar o usuário sobre condições anômalas.

### 9. Temporização

A temporização garante que as leituras dos valores analógicos do joystick ocorram periodicamente a cada 20 µs. Isso permite a atualização contínua dos valores de tensão e corrente, possibilitando o cálculo do erro e a aplicação do controle discreto.

### 10. Monitorador Serial (Executável)

O executável Monitorador Serial recebe os dados enviados pelo Modo de Simulação via conexão USB e os processa para visualização. Ele captura valores de amostra, tensão, corrente e ciclo de trabalho em formato CSV, permitindo ao usuário copiar os dados e gerar gráficos. Além disso, o monitorador gera automaticamente gráficos de evolução dos sinais, facilitando a análise do comportamento do sistema ao longo do tempo.
