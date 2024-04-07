#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <assert.h>
#include <cstdlib>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avr11.h"
#include "kb11.h"
#include <M5Core2.h>
#include <ESP32Time.h>

KB11 cpu;
int kbdelay = 0;
int clkdelay = 0;
uint64_t systime,nowtime,clkdiv;
ESP32Time SystemTime;
int runFlag = 0, btcntr = 0;
float btlvl;
int RLTYPE;


void setup( char *rkfile, char *rlfile, int bootdev)
 {

	if (cpu.unibus.rk11.rk05)
		return;
  cpu.unibus.rk11.rk05 = SD.open(rkfile,"rb+");
  cpu.unibus.rl11.rl02 = SD.open(rlfile,"rb+");
  RLTYPE = 035;
  if (strcasestr(rlfile, ".rl02"))
      RLTYPE = 0235;
  clkdiv = (uint64_t)1000000 / (uint64_t)60;
  systime = SystemTime.getMillis();
	cpu.reset(02002,bootdev);
  Serial.printf("Ready\n");  
}

jmp_buf trapbuf;

void loop0();

[[noreturn]] void trap(uint16_t vec) { longjmp(trapbuf, vec); }

void loop() {
    auto vec = setjmp(trapbuf);
    if (vec == 0) {
        loop0();
    } else {
        cpu.trapat(vec);
    }
}

void loop0() {
    char bbfr[32];

    M5.Lcd.setTextColor(TFT_ORANGE, TFT_BLACK);
    while (true) {
        if ((cpu.itab[0].vec > 0) && (cpu.itab[0].pri > cpu.priority())) {
            cpu.trapat(cpu.itab[0].vec);
            cpu.popirq();
            return; // exit from loop to reset trapbuf
        }
        if (!cpu.wtstate)
           cpu.step();
        cpu.unibus.rk11.step();
        cpu.unibus.rl11.step();
        if (kbdelay++ == 1000) {
            cpu.unibus.cons.poll();
            cpu.unibus.dl11.poll();
            kbdelay = 0;
            nowtime = SystemTime.getMillis();
            M5.update();
            if (btcntr++ == 1000) {
                btcntr = 0;
                btlvl = M5.Axp.GetBatteryLevel();
                sprintf(bbfr, "%3d%%", (int)btlvl);
                M5.Lcd.fillCircle(160, 165, 30, GREEN);
                M5.Lcd.setCursor(135, 170);
                M5.Lcd.print(bbfr);
            }
        }
        if (nowtime-systime > 20) {
            cpu.unibus.kw11.tick();
            systime = nowtime;
        }
    }
}

int startup( char *rkfile, char *rlfile, int bootdev)
{
    setup(rkfile,rlfile,bootdev);
    runFlag++;
    while (1)
        loop();
}

void panic() {
    cpu.printstate();
    std::abort();
}
