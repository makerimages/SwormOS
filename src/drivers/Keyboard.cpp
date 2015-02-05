#include "drivers/Keyboard.hpp"

Keyboard::Keyboard() {

}

void Keyboard::writeControl(uint8_t bit)  {
	outb(0x64,bit);
}


uint8_t Keyboard::readControl() {
	return inb(0x64);
}

void Keyboard::writeData(uint8_t bit) {
	while(readControl() & 0x02) {
		// Do nothing, but wait for the buffer to be empty.
	}
	outb(0x60,bit);
}

uint8_t Keyboard::readData() {
	while(! (readControl() & 0x01)) {
		//Do nothing, just wait for data to arrive
	}
	return inb(0x60);
}


void Keyboard::init() {
	terminal.print("Starting keyboard bringup.\n");
	writeControl(0xAD);
	writeControl(0xA7);

	writeControl(0x20); // Ask for control byte 
	config = readData();

	//config &= ~(1<<0);
	//config &= ~(1<<1);
	//config &= ~(1<<6);

	//writeControl(0x60); // Hey, be prepared for control byte!
	//writeData(config);

	//while (readControl() & 0x01) {
		//inb(0x60); // Clear buffer
	//}

	// Try enabling port 1
	//writeControl(0xAE);
	//writeControl(0x20); // Gimme config

	//config = readData();
	//if(!(config && (1<<4))) {
	//	terminal.print("Port 1 exists.\n");
	//}

	//writeControl(0xA8);




}