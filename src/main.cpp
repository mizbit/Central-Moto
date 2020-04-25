//==========================================================================================
//----------------------------------- MOTO -------------------------------------------------
//==========================================================================================
#include <Arduino.h>
#include <AceButton.h>
using namespace ace_button;

//-------------------------------- Pulsadores-----------------------------------------------
const byte PULSADOR_START_PIN = A0;	 //A0 Pin Arranque
const byte PULSADOR_TURN_R_PIN = A1; //A1 Pin Pulsador Derecho
const byte PULSADOR_TURN_L_PIN = A2; //A2 Pin Pulsador Izquierdo
const byte PULSADOR_LIGHT_PIN = A3;	 //A3 Pin Luz
const byte PULSADOR_BRAKE_PIN = A4;	 // Pulsadores freno
//const byte PULSADOR_AUX_PIN = A5;			// Pulasor Auxiliar

//--------------------------------- Luces --------------------------------------------------
const byte BRAKE_OUT_PIN = 11;	// luz de freno
const byte TURN_L_OUT_PIN = 10; // señalero izquierdo
const byte TURN_R_OUT_PIN = 9;	// señalero derecho
const byte LIGHT_OUT_PIN = 6;	// relé de luz corta
const byte BREAM_OUT_PIN = 5;	// relé de luz alta

//--------------------------------------- Otros --------------------------------------------
const byte IGNITION_OUT_PIN = 3; // Arranque
//const byte bocina = 16;					// Bocina

//------------------------------------- AceButton ----------------------------------------

ButtonConfig turnConfig;
ButtonConfig brakeConfig;

AceButton pulsadorStart(PULSADOR_START_PIN, HIGH);
AceButton pulsadorLight(PULSADOR_LIGHT_PIN, HIGH);
AceButton pulsadorBrake(&brakeConfig, PULSADOR_BRAKE_PIN, HIGH);
AceButton pulsadorTurnR(&turnConfig, PULSADOR_TURN_R_PIN, HIGH);
AceButton pulsadorTurnL(&turnConfig, PULSADOR_TURN_L_PIN, HIGH);

//------------------------------------- Variables ------------------------------------------
//funcion Destellador
boolean signalBlink = LOW;
long tP1 = 0;
long tP2 = 0;
byte sena = 0;

//Funcion Señaleros
byte timeBlink = 200;



//==========================================================================================
//---------------------------------------- Funciones ---------------------------------------
//==========================================================================================

boolean blink(byte TP){								// Funcion Destellados
	tP2 = millis();
	if(tP2 - tP1 > TP){
		tP1 = tP2;
		(signalBlink == LOW) ? signalBlink = HIGH : signalBlink = LOW;
	}  
	return signalBlink;
}

void turn(){
	switch (sena){
		case 0:
			digitalWrite(TURN_R_OUT_PIN, LOW);
			digitalWrite(TURN_L_OUT_PIN, LOW);
			break;
		case 1:
			digitalWrite(TURN_R_OUT_PIN, blink(timeBlink));
			digitalWrite(TURN_L_OUT_PIN, LOW);
			break;
		
		case 2:
			digitalWrite(TURN_L_OUT_PIN, blink(timeBlink));
			digitalWrite(TURN_R_OUT_PIN, LOW);
			break;

		case 3:
			digitalWrite(TURN_R_OUT_PIN, blink(timeBlink));
	    	digitalWrite(TURN_L_OUT_PIN, blink(timeBlink));
			break;
		}
}

void handleEvent(AceButton* Button, uint8_t eventType, uint8_t /* buttonState */){
	switch (eventType){	
		case AceButton::kEventClicked:

			break;

		case AceButton::kEventPressed:

			break;
		}
}
void handleTurnEvent(AceButton* button, uint8_t eventType, uint8_t /* buttonState */){
	switch (eventType){
		case AceButton::kEventClicked:{
			uint8_t pin = button->getPin();
			switch (pin){
				case PULSADOR_TURN_R_PIN:
					sena == 1 ? sena = 0 : sena = 1;
					break;
				
				case PULSADOR_TURN_L_PIN:
					sena == 2 ? sena = 0 : sena = 2;
					break;
			}
			break;
		}
		
		case AceButton::kEventLongPressed:{
			uint8_t pin2 = button->getPin();
			switch (pin2){
				case PULSADOR_TURN_R_PIN:
				case PULSADOR_TURN_L_PIN:
					sena = 3;
					break;
			}
			break;
		}
	}
}

void handleBrakeEvent(AceButton* button, uint8_t eventType, uint8_t /* buttonState */){
	switch (eventType){
		case AceButton::kEventPressed:
			digitalWrite(BRAKE_OUT_PIN, HIGH);
			break;
		
		case AceButton::kEventReleased:
			digitalWrite(BRAKE_OUT_PIN, LOW);
			break;
		}
}

//==========================================================================================
//------------------------------------- Setup ----------------------------------------------
//==========================================================================================

void setup(){

	ButtonConfig *buttonConfig = ButtonConfig::getSystemButtonConfig();
	buttonConfig->setEventHandler(handleEvent);
	buttonConfig->setFeature(ButtonConfig::kFeatureClick);
	buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterClick);

	turnConfig.setEventHandler(handleTurnEvent);
	turnConfig.setFeature(ButtonConfig::kFeatureClick);
	turnConfig.setFeature(ButtonConfig::kFeatureLongPress);

	brakeConfig.setEventHandler(handleBrakeEvent);

	//------------------------------------- Salidas -----------------------
	pinMode(BRAKE_OUT_PIN, OUTPUT);	   // Out Brake
	pinMode(TURN_L_OUT_PIN, OUTPUT);   // Out Turn Left
	pinMode(TURN_R_OUT_PIN, OUTPUT);   // Out Turn Right
	pinMode(LIGHT_OUT_PIN, OUTPUT);	   // Out Light
	pinMode(BREAM_OUT_PIN, OUTPUT);	   // Out Light Bream
	pinMode(IGNITION_OUT_PIN, OUTPUT); // Out Ignition

	//------------------------------------- Entradas ---------------------
	pinMode(PULSADOR_START_PIN, INPUT);	 // Pulsador Start
	pinMode(PULSADOR_TURN_R_PIN, INPUT); // Pulsador Turn Right
	pinMode(PULSADOR_TURN_L_PIN, INPUT); // Pulsador Turn Left
	pinMode(PULSADOR_LIGHT_PIN, INPUT);	 // Pulsador Light
	pinMode(PULSADOR_BRAKE_PIN, INPUT);	 // Pulsador Brake
}

//==========================================================================================
//-------------------------------------- Loop ----------------------------------------------
//==========================================================================================

void loop(){
	pulsadorLight.check();
	pulsadorTurnR.check();
	pulsadorTurnL.check();
	pulsadorBrake.check();
	turn();
}