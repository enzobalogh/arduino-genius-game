#define LED_VERMELHO 10
#define LED_AZUL 7
#define LED_VERDE 9
#define LED_AMARELO 8

#define INDEFINIDO -1

#define BOTAO_VERMELHO 3
#define BOTAO_VERDE 6
#define BOTAO_AMARELO 5
#define BOTAO_AZUL 4
#define BUZZER 12

#define TAMANHO_SEQUENCIA 4

enum Estados{
	PRONTO_PROX_RODADA,
  	USUARIO_RESPONDE,
  	JOGO_FINALIZADO_SUCESSO,
  	JOGO_FINALIZADO_FALHA
};

int rodada = 0;
int ledsRespondidos = 0;
int sequenciaLuzes[TAMANHO_SEQUENCIA];


void setup()
{
  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo();

}


void iniciaPortas(){
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(BUZZER, OUTPUT); 
}
void tocaBuzzerSucesso(){
  tone(BUZZER,  262); // frequência em Hz
  delay(250);
  noTone(BUZZER); // para o som
  delay(250);
  
    tone(BUZZER,  349); // frequência em Hz
  delay(250);
  noTone(BUZZER); // para o som
  delay(250);
  
    tone(BUZZER,  392); // frequência em Hz
  delay(250);
  noTone(BUZZER); // para o som
  delay(250);
  
    tone(BUZZER,  440); // frequência em Hz
  delay(250);
  noTone(BUZZER); // para o som
  delay(250);
  
    tone(BUZZER,  392); // frequência em Hz
  delay(750);
  noTone(BUZZER); // para o som
  delay(250);
  
    tone(BUZZER,  330); // frequência em Hz
  delay(500);
  noTone(BUZZER); // para o som
  delay(250);
  
  
  
}

void tocaBuzzerFracasso(){
  tone(BUZZER,  330); 
  delay(250);
  noTone(BUZZER); 
  delay(250);
  
    tone(BUZZER,  349); 
  delay(250);
  noTone(BUZZER); 
  delay(250);
  
    tone(BUZZER,  330); 
  delay(250);
  noTone(BUZZER); 
  delay(250);
  
    tone(BUZZER,  294); 
  delay(250);
  noTone(BUZZER); 
  delay(250);
  
    tone(BUZZER,  262); 
  delay(750);
  noTone(BUZZER); 
  delay(250);
}

void iniciaJogo(){
  int jogo = analogRead(0);
  randomSeed(jogo);
   
  for(int i = 0;i<TAMANHO_SEQUENCIA;i++){
	sequenciaLuzes[i] = sorteiaCor();
   int led_sorteado = checaRespostaJogador();
    if(led_sorteado == sequenciaLuzes[i]){
    }
  }
}
int sorteiaCor(){
	return random(LED_AZUL, LED_VERMELHO+1);//7 a 10
}

int estadoAtual(){
  if(rodada <= TAMANHO_SEQUENCIA){
    if(ledsRespondidos == rodada){
       return PRONTO_PROX_RODADA;
      
    } else{
      return USUARIO_RESPONDE;
    }
  } else if(rodada == TAMANHO_SEQUENCIA + 1) {
	return JOGO_FINALIZADO_SUCESSO;
  } else{
      return JOGO_FINALIZADO_FALHA ;
    }
}

void preparaNovaRodada(){
  rodada++;
  ledsRespondidos = 0;
  if(rodada <= TAMANHO_SEQUENCIA){
	tocaLedRodada();
  }
  
    
}

void processaRespostaUsuario(){
  int resposta = checaRespostaJogador();
  if(resposta == INDEFINIDO){
    return;
  }

  if(resposta == sequenciaLuzes[ledsRespondidos]){
    ledsRespondidos++;
  } else {
    Serial.println("ERROU");
    rodada = TAMANHO_SEQUENCIA + 2;
  }
	
}

void processaFalhaUsuario(){
  if(checaRespostaJogador() != sequenciaLuzes[ledsRespondidos]){
    piscaTudo();
    return;
  }
}

void processaSucessoJogador(){
	piscaLed(LED_VERMELHO);
  	piscaLed(LED_VERDE);
  	piscaLed(LED_AMARELO);
  	piscaLed(LED_AZUL);
    delay(500);
}

void loop(){
  switch (estadoAtual()){
    case PRONTO_PROX_RODADA:
	Serial.println("PRONTO_PROX_RODADA");
    preparaNovaRodada();
    break;
    
    case USUARIO_RESPONDE:
	Serial.println("USUARIO_RESPONDE"); 
    processaRespostaUsuario();
    break;
    
    case JOGO_FINALIZADO_SUCESSO:
   	Serial.println("JOGO_FINALIZADO_SUCESSO");
    processaSucessoJogador();
    tocaBuzzerSucesso();
	break;
    
    case JOGO_FINALIZADO_FALHA:
    Serial.println("JOGO_FINALIZADO_FALHA");
    processaFalhaUsuario();
    tocaBuzzerFracasso();
	break;
  }
  delay(1000);
 
}

void tocaLedRodada(){
  for(int i = 0;i<rodada;i++){
    piscaLed(sequenciaLuzes[i]);
}
}

int checaRespostaJogador(){
   if(digitalRead(BOTAO_VERDE) == LOW){
    return piscaLed(LED_VERDE);
  }
   if(digitalRead(BOTAO_AMARELO) == LOW){
    return piscaLed(LED_AMARELO);
  }
   if(digitalRead(BOTAO_VERMELHO) == LOW){
    return piscaLed(LED_VERMELHO);
  }
   if(digitalRead(BOTAO_AZUL) == LOW){
    return piscaLed(LED_AZUL);
  }
  return INDEFINIDO;
}

/*
  NOTAS:
  DÓ (C4) → 262 Hz
RÉ (D4) → 294 Hz
MI (E4) → 330 Hz
FÁ (F4) → 349 Hz
SOL (G4) → 392 Hz
LÁ (A4) → 440 Hz
SI (B4) → 494 Hz
  */
void tocaSom(int frequencia){
   tone(BUZZER,  frequencia); 
  delay(250);
  noTone(BUZZER); 
  delay(50);
}

int piscaLed(int portaLed){
  digitalWrite(portaLed, HIGH);
  delay(500);
  digitalWrite(portaLed, LOW);
  delay(500);  
  if(portaLed == LED_VERMELHO){
    tocaSom(262);//DÓ
    
  }else if(portaLed == LED_VERDE){
    tocaSom(294);//RÉ
    
  } else if(portaLed == LED_AMARELO){
    tocaSom(330);//MI
    
  } else if(portaLed == LED_AZUL){
    tocaSom(349);//FÁ
  }
  return portaLed;
}

void piscaTudo(){
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, HIGH);

  delay(1000);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);

  delay(500);
  
}
