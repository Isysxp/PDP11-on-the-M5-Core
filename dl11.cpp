#include <cstdlib>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include "kb11.h"
#include "dl11.h"
#include <stdio.h>
#include <Arduino.h>
#include "ESPTelnetStream.h"
#include <FastLED.h>



extern KB11 cpu;
extern ESPTelnetStream telnet;

static bool keypressed = false;
static int lbright = 0;


DL11::DL11() {
}

void DL11::clearterminal() {
	rcsr = 0;
	xcsr = 0x80;
	rbuf = 0;
	xbuf = 0;
	count = 0;
}

static int _kbhit()
{
	if (telnet.isConnected()) {
		lbright = 150;
		return telnet.available();
	}
	return 0;
}

void DL11::serial_putchar(char c)
{
	telnet.write(c);
}
char DL11::serial_getchar()
{
	return (telnet.read());
}

void DL11::poll() {
	telnet.loop();
	if (++lbright > 100)
		lbright = -100;
	// FastLED.setBrightness(abs(lbright));
	// FastLED.show();
	if (!rcvrdone()) {
		// unit not busy
		if (true)
			if (_kbhit() || keypressed) {
				char ch = serial_getchar();
				count = 0;
				if (true) {
					rbuf = ch & 0x7f;
					rcsr |= 0x80;
					if (rcsr & 0x40) {
						cpu.interrupt(INTDLR, 4);
					}
				}
				else {
					keypressed = false;
				}
			}
	}


	if (xbuf) {
		xcsr |= 0x80;
		xbuf = 0;
		if (xcsr & 0x40) {
			cpu.interrupt(INTDLT, 4);
		}
	}
	else {
		if (iflag == 1) {
			cpu.interrupt(INTDLT, 4);
			iflag = 2;
		}
	}
}

uint16_t DL11::read16(uint32_t a) {
	int i;

	switch (a & 7) {
	case 00:
		return rcsr;
	case 02:
		rcsr &= ~0x80;
		return rbuf;
	case 04:
		return xcsr;
	case 06:
		return xbuf;
	default:
		Serial.printf("Dl11: read from invalid address %06o\n", a);
		trap(INTBUS);
	}
}

void DL11::write16(uint32_t a, uint16_t v) {
	switch (a & 7) {
		case 00:
			rcsr = ((rcsr & 0200) ^ (v & ~0200));
			break;
		case 02:
			//rcsr &= ~0x80;
			break;
		case 04:
			xcsr = ((xcsr & 0200) ^ (v & ~0200));
		if ((xcsr & 0300) == 0300 && iflag == 0)
			iflag = 1;
		if (iflag == 2)
			iflag = 0;
			break;
		case 06:
		xbuf = v & 0x7f;
		serial_putchar(v & 0x7f);
		xbuf |= 0200; // Allow for nulls !!!!
		xcsr &= ~0x80;
		iflag = 0;
		break;
	default:
		Serial.printf("Dl11: write to invalid address %06o\n", a);
		trap(INTBUS);
	}
}

