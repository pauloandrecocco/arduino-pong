# Arduino Pong

Jogo Pong feito para Arduino.

O Pong é um jogo clássico que simula uma partida de tênis de mesa. Esta implementação para Arduino conta com:
- 3 dificuldades de jogo:
  - _Fácil_: Raquete de tamanho 3, velocidade da bola lenta;
  - _Médio_: Raquete de tamanho 3, velocidade da bola média;
  - _Difícil_: Raquete de tamanho 2, velocidade da bola rápida.
- Possibilidade de pausar a partida;
- Contador de pontuação (de 0 a 99), que é mostrado quando o jogo é pausado ou quando algum jogador marca um ponto;
- Efeitos sonoros com Buzzer (sons diferentes quando a bolinha bate na parede, na raquete e quando um jogador marca um ponto).

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/159200138-f976fd49-e6dc-498b-9a17-1d912f067497.gif" alt="animated" />
</p>

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

## Esquemático do Circuito
<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/159200724-54ce7728-a6fd-4073-9356-1df63559a87d.jpg" />
</p>

## Lógica de Funcionamento do Jogo

### "Campo de Jogo"

A partida acontece de fato dentro de uma matriz binária (booleana) de tamanho 8x16, que armazena 1 nas posições ocupadas (pelas raquetes e pela bola) e 0 onde é vazio. Sempre que uma raquete é movida ou quando a bola se move, a matriz de jogo é atualizada com as posições corretas. O que acontece dentro da matriz de jogo é passado para os 2 displays de LED 8x8 (juntos totalizam 8x16, o tamanho da matriz de jogo), onde os jogadores "veem" a partida.

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162508804-b2caec16-e7d5-420a-847d-008bbe6ad08d.png"
       width = "500"
       height = auto />
</p>

### Movimento da Bola
A bola pode ter seis possíveis direções de movimento (direita, esquerda, direita para cima, direita para baixo, esquerda para cima e esquerda para baixo).

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162500952-a2233d0c-8332-476e-9cde-8644ff8160dc.png"
       width = "500"
       height = auto />
</p>

Na situação de pontuação zerada (no início do jogo), a direção da bola é aleatória, podendo ir na direção do jogador da esquerda (Player 1) ou da direita (Player 2). Quando um jogador pontua, a bola começa com o outro (por exemplo o Player 1 fez ponto, então a bola começa com o Player 2, ou seja, vai na direção do Player 1).

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162513963-f0c79beb-9bf6-4319-8d65-979f7aaab7ce.png"
       width = "500"
       height = auto />
</p>

E começa a partida! A bola segue a sua trajetória até encontrar um obstáculo (as paredes laterais ou a raquete).

Ao **encontrar a parede**, a bola muda de direção, mas continua avançando no sentido que ia. 

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162502754-35be2d27-646b-4c13-b5a6-d5773a6a4124.png"
       width = "350"
       height = auto />
</p>

Se a bola **encontra a raquete**, ela verifica em qual lugar da raquete ela tocou. Para as dificuldades Fácil e Médio, ao colidir com a raquete, se a bola tocou na parte de cima, a bola será rebatida para cima. Se for na parte de baixo, será rebatida para baixo. Se for na posição central da raquete, será rebatida horizontalmente.

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162506866-9f3aa9bf-48f1-4d62-8338-3f5eff62e5b3.png"
       width = "200"
       height = auto />
</p>

Para o modo Difícil, como a raquete é menor (tamanho 2), a bola é rebatida para uma direção aleatória.

## Funcionamento das Principais Funções e Procedimentos

### mover()

Responsável por mover a bola pelo "campo de jogo". Com base na direção de movimento da bola no momento, verifica o que está prestes a acontecer (rebater na raquete, marcar ponto (sem raquete pela frente), rebater na parede ou seguir se movimentando na mesma direção) e, conforme isto, faz a ação necessária (rebater, marcar ponto ou movimentar a bola).

Segue abaixo um fluxograma do funcionamento na situação em que a bola está se movendo na direção cima/esquerda (1 das 6 situações possíveis de movimento):

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162509734-c137a5e3-580d-4580-b9b4-7ff867c08bb6.png"
       width = "900"
       height = auto />
</p>

### mudar_dificuldade()

Quando chamado, este procedimento muda a dificuldade de jogo (1: Fácil; 2: Médio; 3: Difícil). Ele muda o valor da variável responsável por armazenar a dificuldade, também muda a velocidade da bola (que varia conforme a dificuldade do jogo), além de fazer a indicação visual da dificuldade atual no LED correspondente.

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162511530-c6569dfa-0b81-41c8-afa9-676c5e47f01e.png"
       width = "800"
       height = auto />
</p>

### ponto_1() / ponto_2()

Como o próprio nome sugere, fazem o incremento da pontuação, também reposicionam a bola no meio do "campo" para o recomeço da partida. Também emitem o som característico de "marcar ponto" e "pausam" o jogo rapidamente, para mostrar a pontuação.

### verif_numero(int valor)

Recebe um número do tipo _inteiro_, de 0 a 9, e armazena o "desenho visual" desse número em uma matriz chamada **numero[6][3]** ("transforma" um _inteiro_ em uma matriz).

Por exemplo, se chamarmos verif_numero(3), a variável **numero[6][3]** ficará da seguinte forma:

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162524634-9b7c4f52-418a-4518-9e46-229d409b78c9.png"
       width = "100"
       height = auto />
</p>

### atualiza_lc1() / atualiza_lc2()

Realizam a atualização dos Displays 1 (esquerda) e 2 (direita) (que formam a imagem do "campo de jogo" para os jogadores).

As telas podem exibir duas coisas:
- Tela de jogo (matriz do jogo);
- Tela de pausa (pontuação dos jogadores).

Se o jogo está **pausado**, o Display 1 mostra a pontuação do Player 1 e o Display 2 mostra a pontuação do Player 2. Se o jogo **não está pausado** (ou seja, está sendo jogado), os displays mostram a matriz do jogo (o Display 1 mostra a metade da esquerda da matriz de jogo e o Display 2 mostra a metade da direita).

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162517826-9956b86c-b3e9-4157-be69-55a1a9835b3e.png"
       width = "350"
       height = auto />
</p>

Quando o jogo está **pausado**, a pontuação dos jogadores é "desenhada" na tela, usando _verif_numero()_, que vimos anteriormente. Primeiro o valor da pontuação é "desenhado" na variável *numero[6][3]* (chamando _verif_numero()_ com o valor desejado) e então esta variável (que agora contém o "desenho" do valor a ser representado na tela) é "desenhada" na posição correta na tela. 

Por exemplo, supondo que um jogador tem 32 pontos: para mostrar o número 32, primeiro a variável *numero[6][3]* adquire o "desenho" do número 3 (dezena da pontuação) e esta variável é "desenhada" na tela na posição correspondente à dezena, na sequência *numero[6][3]* adquire o "desenho" do 2 (unidade da pontuação) e é "desenhada" na posição de unidade:

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162526663-95c973ef-4d2e-4276-af57-4569377b46f7.png"
       width = "700"
       height = auto />
</p>

### fc_timer1()

Essa função está ligada ao estouro do Timer1 e é chamada a cada 1ms. É usada para contar tempo de forma "paralela" ao fluxo de execução do programa, incrementando uma variável contadora.

Esta variável contadora só é incrementada quando o jogo não está pausado.

### setup()

Executado quando o Arduino é ligado ou resetado. Aqui são feitas as configurações iniciais, como por exemplo a configuração dos pinos de entrada e saída, a configuração do Timer1 e a função ligada ao seu estouro, também a configuração dos displays de LED 8x8, além de serem feitas atribuições de valores iniciais à determinadas variáveis (por exemplo a pontuação dos jogadores, que começa sendo 0).

### loop()

O loop() faz várias verificações importantes:
- Verificação do movimento da bola (se é o momento certo da bola se mover (se a variável contadora incrementada por fc_timer1() atingiu o valor desejado), a bola é movida);
- Verificação do pressionamento dos botões (pausa e mudança de dificuldade);
- Aferição dos joysticks, para o posicionamento correto das raquetes do jogo.

Segue abaixo um fluxograma do funcionamento do loop():

<p align="center">
  <img src="https://user-images.githubusercontent.com/91160798/162509434-821c086e-cfeb-44ad-bf70-19f76ca8190b.png" />
</p>

## Créditos

Criado por: Paulo André Camargo Cocco, Mateus da Silva Cocco e Willy John Nakamura Goto.
