/*
 Name:		Ampel_reg.ino
 Created:	12/21/2018 12:11:18 PM
 Author:	braya
*/

//header files
#include "PinConfiguration.h"
#include "CustomBinary.h"
#include "StatesEnums.h"

// func declarations
void entryPoint(void);
void TrafficLightStateMachine(void);
void SetCivilLight(stateCivilLight state);
void SetCarLight(stateCarLight state);

// fields
const int _delay = 1000;
const int _delayBlink = 500;

// the setup function runs once when you press reset or power the board
void setup() {
	
	DDRD = 0x00 | pinCarGreen + pinCarOrange + pinCarRed + pinCivilGreen + pinCivilRed;
	PORTD = 0x00;

	SetCarLight(stateCarLight::RED);
	SetCivilLight(stateCivilLight::RED);
}

// the loop function runs over and over again until power down or reset
void loop() {

	entryPoint();
}

// functions
void entryPoint(void)
{
	while(true)
	{
		if ((PIND & pinButton) == LOW)
		{
			TrafficLightStateMachine();
		}
	}
}

void TrafficLightStateMachine(void)
{
	SetCarLight(stateCarLight::RED);
	SetCivilLight(stateCivilLight::GREEN);
	_delay_ms(_delay);

	SetCarLight(stateCarLight::RED);
	SetCivilLight(stateCivilLight::GREEN_BLINK);
	_delay_ms(_delay);

	SetCarLight(stateCarLight::RED_ORANGE);
	SetCivilLight(stateCivilLight::RED);
	_delay_ms(_delay);

	SetCarLight(stateCarLight::GREEN);
	SetCivilLight(stateCivilLight::RED);
	_delay_ms(_delay);

	SetCarLight(stateCarLight::GREEN_BLINK);
	SetCivilLight(stateCivilLight::RED);
	_delay_ms(_delay);

	SetCarLight(stateCarLight::ORANGE);
	SetCivilLight(stateCivilLight::RED);
	_delay_ms(_delay);

	SetCarLight(stateCarLight::RED);
	SetCivilLight(stateCivilLight::RED);
	_delay_ms(_delay);

}

void SetCivilLight(stateCivilLight state){

	const auto setRed = []()
	{
		PORTD &= ~pinCivilGreen;
		PORTD |= pinCivilRed;
	};

	const auto setGreen = []()
	{
		PORTD &= ~pinCivilRed;
		PORTD |= pinCivilGreen;
	};

	const auto setOff = []()
	{
		PORTD &= ~(pinCivilGreen | pinCivilRed);
	};

	switch (state)
	{
		case stateCivilLight::RED :
			setRed();
			break;

		case stateCivilLight::GREEN :
			setGreen();
			break;

		case stateCivilLight::GREEN_BLINK:
			setOff();
			for (int i = 0; i<3;i++)
			{
				_delay_ms(_delayBlink);
				setOff();
				_delay_ms(_delayBlink);
				setGreen();
			}
			break;

		default:
			setRed();
			break;

	}

}

void SetCarLight(stateCarLight state)
{
	const auto setRed = []()
	{
		PORTD &= ~(pinCarGreen + pinCarOrange);
		PORTD |= pinCarRed;
	};

	const auto setRedOrange = []()
	{
		PORTD &= ~pinCarGreen;
		PORTD |= pinCarRed + pinCarOrange;
	};

	const auto setOrange = []()
	{
		PORTD &= ~(pinCarRed + pinCarGreen);
		PORTD |= pinCarOrange;
	};

	const auto setGreen = []()
	{
		PORTD &= ~(pinCarRed + pinCarOrange);
		PORTD |= pinCarGreen;
	};

	const auto setOff = []()
	{
		PORTD &= ~(pinCarRed + pinCarOrange + pinCarGreen);
	};

	switch (state)
	{
		case stateCarLight::RED :
			setRed();
			break;

		case stateCarLight::RED_ORANGE :
			setRedOrange();
			break;

		case stateCarLight::GREEN :
			setGreen();
			break;

		case stateCarLight::GREEN_BLINK :
			for (int i = 0; i < 3; i++)
			{
				_delay_ms(_delayBlink);
				setOff();
				_delay_ms(_delayBlink);
				setGreen();
			}
			break;
			

		case stateCarLight::ORANGE :
			setOrange();
			break;

	}
}