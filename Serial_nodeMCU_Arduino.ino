String data;

void setup() {
  //VELOCIDADES: 300,1200,2400,4800,9600,14400,19200,28800,38400,57600,115200
  //Creio que usar velocidades muito altas, poder� ter perda dados
  //So testando, para ter certeza
  Serial.begin(4800);
}

//C�DIGO DO SLAVE - NodeMCU
void loop() {
  if(Serial.available()){
    data = Serial.readString();
    if(!data.equals("")){
      Serial.println(data);  
    }
  }
}

//C�DIGO DO MASTER - Arduino
/*void loop() {
  Serial.print("dados");
  Serial.println(millis());
}*/
