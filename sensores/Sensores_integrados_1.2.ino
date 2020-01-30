//-----------------------------------------------------------------------------
/*
Programa de coleta de dados
Versao 1.2
 */
//-----------------------------------------------------------------------------

#include "TimerOne.h"
//-----------------------------------------------------------------------------
//Variaveis do sensor de corrente ACS712
const int pin = A0; //entrada de sinal

const float ADC_SCALE = 1023.0; //valor máximo da leitura do conversor analógico-digital do arduino
const float VREF = 5.0; //tensão de referência do sinal do sensor
const float sensitivity = 0.185; //sensibilidade do sensor de 5A

unsigned long freq_amostragem = 100; //numero de amostras coletadas por periodo de 100ms. Usado para definir o periodo do timer
int leitura = 0; //armazena a leitura analógica do sinal
float corrente = 0; //armazena a corrente lida pelo sensor
int acc_corrente = 0; //acumula as leituras da entrada analógica
unsigned long periodo = 100000; //armazena o periodo do Timer1
int p_zero = 0; //ponto de valor zero. valor entre 0 e 1023
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Variaveis do decoder
const byte interruptPin = 2; //entrada de sinal
unsigned long t_debouncer = 0;
int pulsos = 0; //numero de pulsos realizados dentro do periodo de envio de dados
int enable = 0; //tem valor 1 sempre que uma interrupção precisa ser tratada
unsigned long t_pulso = 0; //armazena o tempo dos pulsos
unsigned long t_prev = 0; //armazena o tempo do pulso anterior
unsigned long t_atual = 0; //armazena o tempo do pulso realizado
unsigned long t_acc = 0; //acumula os tempos dos pulsos realizados por periodo
float vel_angular = 0; //armazena a velocidade angular média 
//-----------------------------------------------------------------------------
//Variaveis do sensor de tensao
const int pinoSensor = A2; //armazena a entrada de sinal
float tensaoEntrada = 0.0; //armazena a tensão de sinal do sensor
float tensao_sensor = 0.0; //armazena a tensão medida pelo sensor
float valorR1 = 30000.0; //resistor 1 do divisor de tansão
float valorR2 = 7500.0; //resistor 2 do divisor de tensão
int leituraSensor = 0; //armazena a leitura da entrada analógica
float acc_tensao = 0; //acumula os valores das amostras de tensao lidos dentro do periodo
//-----------------------------------------------------------------------------


int cont = 0; //armazena o numero de amostras coletadas

//calibra o valor do ponto de 0A
int calibrate() {
  uint16_t acc = 0;
  int zero = 0;
  for (int i = 0; i < 10; i++) {
    acc += analogRead(pin);
  }
  zero = acc / 10;
  return zero;
}


void setup() {
  pinMode(interruptPin, INPUT_PULLUP);//define uma interrupção na entrada do encoder
  pinMode(pinoSensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pulso, RISING); //adiciona uma função para a interrupção
  Serial.begin(9600);
  p_zero = calibrate(); //define o zero da corrente
  Serial.println(p_zero);
  periodo = 100000/freq_amostragem; //periodo do timer definido como 100ms dividido pelo numero de amostras coletadas dentro do mesmo
  Timer1.initialize(periodo); //inicializa o periodo
  Timer1.attachInterrupt(callback);
}

//funcao executada ao final de cada periodo do timer
void callback()
{
  //-----------------------------------------------------------------------------
  //amostragem dos sensores analógicos
  cont++; 
  leitura = analogRead(pin);
  acc_corrente += leitura - p_zero; //acumula os valores da leitura analógica do sinal do sensor subtraidos pelo valor do ponto de 0A
  leituraSensor = analogRead(pinoSensor);
  tensaoEntrada = (leituraSensor * 5.0) / 1023.0; //converte os valores de tensao lidos da entrada analogica para valores do sinal de tensão entre 0 e 5V
  acc_tensao += tensaoEntrada / (valorR2/(valorR1+valorR2)); //converte o sinal de tensão no valor de tensão lido pelo sensor
  //-----------------------------------------------------------------------------
  
  //-----------------------------------------------------------------------------
  //executado apenas quando foi alcançado o tempo para envio dos dados
  if(cont == freq_amostragem)//quando todas as amostras forem coletadas é feito o tratamento e envio dos dados
  {
    corrente = (float)acc_corrente / (float)freq_amostragem / ADC_SCALE * VREF / sensitivity; //calcula o valor medio da entrada analogica do sensor de corrente e converte em corrente em A
    tensao_sensor = acc_tensao/(float)freq_amostragem; //calcula o valor medio da tensao
    if(pulsos != 0)
    {
      vel_angular = 60 * 1000000.0/((float)t_acc/pulsos); //calcula o valor medio da velocidade angular
    }else {
      vel_angular = 0; //caso nenhum pulso seja detectado dentro do periodo 
    }
    acc_corrente = 0;
    acc_tensao = 0;
    cont = 0;
    //formato de impressao dos valores na porta serial |tensao|corrente|velocidade_angular|
    Serial.print(tensao_sensor);
    Serial.print("|");
    Serial.print(corrente);
    Serial.print("|");
    Serial.print(vel_angular);
    Serial.print("|");
    Serial.println("");
    pulsos = 0;
    t_acc = 0;
  }
  //-----------------------------------------------------------------------------
}

//funcao executada para cada pulso realizado
void pulso() {
  t_debouncer = micros(); //armazena o tempo em que a interrupcao ocorreu
  enable = 1;
  t_atual = micros(); //armazena o tempo em que a interrupcao ocorreu
}

void loop() {
  //-----------------------------------------------------------------------------
  //tratamento das interrupcoes
  if((micros() - t_debouncer) >= 50 && enable == 1){
    enable = 0;
    if(digitalRead(interruptPin) == HIGH){
      pulsos++;
      t_pulso = t_atual - t_prev; //calcula o tempo de duracao do pulso
      t_acc = t_acc + t_pulso; //acumula o tempo realizado pelos pulsos realizados dentro do periodo
      t_prev = t_atual; //armazena o tempo em que o ultimo pulso ocorreu
    }
  }
  //-----------------------------------------------------------------------------
}
