# Controle de LED WS2812 com Botões no Raspberry Pi Pico

Este projeto demonstra o controle de uma matriz de LEDs WS2812 (RGB) organizada em uma grade 5x5 para exibir dígitos numéricos. A interação é feita através de dois botões, que permitem incrementar ou decrementar o dígito mostrado, com variação de cores conforme a ação.

---

## 📋 Funcionalidades

- **Exibição de Dígitos (0 a 9):**  
  - Cada dígito é representado por um desenho específico mapeado para os 25 LEDs da matriz.
  
- **Interação via Botões:**
  - **Botão A:** Incrementa o dígito exibido e atualiza o display com a cor vermelha.
  - **Botão B:** Decrementa o dígito exibido e atualiza o display com a cor azul.
  
- **Feedback Visual:**  
  - Cada mudança no dígito acende os LEDs correspondentes com uma cor definida.
  
- **Uso do PIO (Programmable I/O):**  
  - Controle eficiente dos LEDs WS2812 utilizando o módulo PIO do Raspberry Pi Pico.

- **Debounce de Botões:**  
  - Implementado para evitar múltiplos acionamentos indesejados, considerando um intervalo mínimo de 200 ms entre as interrupções.

---

## 🛠 Hardware

| Componente             | GPIO no Pico        | Observações                          |
|------------------------|---------------------|--------------------------------------|
| **Matriz LED WS2812**  | Pino 7              | Conectado à cadeia de 25 LEDs        |
| **Botão A**            | Pino 5              | Configurado com resistor de pull-up  |
| **Botão B**            | Pino 6              | Configurado com resistor de pull-up  |
| **LEDs Indicadores**   | Pinos 13, 11, 12    | LEDs externos para feedback (vermelho *, verde e azul)|

* *Vermelha é a utilizada
---

## 🔢 Mapeamento da Matriz de LEDs

A matriz LED é organizada em uma grade 5x5. O mapeamento interno do código associa cada posição física da matriz a um índice específico para facilitar o desenho dos dígitos. Por exemplo:

- **Índice de Mapeamento:**  
  ```c
  int mapa_leds[25] = {
      24, 23, 22, 21, 20,  
      15, 16, 17, 18, 19,  
      14, 13, 12, 11, 10,  
      5,  6,  7,  8,  9,  
      4,  3,  2,  1,  0   
  };
  ```
- **Dígitos:**  
  São definidas 10 matrizes booleanas, cada uma representando o desenho de um dígito (de 0 a 9). Cada posição "true" indica que o LED correspondente deve ser aceso.

---

## 💻 Como Funciona

1. **Inicialização:**
   - O programa inicializa os pinos do Raspberry Pi Pico, configurando os pinos dos LEDs, dos botões e reservando um state machine (SM) do PIO para o controle dos LEDs WS2812.
   - As rotinas de interrupção para os botões A e B são configuradas para detectar a borda de descida (quando o botão é pressionado).

2. **Interrupção e Debounce:**
   - Ao pressionar o **Botão A** ou o **Botão B**, a função de callback `gpio_irq_handler` é chamada.
   - Se o intervalo desde o último acionamento for superior a 200 ms, o programa:
     - Incrementa ou decrementa o dígito atual.
     - Atualiza o display da matriz de LEDs com o desenho do dígito correspondente.
     - O **Botão A** utiliza a cor vermelha (255, 0, 0) e o **Botão B** utiliza a cor azul (0, 0, 255).

3. **Atualização da Matriz LED:**
   - A função `set_one_led` percorre os 25 LEDs e, utilizando o mapeamento definido, envia os dados de cor para cada LED através do PIO.
   - A conversão de cor é feita com a função `urgb_u32`, e o envio para o LED é realizado pela função `put_pixel`.

4. **Feedback com LEDs Externos:**
   - Durante a execução do loop principal, um LED (no pino 13, configurado como indicador vermelho) pisca para sinalizar que o sistema está ativo.

---

## 📂 Estrutura do Código

- **main():**  
  Inicializa hardware, configura o PIO para os LEDs WS2812 e entra em um loop infinito que pisca um LED indicador.

- **Interrupção (`gpio_irq_handler`):**  
  Lida com os acionamentos dos botões, aplica debounce e atualiza o dígito exibido na matriz.

- **Funções Auxiliares:**
  - `put_pixel()`: Envia dados de cor para um único LED.
  - `urgb_u32()`: Converte valores RGB para um formato de 32 bits.
  - `set_one_led()`: Mapeia o desenho do dígito e atualiza todos os LEDs da matriz.

- **Mapeamento e Desenho dos Dígitos:**  
  Vetores booleanos (`led_digit0` a `led_digit9`) que definem quais LEDs devem ser acesos para formar cada dígito.

---

## 🎥 Demonstração
- **Vídeo de Execução na placa BitDogLab:**  
  https://youtu.be/piZbkzdJ-M8?si=CmuiUHbQv_bcmY5N 

---

## 👥 Desenvolvedor

- Tárcio Santos
