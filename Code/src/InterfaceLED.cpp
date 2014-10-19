/*
 * InterfaceLED.cpp
 *
 *  Created on: 16.01.2014
 *      Author: nils
 */

#include "InterfaceLED.h"

/**
 * Standard constructor.
 * The only constructor of the InterfaceLED class. It initializes the used Ports to output ports
 * and sets the output to Off before any actions.
 */
InterfaceLED::InterfaceLED() {
	DDRB = 0xFF;
	PORTB = 0;
	DDRC = 0xFF;
	PORTC = 0;
}

/**
 * Shows a field on the LED matrix.
 * Loops through the columns (by setting only one 1 for each column consecutively and the rest to zero)
 * and turns the rows to on, which have a turned on cell in this column.
 * Sends clock signals to the shift registers and turns STROBE off while writing (to prevent flickering).
 * @param field the field to be shown
 * @param waitingTime the waiting time between to columns in µs. It controls the brightness of the LEDs.
 * Values beyond approx. 2000 leed to flickering of the LEDs, values smaller than 100 make the LED too dark.
 */
void InterfaceLED::showField(const Field * const field, const uint16_t waitingTime) {

	// loop through all columns
	for(uint8_t column = 0; column < field->getSizeX(); column++) {

		PORTB &= ~(1 << InterfaceLED::m_outputEnablePin);

		// Turn of writing on LED matrix
		PORTB &= ~(1 << InterfaceLED::m_strobePin);

		// first write column index to shift registers
		for(uint8_t counter = 8; counter > 0; counter--) {
			// one clock
			PORTB |= (1 << InterfaceLED::m_clockPin);
			PORTB &= ~(1 << InterfaceLED::m_clockPin);
			// just one 1 on the column index
			if(counter - 1 == column) {
				PORTB |= (1 << InterfaceLED::m_dataPin);
			}
			else
			{
				PORTB &= ~(1 << InterfaceLED::m_dataPin);
			}
		}
		
		// why is the last clock needed?
		PORTB |= (1 << InterfaceLED::m_clockPin);
		PORTB &= ~(1 << InterfaceLED::m_clockPin);
		PORTB &= ~(1 << InterfaceLED::m_dataPin);

		// Turn off writing on LED matrix
		PORTB |= (1 << InterfaceLED::m_strobePin);

		// Turn of writing on LED matrix
		PORTC &= ~(1 << InterfaceLED::m_strobePin);

		for(uint8_t row = 0; row < field->getSizeY(); row++) {
			// one clock
			PORTC |= (1 << InterfaceLED::m_clockPin);
			PORTC &= ~(1 << InterfaceLED::m_clockPin);

			// write 1 to data if cell is set
			if(field->getField(column,row).getCellStatus() == true) {
				PORTC |= (1 << InterfaceLED::m_dataPin);
			}
			else {
				PORTC &= ~(1 << InterfaceLED::m_dataPin);
			}
		}

		// why is the last clock needed?
		PORTC |= (1 << InterfaceLED::m_clockPin);
		PORTC &= ~(1 << InterfaceLED::m_clockPin);
		PORTC &= ~(1 << InterfaceLED::m_dataPin);

		// Turn off writing on LED matrix
		PORTC |= (1 << InterfaceLED::m_strobePin);

		PORTB |= (1 << InterfaceLED::m_outputEnablePin);

		for(uint16_t time = waitingTime; time > 0; time--) {
			_delay_us(1);
		}
	}
}


void InterfaceLED::showCycles(const Field * const field, uint16_t cycles) {
	for(uint16_t t = 0; t < cycles; t++) {
		showField(field);
	}
}
