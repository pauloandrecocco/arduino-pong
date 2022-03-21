# Arduino Pong
Jogo Pong feito para Arduino.

O Pong é um jogo clássico que simula uma partida de tênis de mesa. Esta implementação para Arduino conta com a opção de 3 dificuldades (fácil/médio/difícil - muda a velocidade da bola) e também possui um contador que contabiliza a pontuação de cada jogador. É possível pausar o jogo (a tela de pausa mostra a pontuação de cada jogador).

![ezgif-5-99eca0899a](https://user-images.githubusercontent.com/91160798/159200138-f976fd49-e6dc-498b-9a17-1d912f067497.gif)

## Componentes Utilizados
- 1x Arduino Nano;
- 2x Display Matricial de LED 8x8 (com módulo MAX7219);
- 2x Módulo Joystick;
- 2x Push Button;
- Resistores:
  - 1x 220Ω (usado nos LEDs);
  - 2x 10kΩ (usados nos botões).
- LEDs:
  - 1x LED Verde (indica Modo de Jogo 1 - Fácil);
  - 1x LED Amarelo (indica Modo de Jogo 2 - Médio);
  - 1x LED Vermelho (indica Modo de Jogo 3 - Difícil).
- 1x Buzzer (para efeitos sonoros);
- 1x Protoboard;
- Jumpers.

## Montagem
![arduino-pong-esquemático_page-0001](https://user-images.githubusercontent.com/91160798/159200724-54ce7728-a6fd-4073-9356-1df63559a87d.jpg)
