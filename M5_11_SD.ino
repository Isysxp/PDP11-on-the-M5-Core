
#include <M5Core2.h>
#include <ESP32Time.h>
#include <ESP.h>
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ESPTelnetStream.h"
#include "beep.h"

extern void TStart();

int startup(char* rkfile, char* rlfile, int bootdev);
using namespace std;
char* ReadLine(bool fullDuplex = true, char lineBreak = '\n');

string Fnames[64];
int SelFile = 0, cntr = 0;
int BtKey = 0;
extern int runFlag;

// List contents of SDCard.

void listDir(fs::FS& fs, const char* dirname, uint8_t levels) {
	Serial.printf("Listing directory: %s\r\n", dirname);

	File root = fs.open(dirname);
	if (!root) {
		Serial.println("- failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println(" - not a directory");
		return;
	}
	File file = root.openNextFile();
	while (file) {
		if (strcasestr(file.name(), ".r"))
		{
			if (file.isDirectory()) {
				Serial.print("  DIR : ");
				Serial.println(file.name());
				if (levels) {
					listDir(fs, file.name(), levels - 1);
				}
			}
			else {
				Serial.printf(" File:%3d ", cntr + 1);
				Serial.print(file.name());
				Fnames[cntr++] = file.name();
				Serial.print("\tSIZE: ");
				Serial.println(file.size());
			}
		}
		file = root.openNextFile();
	}
}


void eventDisplay(Event& e) {
	// Serial.printf("%-12s %-18s\r\n", e.typeName(), e.objName());
	// if (e.type == E_RELEASE || e.type == E_PRESSED) Serial.printf("%5d ms", e.duration);
	M5.Buttons.draw();
	if (!(e.type == E_PRESSING))
		return;
	M5.Spk.PlaySound(beep, sizeof(beep));  // Play beep
	if (strcmp(e.objName(), "BtnA") == 0 && !runFlag) {
		M5.Lcd.fillRect(10, 105, 310, 20, BLACK);
		M5.Lcd.setCursor(10, 120);
		if (++SelFile == cntr)
			SelFile = 0;
		M5.Lcd.print("Image: ");
		M5.Lcd.print(Fnames[SelFile].c_str());
	}
	if (strcmp(e.objName(), "BtnB") == 0 && !runFlag) {
		BtKey++;
	}
	if (strcmp(e.objName(), "BtnC") == 0 && runFlag) {
		ESP.restart();
	}
}

void setup() {
	char *bfr, rkfile[32], rlfile[32],bbfr[32];
	int bootdev = 0, btlvl;

	M5.begin();
	Serial.begin(115200);
	// Serial1.begin(115200);
	while (!Serial)
	{
		yield();
	}
	Serial.println("Startup.....");
	if (!SD.begin()) {
		Serial.println("SD Card mount failed.....");
		M5.Lcd.print("SD Card mount failed.....");
		while (1);
	}
	M5.Lcd.setBrightness(200);
	M5.Lcd.drawJpgFile(SD, "/Logo.jpg"); // Load background from SD

	M5.BtnA.setLabel("Select");
	M5.BtnB.setLabel("Boot");
	M5.BtnC.setLabel("Reset");
	M5.BtnA.off = M5.BtnB.off = M5.BtnC.off = { BLUE, WHITE, NODRAW };
	M5.BtnA.on = M5.BtnB.on = M5.BtnC.on = { BLUE, GREEN, NODRAW };
	M5.Buttons.addHandler(eventDisplay);
	M5.Buttons.draw();

	uint64_t cardSize = SD.totalBytes() / (1024 * 1024);
	Serial.printf("SD Card Size: %lluMB\r\n", cardSize);
	Serial.printf("Total heap: %d\r\n", ESP.getHeapSize());
	Serial.printf("Free heap: %d\r\n", ESP.getFreeHeap());
	Serial.printf("Alloc heap: %d\r\n", ESP.getMaxAllocHeap());
	TStart();
	listDir(SD, "/", 3);
	M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
	M5.Lcd.setCursor(10, 120);
	M5.Lcd.print("Image: ");
	M5.Lcd.print(Fnames[SelFile].c_str());
	M5.Lcd.fillCircle(160, 165, 30, DARKGREY);
	btlvl = M5.Axp.GetBatteryLevel();
	sprintf(bbfr, "%3d%%", (int)btlvl);
	M5.Lcd.setCursor(135, 170);
	M5.Lcd.print(bbfr);
	Serial.printf("Enter index of RK05 image:");
	bfr = ReadLine(true, '\r');
	if (bfr[0] != '$') {
		sscanf(bfr, "%d", &SelFile);
		strcpy(rkfile, "/");
		strcat(rkfile, Fnames[SelFile - 1].c_str());
		Serial.printf("\r\nEnter index of RL01/2 image:");
		bfr = ReadLine(true, '\r');
		sscanf(bfr, "%d", &SelFile);
		strcpy(rlfile, "/");
		strcat(rlfile, Fnames[SelFile - 1].c_str());
		Serial.printf("\r\nBoot: RK/RL:");
		bfr = ReadLine(true, '\r');
		if (bfr[1] == 'l' || bfr[1] == 'L')
			bootdev = 1;
	}
	else
	{
		M5.Lcd.fillRect(10, 105, 310, 20, BLACK);
		strcpy(rkfile, "/Empty_RK05.dsk");
		strcpy(rlfile, "/Empty_RL01.dsk");
		M5.Lcd.setCursor(10, 120);
		M5.Lcd.print("Boot: ");
		M5.Lcd.print(Fnames[SelFile].c_str());
		if (strcasestr(Fnames[SelFile].c_str(), ".rl")) {
			strcpy(rlfile, "/");
			strcat(rlfile, Fnames[SelFile].c_str());
			bootdev = 1;
		}
		if (strcasestr(Fnames[SelFile].c_str(), ".rk")) {
			strcpy(rkfile, "/");
			strcat(rkfile, Fnames[SelFile].c_str());
			bootdev = 0;
		}
	}
	if (bootdev)
		Serial.printf("\r\nBooting file:%s on RL0:\r\n", rlfile);
	else
		Serial.printf("\r\nBooting file:%s on RK0:\r\n", rkfile);
	M5.Lcd.fillCircle(160, 165, 30, GREEN);
	startup(rkfile, rlfile, bootdev);
}
