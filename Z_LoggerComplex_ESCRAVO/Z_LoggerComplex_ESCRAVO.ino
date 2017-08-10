void setup() {
  // put your setup code here, to run once:
  Serial.begin(2400);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0)
  {
    unsigned int freq;
    double Z_Val;
    double phase;

    //Serial.println(Serial.readString());

    if(Serial.parseInt() == 'P'){//Os dados estao prontos para serem recebidos
      Serial.println("DADOS PRONTOS");

      freq = Serial.parseInt();
      Z_Val = Serial.parseFloat();
      phase = Serial.parseFloat();
      Serial.println("Freq: " +  (String)freq);
      Serial.println("Z_Val: " + (String)Z_Val);
      Serial.println("Phase: " + (String)phase);  
    }
    delay(100);
  }
}
