#include <LiquidCrystal_I2C.h>

// Declarando variáveis dos componentes
LiquidCrystal_I2C lcd(0x27, 20, 4);

int trigger01 = 13;
int echo01 = 12;
int trigger02 = 11;
int echo02 = 10;

int ledBar[] = {9, 8, 7, 6, 5, 4, 3, 2, A3, A2};
int numLeds = 10;

// Declarando variáveis
float leitura01 = 0;
float leitura02 = 0;
float cm01 = 0;
float cm02 = 0;

bool leitura01Iniciou = false;
bool leitura02Iniciou = false;

unsigned long tempoInicial = 0;
unsigned long tempoFinal = 0;
float intervalo = 0;
float distancia = 100; // Distancia entre os dois sensores (em metros)
float velocidadeMps = 0;
float velocidadeKph = 0;

void setup() {
  // Iniciando o serial
  Serial.begin(9600);

  // Iniciando o LCD
  lcd.init();
  lcd.backlight();

  // Declarando os pinModes dos componentes
  pinMode(trigger01, OUTPUT);
  pinMode(echo01, INPUT);
  pinMode(trigger02, OUTPUT);
  pinMode(echo02, INPUT);

  for(int i = 0; i < numLeds; i++) {
    pinMode(ledBar[i], OUTPUT);
  }

  Serial.println("Pronto para iniciar medições");
}

void loop() {
  // Iniciando o sensor de distancia ultrasonico 01
  digitalWrite(trigger01, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger01, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigger01, LOW);

  // Captando dados dos sensores 01
  leitura01 = pulseIn(echo01, HIGH);

  // Calculando e transformando unidades de medida do sensor 01
  cm01 = leitura01 / 58.0;

  // Checando se a leitura foi iniciada no sensor 01
  if(cm01 <= 300 && !leitura01Iniciou) {
    delay(50);
    if(cm01 <= 300) {
      tempoInicial = millis();
      leitura01Iniciou = true;
      Serial.println("Leitura do Sensor 01 Iniciada");
    }
  }

  // Iniciando o sensor de distancia ultrasonico 02
  digitalWrite(trigger02, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger02, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigger02, LOW);

  // Captando dados do sensor 02
  leitura02 = pulseIn(echo02, HIGH);

  // Calculando e transformando unidades de medida do sensor 02
  cm02 = leitura02 / 58.0;

  // Checando se a leitura foi iniciada no sensor 02
  if(cm02 <= 300 && leitura01Iniciou && !leitura02Iniciou) {
    delay(50);
    if(cm02 <= 300) {
      tempoFinal = millis();
      leitura02Iniciou = true;
      Serial.println("Leitura do Sensor 02 Iniciada");

      // Calculo do intervalo de tempo
      intervalo = (tempoFinal - tempoInicial) / 1000.0;
      Serial.print("Intervalo de Tempo: ");
      Serial.print(intervalo);
      Serial.println(" segundos");

      // Calculo da velocidade
      velocidadeMps = distancia / intervalo;
      velocidadeKph = (float)(distancia / 1000.0) / (float)(intervalo / 3600.0);
      Serial.print("Velociade (m/s): ");
      Serial.print(velocidadeMps);
      Serial.println(" m/s");
      Serial.print("Velociade (km/h): ");
      Serial.print(velocidadeKph);
      Serial.println(" km/h");

      // Resetar estados das leituras
      leitura01Iniciou = false;
      leitura02Iniciou = false;

      // Exibindo velociade no LCD
      lcd.setCursor(0, 0);
      lcd.print("Velocidade:");
      exibirLCD(0, 1, velocidadeMps, ">", "m/s");
      exibirLCD(0, 2, velocidadeKph, ">", "km/h");
      delay(500);
    }
  }

  // Bloco de verificação para ligar a LEDbar
  if(velocidadeMps != 0 && velocidadeKph != 0) {
    ligarLEDbar(velocidadeKph, ledBar);
    delay(500);
  }
}

// Função para apagar linha do LCD
void apagarLinha(int coluna, int linha) {
  lcd.setCursor(coluna, linha);
  for (int i = coluna; i < (20 - coluna); i++) {
    lcd.print(" ");
  }
  lcd.setCursor(coluna, linha);
}

// Função para exibir informações no LCD
void exibirLCD(int coluna, int linha, float variavel, String mensagem, String medida) {
  apagarLinha(coluna, linha);
  lcd.setCursor(coluna, linha);
  lcd.print(mensagem);
  lcd.print(" ");
  lcd.print(variavel);
  lcd.print(" ");
  lcd.print(medida);
}

// Função para ligar a LEDbar
void ligarLEDbar(float variavel, int lista[]) {

  // Liga 10 LEDs da LEDbar
  if(variavel >= 300) {
    for(int i = 0; i < 10; i++) {
      digitalWrite(lista[i], HIGH);
    }

  // Liga 8 LEDs da LEDbar 
  } else if(variavel >= 200) {
    for(int i = 0; i < 8; i++) {
      digitalWrite(lista[i], HIGH);
    } 

  // Liga 6 LEDs da LEDbar
  } else if(variavel >= 150) {
    for(int i = 0; i < 6; i++) {
      digitalWrite(lista[i], HIGH);
    }
  
  // Liga 4 LEDs da LEDbar
  } else if(variavel >= 100) {
    for(int i = 0; i < 4; i++) {
      digitalWrite(lista[i], HIGH);
    }

  // Liga 2 LEDs da LEDbar 
  } else if(variavel >= 75) {
    for(int i = 0; i < 2; i++) {
      digitalWrite(lista[i], HIGH);
    }

  // Liga 1 LEDs da LEDbar 
  } else if(variavel >= 50) {
    for(int i = 0; i < 1; i++) {
      digitalWrite(lista[i], HIGH);
    } 
  }
  delay(5000);

  // Desliga 10 LEDs da LEDbar
  for(int i = 0; i < 10; i++) {
    digitalWrite(lista[i], LOW);
  }
}