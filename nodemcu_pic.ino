/**
 * Projeto de comunicacao entre NodeMCU e pic.
 * Desenvolvido por Wellington
 * wellingtonteles@gmail.com
 * Desenvolvido para comunicacao com PIC em 2400
 * Seridor comunicando na porta 80
 */
 
#include <ESP8266WiFi.h>
 
const char* ssid = "SSID";
const char* password = "Password";

WiFiServer server(80);
String caminho;
 
void setup() {
  Serial.begin(2400);
  delay(10);
 
  // Connectando to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  caminho = WiFi.localIP().toString();
 
}

void loop() {
  // Checando se o cliente foi conectado
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Esperando para enviar os dados
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Lendo o comando que veio da pagina
  String request = client.readStringUntil('\r');
  String resposta;
  String url;
  Serial.println(request);
  //Seprando a stringo do request
  String cmd = request.substring(9,(request.length()-9));
  //Imprimindo na pagina o comando recebido para o pic
  url += "<p>Comando Enviado = "+cmd+"</p>";
  if(cmd != ""){
    Serial.println(cmd);
    resposta = Serial.read();
    Serial.print("Resposta: ");
    cmd == "";
    resposta = Serial.readString();
    Serial.print("Resposta => ");
    url += "<p>Resposta Enviada = "+resposta+"</p>";
    url += "<p><a href='http://"+caminho+"/resp="+resposta+"' target='_blank'>Link Resposta</a></p>";
    Serial.println(resposta);
  }
  client.flush();
 
  // Pagina de Resposta
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  Esta linha e obrigatoria
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<br><br>");
  client.println(url);
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
