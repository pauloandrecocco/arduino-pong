// Criado por: Paulo André Camargo Cocco, Mateus da Silva Cocco e Willy John Nakamura Goto

// --- Inclusão das bibliotecas ---
#include <TimerOne.h>
#include "LedControl.h"

// --- Declaração das variáveis ---

// Portas do Arduino
const byte pin_led_verde = 2,
           pin_led_amarelo = 3,
           pin_led_vermelho = 4,
           pin_bot_pausa = 5,
           pin_bot_dif = 6, // Botão que muda a dificuldade
           // Display 1
           pin_din_d_1 = 7,
           pin_cs_d_1 = 8,
           pin_clk_d_1 = 9,
           // Display 2
           pin_din_d_2 = 10,
           pin_cs_d_2 = 11,
           pin_clk_d_2 = 12,
           // Pino do buzzer
           pin_buzzer = 13,
           // Joystick 1
           pin_pot_1 = 14 /*A0*/,
           // Joystick 2
           pin_pot_2 = 15 /*A1*/;

bool matriz_jogo[8][16]; // Matriz em que o jogo será de fato jogado. As telas só mostram o que acontece na matriz

byte numero[6][3],
     um[6][3] = 
     {
       {0, 0, 1},
       {0, 0, 1},
       {0, 0, 1},
       {0, 0, 1},
       {0, 0, 1},
       {0, 0, 1}
     },
     dois[6][3] =
     {
       {1, 1, 1},
       {0, 0, 1},
       {1, 1, 1},
       {1, 0, 0},
       {1, 0, 0},
       {1, 1, 1}
     },
     tres[6][3] =
     {
       {1, 1, 1},
       {0, 0, 1},
       {1, 1, 1},
       {0, 0, 1},
       {0, 0, 1},
       {1, 1, 1}
     },
     quatro[6][3] =
     {
       {1, 0, 1},
       {1, 0, 1},
       {1, 1, 1},
       {0, 0, 1},
       {0, 0, 1},
       {0, 0, 1}
     },
     cinco[6][3] =
     {
       {1, 1, 1},
       {1, 0, 0},
       {1, 1, 1},
       {0, 0, 1},
       {0, 0, 1},
       {1, 1, 1}
     },
     seis[6][3] =
     {
       {1, 1, 1},
       {1, 0, 0},
       {1, 1, 1},
       {1, 0, 1},
       {1, 0, 1},
       {1, 1, 1}
     },
     sete[6][3] = 
     {
       {1, 1, 1},
       {0, 0, 1},
       {0, 0, 1},
       {0, 0, 1},
       {0, 0, 1},
       {0, 0, 1}
     },
     oito[6][3] =
     {
       {1, 1, 1},
       {1, 0, 1},
       {1, 1, 1},
       {1, 0, 1},
       {1, 0, 1},
       {1, 1, 1}
     },
     nove[6][3] =
     {
       {1, 1, 1},
       {1, 0, 1},
       {1, 1, 1},
       {0, 0, 1},
       {0, 0, 1},
       {1, 1, 1}
     },
     zero[6][3] =
     {
       {1, 1, 1},
       {1, 0, 1},
       {1, 0, 1},
       {1, 0, 1},
       {1, 0, 1},
       {1, 1, 1}
     };
           
int valor_pot_1, // Armazena o valor e depois o map() do valor no momento no potenciômetro 1
    valor_pot_2,
    valor_anterior_pot_1, // Armazena o map() do último valor no potenciômetro 1, para comparar com o map() do valor atual e ver se tem mudança
    valor_anterior_pot_2,
    
    dificuldade, // Vai de 1 a 3
    dificuldade_anterior,
    delays[3] = {180, 100, 60},
    velocidade_jogo, // Armazena um determinado valor de delays[3], para executar a função mover() a cada tempo desses
    pts_1, // Pontos do Player 1
    pts_2, // Pontos do Player 2
    pos_linha, // Linha da matriz onde está a bola
    pos_coluna, // Coluna da matriz onde está a bola
    direcao, // Vai de 1 a 6, indica a direção do movimento
             // 1  _  4
             // 2 |_| 5
             // 3     6
    cont, // Contador, que vai de 0 até velocidade_jogo
    
    buttonState_pausa, // Leitura no momento do botão
    lastButtonState_pausa = LOW,   // Leitura anterior do botão
    buttonState_dif,
    lastButtonState_dif = LOW;

unsigned long lastDebounceTime_pausa = 0,
              lastDebounceTime_dif = 0,
              debounceDelay = 50;

bool pausado, // Indica se o jogo está pausado
     pode_pausar; // Indica se o usuário pode pausar o jogo ou não. Serve para o usuário não atrapalhar alguma execução no momento

// --- Inicialização dos displays de LED ---

//               LedControl(DIN, CLK, CS, nº do display);
LedControl lc1 = LedControl(pin_din_d_1, pin_clk_d_1, pin_cs_d_1, 1);
LedControl lc2 = LedControl(pin_din_d_2, pin_clk_d_2, pin_cs_d_2, 2);

// --- Início do programa ---

// Função que muda a dificuldade do jogo. É um ciclo
void mudar_dificuldade()
{
  switch(dificuldade)
  {
    case 1:
      // Muda a dificuldade para 2 e sinaliza no LED
      dificuldade_anterior = 1;
      dificuldade = 2;
      velocidade_jogo = delays[1];
      digitalWrite(pin_led_verde, LOW);
      digitalWrite(pin_led_vermelho, LOW);
      digitalWrite(pin_led_amarelo, HIGH);
      break;
    case 2:
      // Muda a dificuldade para 3 e sinaliza no LED
      dificuldade_anterior = 2;
      dificuldade = 3;
      velocidade_jogo = delays[2];
      digitalWrite(pin_led_verde, LOW);
      digitalWrite(pin_led_amarelo, LOW);
      digitalWrite(pin_led_vermelho, HIGH);
      break;
    case 3:
      // Volta a dificuldade para 1 e sinaliza no LED
      dificuldade_anterior = 3;
      dificuldade = 1;
      velocidade_jogo = delays[0];
      digitalWrite(pin_led_amarelo, LOW);
      digitalWrite(pin_led_vermelho, LOW);
      digitalWrite(pin_led_verde, HIGH);
      break;
  }
}

// Função que recebe um número, de 0 a 9, e armazena o desenho desse número em um vetor numero[5][3]. para imprimir na tela
void verif_numero(int valor)
{
  switch(valor)
  {
    case 1:
      for(int i = 0; i <= 5; i++)
      {
        for(int j = 0; j <= 2; j++)
        {
          numero[i][j] = um[i][j];
        }
      }
      break;
    case 2:
      for(int i = 0; i <= 5; i++)
      {
        for(int j = 0; j <= 2; j++)
        {
          numero[i][j] = dois[i][j];
        }
      }
      break;
    case 3:
      for(int i = 0; i <= 5; i++)
      {
        for(int j = 0; j <= 2; j++)
        {
          numero[i][j] = tres[i][j];
        }
      }
      break;
    case 4:
      for(int i = 0; i <= 5; i++)
      {
        for(int j = 0; j <= 2; j++)
        {
          numero[i][j] = quatro[i][j];
        }
      }
      break;
    case 5:
      for(int i = 0; i <= 5; i++)
      {
        for(int j = 0; j <= 2; j++)
        {
          numero[i][j] = cinco[i][j];
        }
      }
      break;
    case 6:
      for(int i = 0; i <= 5; i++)
      {
        for(int j = 0; j <= 2; j++)
        {
          numero[i][j] = seis[i][j];
        }
      }
      break;
    case 7:
      for(int i = 0; i <= 5; i++)
      {
        for(int j = 0; j <= 2; j++)
        {
          numero[i][j] = sete[i][j];
        }
      }
      break;
    case 8:
      for(int i = 0; i <= 5; i++)
      {
        for(int j = 0; j <= 2; j++)
        {
          numero[i][j] = oito[i][j];
        }
      }
      break;
    case 9:
      for(int i = 0; i <= 5; i++)
      {
        for(int j = 0; j <= 2; j++)
        {
          numero[i][j] = nove[i][j];
        }
      }
      break;
    case 0:
      for(int i = 0; i <= 5; i++)
      {
        for(int j = 0; j <= 2; j++)
        {
          numero[i][j] = zero[i][j];
        }
      }
      break;
  }
}

// Função que atualiza o que é mostrado no Display 1
void atualiza_lc1()
{
  // lc1.setLed(0, coluna, linha, 1 ou 0)

  // Se jogo não está pausado
  if(pausado == false)
  {
    for(int i = 0; i <= 7; i++)
    {
      for(int j = 0; j <= 7; j++)
      {
        lc1.setLed(0, j, 7 - i, matriz_jogo[i][j]);
      }
    }
  }
  // Então jogo está pausado
  else
  {
    // Limpa o Display 1 e mostra a pontuação do Player 1
    lc1.clearDisplay(0);

    // Mostra o primeiro dígito
    verif_numero(pts_1 / 10);
    // Imprime o número na posição correta na tela
    for(int i = 1; i <= 6; i++)
    {
      for(int j = 0; j <= 2; j++)
      {
        lc1.setLed(0, j, 7 - i, numero[i - 1][j]);
      }
    }
    // Mostra o segundo dígito
    verif_numero(pts_1 - (pts_1 / 10)*10);
    // Imprime o número na posição correta na tela
    for(int i = 1; i <= 6; i++)
    {
      for(int j = 4; j <= 6; j++)
      {
        lc1.setLed(0, j, 7 - i, numero[i - 1][j - 4]);
      }
    }
  }
  
}

// Função que atualiza o que é mostrado no Display 2
void atualiza_lc2()
{
  // lc2.setLed(0, coluna, linha, 1 ou 0)

  // Se jogo não está pausado
  if(pausado == false)
  {
    for(int i = 0; i <= 7; i++)
    {
      for(int j = 0; j <= 7; j++)
      {
        lc2.setLed(0, j, 7 - i, matriz_jogo[i][8 + j]);
      }
    }
  }
  // Então jogo está pausado
  else
  {
    // Limpa o Display 2 e mostra a pontuação do Player 2
    lc2.clearDisplay(0);

    // Mostra o primeiro dígito
    verif_numero(pts_2 / 10);
    // Imprime o número na posição correta na tela
    for(int i = 1; i <= 6; i++)
    {
      for(int j = 1; j <= 3; j++)
      {
        lc2.setLed(0, j, 7 - i, numero[i - 1][j - 1]);
      }
    }
    // Mostra o segundo dígito
    verif_numero(pts_2 - (pts_2 / 10)*10);
    // Imprime o número na posição correta na tela
    for(int i = 1; i <= 6; i++)
    {
      for(int j = 5; j <= 7; j++)
      {
        lc2.setLed(0, j, 7 - i, numero[i - 1][j - 5]);
      }
    }
  }
  
}

// Função que posiciona a raquete do Player 1 na matriz
void pos_raquete_1()
{
  // Zera a coluna da raquete e depois desenha a nova raquete
  for(int i = 0; i <= 7; i++)
  {
    matriz_jogo[i][0] = 0;
  }

  if(dificuldade != 3) // Se a dificuldade é 3, a raquete tem tamanho 2, ou seja, sem essa primeira parte
  {
    matriz_jogo[valor_pot_1 + -1][0] = 1;
  }
  matriz_jogo[valor_pot_1][0] = 1;
  matriz_jogo[valor_pot_1 + 1][0] = 1;
}

// Função que posiciona a raquete do Player 2 na matriz
void pos_raquete_2()
{
  // Zera a coluna da raquete e depois desenha a nova raquete
  for(int i = 0; i <= 7; i++)
  {
    matriz_jogo[i][15] = 0;
  }

  if(dificuldade != 3) // Se a dificuldade é 3, a raquete tem tamanho 2, ou seja, sem essa primeira parte
  {
    matriz_jogo[valor_pot_2 + -1][15] = 1;
  }
  matriz_jogo[valor_pot_2][15] = 1;
  matriz_jogo[valor_pot_2 + 1][15] = 1;
}

// Função ligada à interrupção do Timer1
// Verifica o pressionamento dos botões e as posições dos joysticks
void fc_timer1()
{
  // Incrementa o cont
  // Dá certo o pois o estouro do Timer1 acontece a cada 1 milissegundo, e velocidade_jogo está em milissegundos
  if(pausado == false)
  {
    cont++;
  }
}

// Funções que incrementam a pontuação dos jogadores
void ponto_1()
{
  pts_1++;
  if(pts_1 > 99)
  {
    pts_1 = 0;
  }
  zerar_bola();
  bola_inicio(1);

  tone(pin_buzzer, 880, 300);
  
  pausado = !pausado;
  atualiza_lc1();
  atualiza_lc2();
  delay(1000);
  pausado = !pausado;
}

void ponto_2()
{
  pts_2++;
  if(pts_2 > 99)
  {
    pts_2 = 0;
  }
  zerar_bola();
  bola_inicio(2);

  tone(pin_buzzer, 880, 300);
  
  pausado = !pausado;
  atualiza_lc1();
  atualiza_lc2();
  delay(1000);
  pausado = !pausado;
}

// Função que zera a posição da bola na matriz
void zerar_bola()
{
  matriz_jogo[pos_linha][pos_coluna] = 0;
}

// Função que coloca 1 na nova posição da bola na matriz
void atualizar_bola()
{
  matriz_jogo[pos_linha][pos_coluna] = 1;
}

// Função que posiciona a bola  no (re)começo do jogo
void bola_inicio(int valor)
{
  // 1 começa do lado esquerdo e vai pra esquerda, 2 começa do lado direito e vai pra direita
  // 1 - Player 1 fez ponto, então a bola começa com o Player 2, ou seja, vai na direção do Player 1
  // 2 - Player 2 fez ponto, então a bola começa com o Player 1, ou seja, vai na direção do Player 2
  //    _ 
  // 1 |_| 2
  
  switch(valor)
  {
    case 1:
      pos_linha = 3;
      pos_coluna = 7;
      matriz_jogo[pos_linha][pos_coluna] = 1;
      direcao = 2; // Vai para a esquerda
      break;
    case 2:
      pos_linha = 3;
      pos_coluna = 8;
      matriz_jogo[pos_linha][pos_coluna] = 1;
      direcao = 5; // Vai para a direita
      break;
  }
}

void andar_na_direcao_1()
{
  direcao = 1; // Vai subindo para a esquerda
  zerar_bola();
  pos_linha--;
  pos_coluna--;
  atualizar_bola();

  if((matriz_jogo[pos_linha][pos_coluna - 1] == 1) || (matriz_jogo[pos_linha][pos_coluna + 1] == 1)) // A bola ficou de frente com a raquete
  {
    tone(pin_buzzer, 440, 100); // Faz um barulho de rebater na raquete
  }
  if((pos_linha == 0) || (pos_linha == 7)) // A bola bateu na parede, na parte de cima ou de baixo
  {
    tone(pin_buzzer, 441, 100); // Faz um barulho de bater na parede
  }
}
void andar_na_direcao_2()
{
  direcao = 2; // Vai para a esquerda
  zerar_bola();
  pos_coluna--;
  atualizar_bola();

  if((matriz_jogo[pos_linha][pos_coluna - 1] == 1) || (matriz_jogo[pos_linha][pos_coluna + 1] == 1)) // A bola ficou de frente com a raquete
  {
    tone(pin_buzzer, 440, 100); // Faz um barulho de rebater na raquete
  }
  if((pos_linha == 0) || (pos_linha == 7)) // A bola bateu na parede, na parte de cima ou de baixo
  {
    tone(pin_buzzer, 441, 100); // Faz um barulho de bater na parede
  }
}
void andar_na_direcao_3()
{
  direcao = 3; // Vai descendo para a esquerda
  zerar_bola();
  pos_linha++;
  pos_coluna--;
  atualizar_bola();

  if((matriz_jogo[pos_linha][pos_coluna - 1] == 1) || (matriz_jogo[pos_linha][pos_coluna + 1] == 1)) // A bola ficou de frente com a raquete
  {
    tone(pin_buzzer, 440, 100); // Faz um barulho de rebater na raquete
  }
  if((pos_linha == 0) || (pos_linha == 7)) // A bola bateu na parede, na parte de cima ou de baixo
  {
    tone(pin_buzzer, 441, 100); // Faz um barulho de bater na parede
  }
}
void andar_na_direcao_4()
{
  direcao = 4; // Vai subindo para a direita
  zerar_bola();
  pos_linha--;
  pos_coluna++;
  atualizar_bola();

  if((matriz_jogo[pos_linha][pos_coluna - 1] == 1) || (matriz_jogo[pos_linha][pos_coluna + 1] == 1)) // A bola ficou de frente com a raquete
  {
    tone(pin_buzzer, 440, 100); // Faz um barulho de rebater na raquete
  }
  if((pos_linha == 0) || (pos_linha == 7)) // A bola bateu na parede, na parte de cima ou de baixo
  {
    tone(pin_buzzer, 441, 100); // Faz um barulho de bater na parede
  }
}
void andar_na_direcao_5()
{
  direcao = 5; // Vai para a direita
  zerar_bola();
  pos_coluna++;
  atualizar_bola();

  if((matriz_jogo[pos_linha][pos_coluna - 1] == 1) || (matriz_jogo[pos_linha][pos_coluna + 1] == 1)) // A bola ficou de frente com a raquete
  {
    tone(pin_buzzer, 440, 100); // Faz um barulho de rebater na raquete
  }
  if((pos_linha == 0) || (pos_linha == 7)) // A bola bateu na parede, na parte de cima ou de baixo
  {
    tone(pin_buzzer, 441, 100); // Faz um barulho de bater na parede
  }
}
void andar_na_direcao_6()
{
  direcao = 6; // Vai descendo para a direita
  zerar_bola();
  pos_linha++;
  pos_coluna++;
  atualizar_bola();

  if((matriz_jogo[pos_linha][pos_coluna - 1] == 1) || (matriz_jogo[pos_linha][pos_coluna + 1] == 1)) // A bola ficou de frente com a raquete
  {
    tone(pin_buzzer, 440, 100); // Faz um barulho de rebater na raquete
  }
  if((pos_linha == 0) || (pos_linha == 7)) // A bola bateu na parede, na parte de cima ou de baixo
  {
    tone(pin_buzzer, 441, 100); // Faz um barulho de bater na parede
  }
}

// Função que recebe um número de 1 a 6 e configura para andar na direção correspondente a esse número
void aleat_andar_na_direcao(int numero)
{
  switch(numero)
  {
    case 1:
      andar_na_direcao_1();
      break;
    case 2:
      andar_na_direcao_2();
      break;
    case 3:
      andar_na_direcao_3();
      break;
    case 4:
      andar_na_direcao_4();
      break;
    case 5:
      andar_na_direcao_5();
      break;
    case 6:
      andar_na_direcao_6();
      break;
  }
}

void mover()
{
  // DIREÇÃO 1 ----------------------------------------------------------------
  if(direcao == 1) // Se está subindo pra esquerda
  {
    if(pos_coluna == 1) // Se está na coluna imediatamente antes da coluna da raquete
    {
      if(matriz_jogo[pos_linha][0] == 0) // Se não tem raquete na frente, é ponto do Player 2
      {
        ponto_2();
      }
      else // Tem raquete na frente
      {
        if(pos_linha == 0) // Verificar se a bola está na quina
        {
          andar_na_direcao_6(); // Volta descendo pra direita          
        }
        else // Não está na quina
        {
          if(dificuldade == 3) // Se a dificuldade é 3, a volta é aleatória
          {
            randomSeed(analogRead(7));
            aleat_andar_na_direcao(random(4,7)); // Maior ou igual a 4 e menor que 7
          }
          else
          {
            // Verificar onde está batendo na raquete
            if((matriz_jogo[pos_linha - 1][pos_coluna - 1] == 0) && (matriz_jogo[pos_linha + 1][pos_coluna - 1] == 1)) // Se é o topo da raquete
            {
              andar_na_direcao_4();; // Volta subindo pra direita
            }
            else if((matriz_jogo[pos_linha - 1][pos_coluna - 1] == 1) && (matriz_jogo[pos_linha + 1][pos_coluna - 1] == 1)) // Se é o centro da raquete
            {
              andar_na_direcao_5(); // Vai para a direita
            }
            else // Se é a base da raquete
            {
              andar_na_direcao_6(); // Volta descendo pra direita
            }           
          }
        }
      }
      
    }
    else if(pos_linha == 0) // Se a bola está batendo na parede de cima
    {
      andar_na_direcao_3(); // Muda a direção para descer para a esquerda
    }
    else // Bola pode continuar subindo pra esquerda
    {
      andar_na_direcao_1();
    }
  }
  // DIREÇÃO 2 ----------------------------------------------------------------
  else if(direcao == 2) // Se está indo pra esquerda
  {
    if((pos_coluna - 1) == 0) // Se a próxima posição da bola vai ser onde a raquete pode estar (coluna 0 da matriz)
    {
      // Tem que verificar se a raquete está na frente ou não
      if(matriz_jogo[pos_linha][pos_coluna - 1] == 1) // A raquete está na frente
      {
        if(dificuldade == 3) // Se a dificuldade é 3, a volta é aleatória
        {
          randomSeed(analogRead(7));
          aleat_andar_na_direcao(random(4,7)); // Maior ou igual a 4 e menor que 7
        }
        else
        {
          // Verificar onde está batendo na raquete
          if((matriz_jogo[pos_linha - 1][pos_coluna - 1] == 0) && (matriz_jogo[pos_linha + 1][pos_coluna - 1] == 1)) // Se é o topo da raquete
          {
            andar_na_direcao_4();; // Volta subindo pra direita
          }
          else if((matriz_jogo[pos_linha - 1][pos_coluna - 1] == 1) && (matriz_jogo[pos_linha + 1][pos_coluna - 1] == 1)) // Se é o centro da raquete
          {
            andar_na_direcao_5(); // Vai para a direita
          }
          else // Se é a base da raquete
          {
            andar_na_direcao_6(); // Volta descendo pra direita
          }
        }
      }
      else // A raquete não está na frente, então é ponto do Player 2
      {
        ponto_2();
      }
    }
    else // Ainda pode andar mais para a esquerda
    {
      andar_na_direcao_2();
    }
  }
  // DIREÇÃO 3 ----------------------------------------------------------------
  else if(direcao == 3) // Se está descendo pra esquerda
  {
    if(pos_coluna == 1) // Se está na coluna imediatamente antes da coluna da raquete
    {
      if(matriz_jogo[pos_linha][0] == 0) // Se não tem raquete na frente, é ponto do Player 2
      {
        ponto_2();
      }
      else // Tem raquete na frente
      {
        if(pos_linha == 7) // Verificar se a bola está na quina
        {
          andar_na_direcao_4(); // Volta subindo pra direita          
        }
        else // Não está na quina
        {
          if(dificuldade == 3) // Se a dificuldade é 3, a volta é aleatória
          {
            randomSeed(analogRead(7));
            aleat_andar_na_direcao(random(4,7)); // Maior ou igual a 4 e menor que 7
          }
          else
          {
            // Verificar onde está batendo na raquete
            if((matriz_jogo[pos_linha - 1][pos_coluna - 1] == 0) && (matriz_jogo[pos_linha + 1][pos_coluna - 1] == 1)) // Se é o topo da raquete
            {
              andar_na_direcao_4();; // Volta subindo pra direita
            }
            else if((matriz_jogo[pos_linha - 1][pos_coluna - 1] == 1) && (matriz_jogo[pos_linha + 1][pos_coluna - 1] == 1)) // Se é o centro da raquete
            {
              andar_na_direcao_5(); // Vai para a direita
            }
            else // Se é a base da raquete
            {
              andar_na_direcao_6(); // Volta descendo pra direita
            }
          }
        }
      }
      
    }
    else if(pos_linha == 7) // Se a bola está batendo na parede de baixo
    {
      andar_na_direcao_1(); // Muda a direção para subir para a esquerda
    }
    else // Bola pode continuar descendo  pra esquerda
    {
      andar_na_direcao_3();
    }
  }
  // DIREÇÃO 4 ----------------------------------------------------------------
  else if(direcao == 4) // Se está subindo pra direita
  {
    if(pos_coluna == 14) // Se está na coluna imediatamente antes da coluna da raquete
    {
      if(matriz_jogo[pos_linha][15] == 0) // Se não tem raquete na frente, é ponto do Player 1
      {
        ponto_1();
      }
      else // Tem raquete na frente
      {
        if(pos_linha == 0) // Verificar se a bola está na quina
        {
          andar_na_direcao_3(); // Volta descendo pra esquerda          
        }
        else // Não está na quina
        {
          if(dificuldade == 3) // Se a dificuldade é 3, a volta é aleatória
          {
            randomSeed(analogRead(7));
            aleat_andar_na_direcao(random(1,4)); // Maior ou igual a 1 e menor que 4
          }
          else
          {
            // Verificar onde está batendo na raquete
            if((matriz_jogo[pos_linha - 1][pos_coluna + 1] == 0) && (matriz_jogo[pos_linha + 1][pos_coluna + 1] == 1)) // Se é o topo da raquete
            {
              andar_na_direcao_1();; // Volta subindo pra esquerda
            }
            else if((matriz_jogo[pos_linha - 1][pos_coluna + 1] == 1) && (matriz_jogo[pos_linha + 1][pos_coluna + 1] == 1)) // Se é o centro da raquete
            {
              andar_na_direcao_2(); // Vai para a esquerda
            }
            else // Se é a base da raquete
            {
              andar_na_direcao_3(); // Volta descendo pra esquerda
            }
          }
        }
      }
      
    }
    else if(pos_linha == 0) // Se a bola está batendo na parede de cima
    {
      andar_na_direcao_6(); // Muda a direção para descer para a direita
    }
    else // Bola pode continuar subindo pra direita
    {
      andar_na_direcao_4();
    }
  }
  // DIREÇÃO 5 ----------------------------------------------------------------
  else if(direcao == 5) // Se está indo pra direita
  {
    if((pos_coluna + 1) == 15) // Se a próxima posição da bola vai ser onde a raquete pode estar (coluna 15 da matriz)
    {
      // Tem que verificar se a raquete está na frente ou não
      if(matriz_jogo[pos_linha][pos_coluna + 1] == 1) // A raquete está na frente
      {
        if(dificuldade == 3) // Se a dificuldade é 3, a volta é aleatória
        {
          randomSeed(analogRead(7));
          aleat_andar_na_direcao(random(1,4)); // Maior ou igual a 1 e menor que 4
        }
        else
        {
          // Verificar onde está batendo na raquete
          if((matriz_jogo[pos_linha - 1][pos_coluna + 1] == 0) && (matriz_jogo[pos_linha + 1][pos_coluna + 1] == 1)) // Se é o topo da raquete
          {
            andar_na_direcao_1();; // Volta subindo pra esquerda
          }
          else if((matriz_jogo[pos_linha - 1][pos_coluna + 1] == 1) && (matriz_jogo[pos_linha + 1][pos_coluna + 1] == 1)) // Se é o centro da raquete
          {
            andar_na_direcao_2(); // Vai para a esquerda
          }
          else // Se é a base da raquete
          {
            andar_na_direcao_3(); // Volta descendo pra esquerda
          }
        }
      }
      else // A raquete não está na frente, então é ponto do Player 1
      {
        ponto_1();
      }
    }
    else // Ainda pode andar mais para a direita
    {
      andar_na_direcao_5();
    }
  }
  // DIREÇÃO 6 ----------------------------------------------------------------
  else if(direcao == 6) // Se está descendo pra direita
  {
    if(pos_coluna == 14) // Se está na coluna imediatamente antes da coluna da raquete
    {
      if(matriz_jogo[pos_linha][15] == 0) // Se não tem raquete na frente, é ponto do Player 1
      {
        ponto_1();
      }
      else // Tem raquete na frente
      {
        if(pos_linha == 7) // Verificar se a bola está na quina
        {
          andar_na_direcao_1(); // Volta subindo pra esquerda          
        }
        else // Não está na quina
        {
          if(dificuldade == 3) // Se a dificuldade é 3, a volta é aleatória
          {
            randomSeed(analogRead(7));
            aleat_andar_na_direcao(random(1,4)); // Maior ou igual a 1 e menor que 4
          }
          else
          {
            // Verificar onde está batendo na raquete
            if((matriz_jogo[pos_linha - 1][pos_coluna + 1] == 0) && (matriz_jogo[pos_linha + 1][pos_coluna + 1] == 1)) // Se é o topo da raquete
            {
              andar_na_direcao_1();; // Volta subindo pra esquerda
            }
            else if((matriz_jogo[pos_linha - 1][pos_coluna + 1] == 1) && (matriz_jogo[pos_linha + 1][pos_coluna + 1] == 1)) // Se é o centro da raquete
            {
              andar_na_direcao_2(); // Vai para a esquerda
            }
            else // Se é a base da raquete
            {
              andar_na_direcao_3(); // Volta descendo pra esquerda
            }
          }
        }
      }
      
    }
    else if(pos_linha == 7) // Se a bola está batendo na parede de baixo
    {
      andar_na_direcao_4(); // Muda a direção para subir para a direita
    }
    else // Bola pode continuar descendo pra direita
    {
      andar_na_direcao_6();
    }
  }

  atualiza_lc1();
  atualiza_lc2();
}

// Setup - executa só uma vez, quando liga pela primeira vez o Arduino
void setup()
{
  Serial.begin(9600);
  
  pinMode(pin_led_verde, OUTPUT);
  pinMode(pin_led_amarelo, OUTPUT);
  pinMode(pin_led_vermelho, OUTPUT);
  pinMode(pin_bot_pausa, INPUT);
  pinMode(pin_bot_dif, INPUT);
  pinMode(pin_pot_1, INPUT);
  pinMode(pin_pot_2, INPUT);
  
  Timer1.initialize(1000); // 1000 Hz
  Timer1.attachInterrupt(fc_timer1);

  lc1.shutdown(0,false);
  lc2.shutdown(0,false);
  // Ajusta a intensidade de 0 a 15 (usar < 8)
  lc1.setIntensity(0,2);
  lc2.setIntensity(0,2);
  // Apagar os displays
  lc1.clearDisplay(0);
  lc2.clearDisplay(0);

  pausado = true; // Começa com o jogo pausado
  pode_pausar = true;
  dificuldade = 3; // Começa na dificuldade mais difícil e muda, para ir para a dificuldade 1
  mudar_dificuldade();
  dificuldade_anterior = dificuldade;
  pts_1 = 0;
  pts_2 = 0;
  cont = 0;

  // Começar matriz do jogo zerada
  memset(matriz_jogo, 0, sizeof(matriz_jogo));
  
  valor_pot_1 = 0;
  valor_pot_2 = 0;
  valor_anterior_pot_1 = 0;
  valor_anterior_pot_2 = 0;

  randomSeed(analogRead(7)); // Le o valor da porta A7 para definir o ponto de partida na lista de geração dos números pseudo-aleatórios
  bola_inicio(random(1,3)); // Gera 1 ou 2 (1 <= valor gerado < 3)
  atualiza_lc1();
  atualiza_lc2();
}

void loop()
{
  if(pausado == false)
  {
    if(cont >= velocidade_jogo)
    {
      cont = 0;
      mover();
    }
  }
  
  if(pode_pausar == true)
  {
    // Verifica se o botão de pausa foi pressionado
    int reading_pausa = digitalRead(pin_bot_pausa);
  
    if(reading_pausa != lastButtonState_pausa)
    {
      // Reset the debouncing timer
      lastDebounceTime_pausa = millis();
    }
    if((millis() - lastDebounceTime_pausa) > debounceDelay)
    {
      if (reading_pausa != buttonState_pausa)
      {
        buttonState_pausa = reading_pausa;
        if (buttonState_pausa == HIGH)
        {
          // Botão realmente foi pressionado
          // Muda o estado (pausado <=> rodando) e muda a tela (tela de pausa <=> tela de jogo)
          pausado = !pausado;
          atualiza_lc1();
          atualiza_lc2();
        }
      }
    }
    lastButtonState_pausa = reading_pausa;  
  }

  // Verifica se o botão de mudar a dificuldade foi pressionado
  int reading_dif = digitalRead(pin_bot_dif);
  
  if(reading_dif != lastButtonState_dif)
  {
    // Reset the debouncing timer
    lastDebounceTime_dif = millis();
  }
  if((millis() - lastDebounceTime_dif) > debounceDelay)
  {
    if (reading_dif != buttonState_dif)
    {
      buttonState_dif = reading_dif;
      if (buttonState_dif == HIGH)
      {
        // Botão realmente foi pressionado
        // Muda a dificuldade do jogo
        mudar_dificuldade();
      }
    }
  }
  lastButtonState_dif = reading_dif;

  // --- --- --- --- --- ---
  
  // Se o jogo não está pausado, ou seja, está rodando
  if(pausado == false)
  {
    // Mede o valor do joystick 1 
    valor_pot_1 = analogRead(pin_pot_1);
    if(dificuldade == 3) // Raquete tamanho 2
    {
      valor_pot_1 = map(valor_pot_1, 0, 950, 0, 6);
    }
    else // Raquete tamanho 3
    {
      valor_pot_1 = map(valor_pot_1, 0, 950, 1, 6);
    }
  
    // Compara se valor atual é igual ao anterior e dificuldade anterior é igual à atual. Se pelo menos uma dessas comparações for verdade, deve atualizar a matriz e, consequentemente, a tela
    if((valor_pot_1 != valor_anterior_pot_1) || (dificuldade_anterior != dificuldade))
    {
      valor_anterior_pot_1 = valor_pot_1;
      pos_raquete_1();
      atualiza_lc1();
    }

    // Mede o valor do joystick 2 
    valor_pot_2 = analogRead(pin_pot_2);
    if(dificuldade == 3) // Raquete tamanho 2
    {
      valor_pot_2 = map(valor_pot_2, 0, 950, 0, 6);
    }
    else // Raquete tamanho 3
    {
      valor_pot_2 = map(valor_pot_2, 0, 950, 1, 6);
    }
  
    // Compara se valor atual é igual ao anterior e dificuldade anterior é igual à atual. Se pelo menos uma dessas comparações for verdade, deve atualizar a matriz e, consequentemente, a tela
    if((valor_pot_2 != valor_anterior_pot_2) || (dificuldade_anterior != dificuldade))
    {
      if(dificuldade_anterior != dificuldade)
      {
        // Essa atribuição acontece só aqui pois, se acontecesse para o caso do joystick 1, dificuldade_anterior já seria igual à variável dificuldade, então não entraria no primeiro "if" acima
        dificuldade_anterior = dificuldade;
      }
      valor_anterior_pot_2 = valor_pot_2;
      pos_raquete_2();
      atualiza_lc2();
    }
  }
}