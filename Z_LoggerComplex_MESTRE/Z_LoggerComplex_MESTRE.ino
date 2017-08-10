#define TWI_FREQ 100000L
#define cycles_base 511		  // Número máximo de incrementos
#define cycles_multiplier 1   // Ganho PGA
#define start_frequency 50000 // Frequência Inicial
#define inc_freq 100		  // Incremento de Frequência
#define num_inc 11			  // Número de Incrementos
#define cal_samples 10		  // Amostras de Calibração

#define BUTTON_PIN 7

#include <AD5933.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial serialNode(4, 5); //RX,TX ; Pinos 4,5 (Estou utilizando o Arduino UNO)

const int numofIncrement = 100;
const double calResistance = 180000;
double gainFactor, pShift;
double t;
unsigned freq;
int i;

void configurarSensor()
{
	AD5933.setExtClock(false);
	AD5933.resetAD5933();
	AD5933.setStartFreq(start_frequency);
	AD5933.setSettlingCycles(cycles_base, cycles_multiplier);
	AD5933.setStepSize(inc_freq);
	AD5933.setNumofIncrement(num_inc);
	AD5933.setPGA(GAIN_1);
	AD5933.setRange(RANGE_1);
}

void setup()
{
	pinMode(BUTTON_PIN, INPUT);
	t = 0;
	Wire.begin();
	Serial.begin(9600);
	serialNode.begin(2400);
	//'freq' esta sendo redefinido abaixo, mas  ele nao eh utilizado no metodo 'setup'
	//logo, ele ficará no escopo local e nao tera utilidade
	unsigned freq = 0;

	//setByte(0x81, 0x18); // Reset & Use Ext. Clock - 0001 1000
	configurarSensor();
	double temp = AD5933.getTemperature();
#if VERBOSE
	Serial.print("A temperatura é de ");
	Serial.print(temp);
	Serial.println(" graus Celsius.");
#endif
	Serial.println("Configure o sistema para calibração. Ao terminar, pressione 'p' e <Enter>");
	aguardarInterrupcao();
	//gainFactor = AD5933.getGainFactor(calResistance,1);
	AD5933.getGainFactor(calResistance, cal_samples, gainFactor, pShift, false);
	Serial.print("Fator de Ganho: ");
	Serial.println(gainFactor);
	Serial.print("Deslocamento de Fase do Sistema: ");
	Serial.println(pShift);
	delay(500);
	Serial.println("Configure o sistema para medição. Ao terminar, pressione 'p' e <Enter>");
	aguardarInterrupcao();

	//Para marior organizacao, o loop ficou dentro do metodo 'varredura';
	varredura();
}

void loop()
{
}

void aguardarInterrupcao()
{
	while (!digitalRead(BUTTON_PIN))
	{
		//AGUARDANDO PULSO NO BOTÃO
	}
}

void varredura()
{
	for (int i = 0; i < num_inc; i++)
	{
		AD5933.tempUpdate();
		AD5933.setCtrMode(INCR_FREQ);
		double cReal, cImag;
		double Z_Val, phase;
		AD5933.getComplex(gainFactor, pShift, Z_Val, phase);
		freq = start_frequency + inc_freq * t;

		//Ordem de envio: freq,Z_Val,phase
		// 'P' Indica que a node pode trabalhar
		String dados = ((String)'P') + ","((String)freq) + "," + ((String)Z_Val) + "," + ((String)phase);
		//TESTE String dados = ((String)'P') + "," + ((String)freq) + "," + ((String)(i * 2)) + "," + ((String)(i * 3));
		serialNode.println(dados);
		delay(500);
		t++;
	}
}
