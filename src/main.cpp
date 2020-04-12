#include <Arduino.h>
#include <Keyboard.h>
#include <Mouse.h>

#include <CapacitiveSensor.h>

#include <JC_Button.h>

#include "CapacitiveKey.h"

#define DISABLE_PIN 12

#define SERIAL_OUTPUT

Button modeSwitch(11);

void setup() {
	#ifdef SERIAL_OUTPUT
	Serial.begin(115200);
	#endif

	Keyboard.begin();
	Mouse.begin();

	modeSwitch.begin();

	pinMode(DISABLE_PIN, INPUT_PULLUP);
}

CapacitiveKey keyZ = CapacitiveKey(
	2,					//Capacitive send pin
	7,					//Capacitive sense pin
	6,					//LED pin
	15,					//Capacitive threshold
	'z',				//Keyboard key
	MOUSE_LEFT, //Mouse button
	32					//LED brightness
);
CapacitiveKey keyX = CapacitiveKey(
	4,					//Capacitive send pin
	8,					//Capacitive sense pin
	10,					//LED pin
	15,					//Capacitive threshold
	'x',				//Keyboard key
	MOUSE_RIGHT,//Mouse button
	32					//LED brightness
);

bool changeCooldown = false;
bool isMouse = false;

void loop() {
	bool keyboardActive = digitalRead(DISABLE_PIN);

	modeSwitch.read();
	if(modeSwitch.isPressed()) {
		analogWrite(6, 128);
	}

	if(modeSwitch.releasedFor(500)) {
		if(changeCooldown) analogWrite(10, 0);
		changeCooldown = false;
	}

	if(!changeCooldown && modeSwitch.pressedFor(500)) {
		analogWrite(10, 128);

		isMouse = !isMouse;
		changeCooldown = true;
	}

	if(!changeCooldown) {
		keyZ.keyUpdate(keyboardActive, isMouse);
		keyX.keyUpdate(keyboardActive, isMouse);
	}

	#ifdef SERIAL_OUTPUT
	keyZ.log();
	keyX.log();
	#endif
}