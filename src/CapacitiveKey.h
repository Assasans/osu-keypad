#include <Arduino.h>
#include <Keyboard.h>
#include <Mouse.h>

#include <CapacitiveSensor.h>

class CapacitiveKey {
	private:
		CapacitiveSensor* sensor;

		char keyboardKey;
		uint8_t mouseButton;

		bool keyReleased = true;

		unsigned int releaseDelay = 20;
		unsigned int releaseTimer;

		unsigned int pressThreshold;

		int ledPin;
		unsigned char ledBrightness;

		unsigned int lastSample;
	public:
		CapacitiveKey(
			uint8_t sendPin, uint8_t receivePin, int ledPin,
			unsigned int pressThreshold,
			char keyboardKey, uint8_t mouseButton,
			unsigned char ledBrightness
		) {
			sensor = new CapacitiveSensor(sendPin, receivePin);
			this->pressThreshold = pressThreshold;
			this->keyboardKey = keyboardKey;
			this->mouseButton = mouseButton;

			this->ledPin = ledPin;
			this->ledBrightness = ledBrightness;

			pinMode(ledPin, OUTPUT);
		}

		~CapacitiveKey() {
			analogWrite(ledPin, 0);
			delete sensor;
		}

		void keyUpdate(bool keyboardActive, bool isMouse) {
			lastSample = sensor->capacitiveSensorRaw(1);
			if(lastSample > pressThreshold) {
				if(keyReleased) {
					analogWrite(ledPin, ledBrightness);
					if(keyboardActive) {
						if(isMouse) Mouse.press(mouseButton);
						else Keyboard.press(keyboardKey);
					}
					keyReleased = false;
				}
				releaseTimer = releaseDelay;
			} else {
				if(keyReleased) return;
				if(releaseTimer == 0) {
					analogWrite(ledPin, 0);

					if(isMouse) Mouse.release(mouseButton);
					else Keyboard.release(keyboardKey);

					keyReleased = true;
				} else releaseTimer--;
			}
		}

		void log() {
			Serial.print("Key '");
			Serial.print(keyboardKey);
			Serial.print("' (Mouse: ");
			Serial.print(mouseButton);
			Serial.print("): ");
			Serial.println(getLastSample());
		}

		unsigned int getLastSample() {
			return lastSample;
		}
};
