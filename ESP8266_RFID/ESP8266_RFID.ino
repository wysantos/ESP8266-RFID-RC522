/*
 *********************************************************************************************************
 ESTE CODIGO TEM A INTENÇÃO DE MOSTRAR UMA APLICAÇÃO USANDO UM ESP8266-12,
 E UM SHIELD RFID-RC522.
 
 ESTE CODIGO FONTE É LIVRE E SEGUE OS PRINCIPIOS GNU, PODE SER ADAPTADO, NODIFICADO
 PARCIAL OU INTEIRAMENTE SEM PERVIO AVISO POR QUALQUER PESSOA.

 COMIGO FUNCIONOU PERFEITAMENTE SEM PROBLEMA ALGUM, MAS NÃO GARANTO SEU FUNCIONAMENTO,
 FICA POR CONTA E RISCO DE QUEM QUISER USAR, DUVIDAS FAVOR ME CONTATAR NO E-MAIL ABAIXO.

 wysantos@gmail.com 

 Wanderley Santos, Administrador de Sistemas, aficionado por eletronica e programação.
 https://exclusivesystem.com.br 
 ************************************************************************************************************
 *************************PINAGEM****************************************************************************
  
  RFID-RC522   Wemos          Nodemcu
RST/Reset RST  D3  [1*]        D3 [1*]      GPIO-0  [1*]
SPI SS  SDA[3] D8  [2*]        D8 [2*]      GPIO-15 [2*]
SPI MOSI MOSI  D7              D7           GPIO-13
SPI MISO MISO  D6              D6           GPIO-12
SPI SCK SCK    D5              D5           GPIO-14

[1*] (1, 2) Configuracao tipica definindo como RST_PIN no sketch / programa.
[2*] (1, 2) Configuracao, tipica definindo como SS_PIN no sketch / programa.
[3*] O pino SDA pode ser rotulado como SS em algumas placas de MFRC522 / mais antigas, significam a mesma coisa.
******************************************************************************************************************
 */


#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN    D3    
#define SS_PIN     D8   
#define LED        D1 //LED & BUZZER
#define RELE        D2 //LED & BUZZER

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() 
{
  Serial.begin(115200);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  digitalWrite(D1,LOW);
  digitalWrite(D2,HIGH);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
}

void loop() 
{
  // Procura por cartao RFID
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona o cartao RFID
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID:");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "52 69 A9 AB") //UID 1 - Cartao
  {
    Serial.println("LIBERADO!");
    Serial.println();
    digitalWrite(D1, HIGH);     // LIGA LED/BUZZER
    delay(500);              // DELAY /espera 1 segundos
    digitalWrite(D1, LOW);  // DESlIGA LED/BUZZER
    digitalWrite(D2, LOW);     // ATIVA RELE, abre trava solenoide, etc.
    delay(2000);              // DELAY /espera 1 segundos
    digitalWrite(D2, HIGH);  // Desativa RELE, fecha  trava solenoide, ETC.    
  }
  else
  {
    Serial.println("NÃO AUTORIZADO!");
    Serial.println();    
    digitalWrite(D1, HIGH);     // LIGA LED/BUZZER
    delay(400);              // DELAY /espera 1 segundos
    digitalWrite(D1, LOW);  // DESlIGA LED/BUZZER
  }
}
