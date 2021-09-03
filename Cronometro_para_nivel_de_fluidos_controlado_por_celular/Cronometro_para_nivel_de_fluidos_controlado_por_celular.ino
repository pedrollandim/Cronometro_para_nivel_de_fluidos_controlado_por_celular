#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


/* Put your SSID & Password */
const char* ssid = "NomeDaRedeWIFI";
const char* password = "senhaDaRede";



ESP8266WebServer server(80);

//Variaveis para pegar os dados da pagina web
String Argument_Name, Clients_Response;
String minutoConta, segundoConta, TempoCron;
String hora, minuto, segundo, velocidade, vazao,tempogasto;
String hora1, minuto1, segundo1;
String hora2, minuto2, segundo2;

float hora1valor, hora1AUX, minuto1valor, minuto1AUX, segundo1valor, segundo1AUX;
float hora2valor, hora2AUX, minuto2valor, minuto2AUX, segundo2valor, segundo2AUX;
float vazaovalor,velocidadevalor,tempogastovalor;
String h11, m11, s11;


//Variaveis da pagina web ja trabalhadas 
String area, area2;
float areaAUX, areavalor;


//Variaveis para controlar os estados dos botões 
bool LED1status = LOW;
bool LED2status = LOW;
bool LED3status = LOW;
bool LED4status = LOW;
bool LED5status = LOW;
bool LED6status = LOW;

bool ct,ct2,ct3,ct4;


int a;

//Variaveis para as boias 
const byte boiaBaixa = 2;
const byte boiaAlta = 0;


void setup() {



  IPAddress local_ip(192, 168, 1, 1); // IP set to Static
  IPAddress gateway(192, 168, 1, 1); // gateway set to Static
  IPAddress subnet(255, 255, 255, 0); // subnet set to Static


  Serial.begin(9600);


  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(500);

  // Link para entrar no aplicativo http:192.168.1.1/
  server.on("/", HandleClient); // The client connected with no arguments e.g. http:192.168.1.1/
  server.on("/led1on", handle_led1on); // Link para entrar no aplicativo http:192.168.1.1/led1on
  server.on("/led1off", handle_led1off); // Link para entrar no aplicativo http:192.168.1.1/led1off
  server.on("/led2on", handle_led2on);  // Link para entrar no aplicativo http:192.168.1.1/led2on
  server.on("/led2off", handle_led2off);
  server.on("/led3on", handle_led3on);
  server.on("/led3off", handle_led3off);
  server.on("/led4on", handle_led4on);
  server.on("/led4off", handle_led4off);
  server.on("/result", ShowClientResponse);

  server.begin();

  /* Variaaveis do cronometro */
  pinMode(boiaBaixa, INPUT_PULLUP); // Habilita o pull up interno do Arduino /// Botão sem resistor
  pinMode(boiaAlta, INPUT_PULLUP); // Habilita o pull up interno do Arduino /// Botão sem resistor
  ct=LOW;
  ct2=LOW;
  ct3=LOW;
  ct4=LOW;
  a=0;
  tempogastovalor=0;
  velocidadevalor=0;        
  vazaovalor=0; 
}

void HandleClient() {
  uint8_t led1stat, led2stat, led3stat, led4stat, led5stat, led6stat;
  led1stat = LED1status;
  led2stat = LED2status;
  led3stat = LED3status;
  led4stat = LED4status;
  led5stat = LED5status;
  led6stat = LED6status;

  salvaValores();
  
  String webpage;
  webpage = "<html>";

  /////MODELO GRAFICO 1 
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Cronometro</title>\n";

  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";

 
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  /////////////////////

  
  //webpage += "<head><title>ESP8266 Input Example</title>";

  /////MODELO GRAFICO 2
  /* 
  webpage += "<style>";
  webpage += "body { background-color: #E6E6FA; font-family: Arial, Helvetica, Sans-Serif; Color: blue;}";
  webpage += "</style>";
  */
  /////////////////////
  
  webpage += "</head>";
  webpage += "<body>";

  webpage += "<h3>Tempo: " + tempogasto + "s"+ " </h3>";
  webpage += "<h3>Vazao: " + vazao + "m^3/s"+" </h3>";
  webpage += "<h3>Velocidade: " + velocidade  + "m/s" +" </h3>";
  webpage += "<h3>Area: " + area2 + " m^2" + "\n </h3>"; 


  //webpage += "<form id='myForm' action='http://192.168.1.1' method='POST'>";
  webpage += "<form id='myForm' action='http://192.168.1.1'>";
  
  webpage += "<input type='text' id='user_input1' name='user_input1'>";
  webpage += "<input type='text' id='user_input2' name='user_input2'>";
  webpage += "<input type='text' id='user_input3' name='user_input3'>";
  webpage += "<br><br>\n";
  webpage += "<input type='text' id='user_input' name='user_input'>&nbsp;<input type='submit' value='Enter'>";
  //webpage += "<input type='text' id='user_input' name='user_input'>";
  webpage += "</form>";

  // BOTÕES DE CONTROLE
  if (led1stat)
  {
    webpage += "<p>Contagem permitida</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  }
  else
  {
    webpage += "<p>Contagem bloqueada</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  }

  if (led2stat)
  {
    webpage += "<p>Restar</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";
  }
  else
  {
    webpage += "<p>Restar</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";
  }

  if (led3stat)
  {
    webpage += "<p>Sentido do fluxo</p><a class=\"button button-off\" href=\"/led3off\">Descend.</a>\n";
  }
  else
  {
    webpage += "<p>Sentido do fluxo</p><a class=\"button button-on\" href=\"/led3on\">Ascend.</a>\n";
  }

  if (led4stat)
  {
    webpage += "<p>Atualizar </p><a class=\"button button-off\" href=\"/led4off\">OFF</a>\n";

  }
  else
  {
    webpage += "<p>Atualizar</p><a class=\"button button-on\" href=\"/led4on\">ON</a>\n";
  }
  //////////////////////
  
if (led1stat)
  {
   
  webpage += "<script>\n";
  webpage += "var horaMEDIDA;\n";
  webpage += "var minutoMEDIDA;\n";
  webpage += "var segundoMEDIDA;\n";
  webpage += "var d = new Date();\n";
  webpage += "horaMEDIDA = d.getHours();\n";
  webpage += "minutoMEDIDA = d.getMinutes();\n";
  webpage += "segundoMEDIDA = d.getSeconds();\n";

  webpage += "teste = d.getHours();\n";
  webpage += "user_input1.value=horaMEDIDA;\n";
  webpage += "user_input2.value=minutoMEDIDA;\n";
  webpage += "user_input3.value=segundoMEDIDA;\n";
  webpage += "document.getElementById('myForm').submit();\n";
  webpage += "</script>\n";
  delay(800);
  }



if (led5stat)
  {

     salvaValores2();
  }



if (led6stat)
  {
  salvaValores3();
   
  }  


  webpage += "</body>";
  webpage += "</html>";


  
  
  salvaValores();
  
  server.send(200, "text/html", webpage); // Send a response to the client asking for input


  
 
  
}

/// FUNCAO PARA SALVAR O VALOR DA AREA 
void salvaValores() {
  if (server.args() > 0) { // Arguments were received
    for (uint8_t i = 0; i < server.args(); i++) {
      Argument_Name = server.argName(i);
    
      
      if (server.argName(i) == "user_input") {
        Clients_Response = server.arg(i);
        areaAUX = Clients_Response.toFloat();
        
        if (areaAUX > 0) {
          areavalor = areaAUX;
        }
      }
        

    }


  }

}

/// FUNCAO PARA SALVAR O VALOR DO TEMPO 1 
void salvaValores2() {
  //Serial.println("V11");
  
  if (server.args() > 0) { // Arguments were received
    Serial.print("Salva valores 2: ");
    for (uint8_t i = 0; i < server.args(); i++) {
      Argument_Name = server.argName(i);
      
        if (server.argName(i) == "user_input1") {
          
          hora1 = server.arg(i);
          hora1AUX = hora1.toFloat();//Integer.parseInt(Clients_Response);
          hora1valor = hora1AUX;
          if (LED3status == false){
          ct=HIGH;
          }
          if(LED3status == true){
          ct3=HIGH;
            }
          

        }
        if (server.argName(i) == "user_input2") {
          minuto1 = server.arg(i);
          minuto1AUX = minuto1.toFloat();//Integer.parseInt(Clients_Response);
          minuto1valor = minuto1AUX;

        }
        if (server.argName(i) == "user_input3") {
          segundo1 = server.arg(i);
          segundo1AUX = segundo1.toFloat();//Integer.parseInt(Clients_Response);
          segundo1valor = segundo1AUX;
        }
        
        }

    


  }

}

/// FUNCAO PARA SALVAR O VALOR DO TEMPO 2
/// TEMPO2 - TEMPO 1 = DURAÇÃO DO PROCESSO
void salvaValores3() {
  //Serial.println("V11");
  
  if (server.args() > 0) { // Arguments were received
    Serial.print("Salva valores 3: ");
    for (uint8_t i = 0; i < server.args(); i++) {
      Argument_Name = server.argName(i);
    
      
        if (server.argName(i) == "user_input1") {
          
          hora2 = server.arg(i);
          hora2AUX = hora2.toFloat();//Integer.parseInt(Clients_Response);
          hora2valor = hora2AUX;

          if (LED3status == false){
          ct2=HIGH;
          }
          if(LED3status == true){
          ct4=HIGH;
            }
          
        }
        if (server.argName(i) == "user_input2") {
          minuto2 = server.arg(i);
          minuto2AUX = minuto2.toFloat();//Integer.parseInt(Clients_Response);
          minuto2valor = minuto2AUX;

          
          
        }
        if (server.argName(i) == "user_input3") {
          segundo2 = server.arg(i);
          segundo2AUX = segundo2.toFloat();//Integer.parseInt(Clients_Response);
          segundo2valor = segundo2AUX;
        }
        
        }

  }

}

void calculos(){

  tempogastovalor=((hora2valor-hora1valor)*3600)+((minuto2valor-minuto1valor)*60)+((segundo2valor-segundo1valor));
  velocidadevalor=(0.2)/(tempogastovalor);        
  vazaovalor=(areavalor * velocidadevalor);        

  
  area2 = String(areavalor, 5);          
  tempogasto = String(tempogastovalor, 5);          
  vazao = String(vazaovalor, 5);
  velocidade = String(velocidadevalor, 5);
  Serial.print("area ");
  Serial.println(area2);
  Serial.print("vazao ");
  Serial.println(vazao);       
  }


void ShowClientResponse() {

  String webpage;
  webpage = "<html>";
  webpage += "<head><title>ESP8266 Input Example</title>";
  webpage += "<style>";
  webpage += "body { background-color: #E6E6FA; font-family: Arial, Helvetica, Sans-Serif; Color: blue;}";
  webpage += "</style>";
  webpage += "</head>";
  webpage += "<body>";
  webpage += "<h1><br>ESP8266 Server - This was what the client sent</h1>";
  webpage += "<p>Response received was: " + Clients_Response + "</p>";
  webpage += "</body>";
  webpage += "</html>";
  server.send(200, "text/html", webpage); // Send a response to the client asking for input
}


void loop() {
  server.handleClient();

  if (LED1status)
    { 
      handle_led1on();
      controle();
    }else{
      handle_led1off();
    }
  
  if (LED2status)
    {
      resetarVariaveis();
      handle_led2on();
    }else{
      handle_led2off();
    }


}

void controle(){
server.handleClient();
while (LED1status == true) {
  server.handleClient();

  /*enchendo*/
  if (digitalRead(boiaBaixa) == 0 && LED3status == false && ct == LOW){
     handle_led5on();
      //handle_led5on();
      //salvaValores2();
      Serial.println("BOIA ACIONADA pino tirado 1");
    }else{
      handle_led5off();
      }

   if (digitalRead(boiaAlta) == 0 && LED3status == false && ct2 == LOW){
       handle_led6on();
      //salvaValores3();
    }else{
      handle_led6off();
      }

      /*secando*/
  if (digitalRead(boiaAlta) == 1 && LED3status == true && ct3 == LOW){
      handle_led5on();
      //salvaValores2();
    }else{
      handle_led5off();
      }

   if (digitalRead(boiaBaixa) == 1 && LED3status == true && ct4 == LOW){
      handle_led6on();
      //salvaValores3();
    }else{
      handle_led6off();
      }

      if (LED2status)
    {
      resetarVariaveis();
      handle_led2on();
    }else{
      handle_led2off();
    }
}  
}

////-> CONTROLE DOS ESTADOS DOS BOTÕES 
void handle_led1on() {
  LED1status = HIGH;
  HandleClient();
}
void handle_led1off() {
  LED1status = LOW;
  HandleClient();
}

void handle_led2on() {
  LED2status = HIGH;
  HandleClient();
}
void handle_led2off() {
  LED2status = LOW;
  HandleClient();
}

void handle_led3on() {
  LED3status = HIGH;
  HandleClient();
}
void handle_led3off() {
  LED3status = LOW;
  HandleClient();
}

void handle_led4on() {
  LED4status = HIGH;
  calculos();
  HandleClient();
}
void handle_led4off() {
  LED4status = LOW;
  HandleClient();
}

void handle_led5on() {
  LED5status = HIGH;
  HandleClient();
}
void handle_led5off() {
  LED5status = LOW;
  HandleClient();
}

void handle_led6on() {
  LED6status = HIGH;
  HandleClient();
}
void handle_led6off() {
  LED6status = LOW;
  HandleClient();
}

void resetarVariaveis() {
  ct=LOW;
  ct2=LOW;
  ct3=LOW;
  ct4=LOW;
  hora = "0";
  minuto = "0";
  segundo = "0";
  area = "0";
  areavalor = 0;
  Clients_Response = "0";

tempogasto="0";
vazao="0";
  velocidade="0";
area2="0";

  tempogastovalor=0;
  velocidadevalor=0;        
  vazaovalor=0;     
}
