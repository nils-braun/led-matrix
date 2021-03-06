/**
 * @name led-matrix
 * @file main.cpp
 * @version 1.0.0
 * @author Markus Prim & Nils Braun
 * @brief TODO
 */

#include <avr/io.h>
#include <util/delay.h>

#include <Field.h>
#include <InterfaceLED.h>

int main() {

	// this is hardware restricted
	const unsigned int nXLEDs(8);
	const unsigned int nYLEDs(8);

	// setup conways game of life
	Field field(nXLEDs, nYLEDs);
	field.fillFieldMono(false);

	InterfaceLED interface;
		field.setField(0,0, true);
		field.setField(5,1, true);
		field.setField(4,2, true);
		field.setField(3,3, true);
		field.setField(4,4, true);

	while(1) {
		interface.showField(field);
//_delay_ms(1000);
	}
}
