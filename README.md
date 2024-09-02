# PDP11-on-the-M5-Core

  This application is based upon the Pico 1140 project (https://github.com/Isysxp/Pico_1140) which implements a reasonable emulation
of a DEC PDP11/40 processor with RK11 and RL01/2 disk drives. The idea was to shrink what used to be an entire rack full of stuff
into a tiny box that can be lost behind the console terminal. (See photgraph: M5.jpg)
The M5Core2 module from M5Stack
(https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit) is a remarkable system containing an ESP32 with 4Mb PSRAM,
a 320x240 touch screen TFT, an SD Card and a number of other interesting devices. It also has its own internal Lithium cell that
can be charged from the USB socket. This will run your whole datacentre for some hours!!!! <br>
  The PDP11/40 emulation includes 124 KW of RAM, an FP11 floating point unit, the EIS instruction set,
a DL11 console serial port, an RL01/2 drive, an RK11 drive and a KL11 serial interface. It will run a number of DEC operating systems
including Unix V6, RT11, RSX11/M and RSTS. The disk images which are SIMH compatible are stored on the SD card and selected via the touch screen.
The console KL11 and the DL11 may be acceessed via telnet to port 2000 for the console and 23 for the DL11.The app will register the name M5PDP11 in your router.<br><br>
   The app can be built via the Arduino IDE and requires a number of extra libraries as below:<br>
Using library M5Core2 version 0.1.7 in folder "file:///c:/Users/iansc/Documents/Arduino/libraries/M5Core2"
Using library Wire version 2.0.0 in folder "file:///c:/Users/iansc/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.13/libraries/Wire"
Using library SPI version 2.0.0 in folder "file:///c:/Users/iansc/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.13/libraries/SPI"
Using library FS version 2.0.0 in folder "file:///c:/Users/iansc/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.13/libraries/FS"
Using library SD version 2.0.0 in folder "file:///c:/Users/iansc/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.13/libraries/SD"
Using library SPIFFS version 2.0.0 in folder "file:///c:/Users/iansc/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.13/libraries/SPIFFS"
Using library ESP32Time version 2.0.1 in folder "file:///c:/Users/iansc/Documents/Arduino/libraries/ESP32Time"
Using library ESP_Telnet version 2.1.2 in folder "file:///c:/Users/iansc/Documents/Arduino/libraries/ESP_Telnet"
Using library WiFi version 2.0.0 in folder "file:///c:/Users/iansc/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.13/libraries/WiFi"
Using library HTTPClient version 2.0.0 in folder "file:///c:/Users/iansc/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.13/libraries/HTTPClient"
Using library WiFiClientSecure version 2.0.0 in folder "file:///c:/Users/iansc/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.13/libraries/WiFiClientSecure"
<br><br>
Add all of the files in the /images directory to the root of a FAT32 formatted SD Card.
Before bulding the app, open ESP_Telnet.cpp and change the SSID and PSWD data to match your router.<br>
<br><br>
  First, use the 'Select' button to select the image you wish to boot. At this point it is a good idea to telnet to M5PDP11/port 2000 to connect to the console terminal.
Then, click the 'Boot' button and the console should show that the selected OS has booted. The 'Reset' button does a hard reset of the ESP32 which will
disconnect all of the network links to starts over. NB is you are running an old Unix, do sync a lot before doing this! If the PDP11 halts, this will also
cause a hard reset. <br>
  To troubleshoot, connect a terminal to the USB port (I use Teraterm) and this will generate some more output and act in parallel with
the console telnet session. In general, it is a good idea to test your disk images in Simh before adding them to the SDCard.
To add a new image, make sure that the the extension is either .RK05 for RK boot or .RL02 for an RL01/2 boot. This extension and the file
size are used to determine if the image is an RL01, RL02 or RK11 and to select the correct bootstrap. NB the emulation is a PDP11/40 with 18 bit addressing
and only kernel and user space. It will not run 22 bit OSs or those that need I/D mapping etc. However, it will run RSX11/M 4.6 and RSTS/E V9 which
were still used in the early 90's. In the images directory is an RT11 V5 image to run Multi-user Basic (New_RT11_5_XM.RL01). The default boot is to the SJ monitor.<br>
To give this a try, connect to the 2 telnet ports (23 and 2000), select the image as above, press 'Boot'<br>
.r mubas

WARNING: DATE NOT SET


MU BASIC-11/RT-11 V2.00R<br>
CONFIGURATION FILE : *2USER<br>
MU BASIC-11/RT-11 IS OPERATIONAL.<br>
PLEASE TYPE IN "HELLO".<br>

And, the last 2 lines as above will apear in the telnet port 23 window and you are now runnning 2 user Basic.<br>
I have also included an RK image of Unix V6 (type unix at the @ prompt) and it supports a second user via telnet to port 23.
Login with root or if you really think you can cut it ... with ken!!!! Also, look in the /user directory.<br>

Have fun,<br>
Ian Schofield
October 2023
<br>
<br>
In the meanwhile, I have been advised that building this app using the Arduino IDE throws lost of errors. This is due to
the c standard set in the M5Core board support package. To correct this, find the platform.txt file in the Arduino packages
directory (Arduino15\packages\m5stack\hardware\esp32\2.1.1) and change the line '-std=gnu++11' to '-std=gnu++17'.
The app should now build without errors!<br>

Ian Schofield
April 2024
<br>
<br>
The app has now been updated to support 22 bit addressing with a non split I/D space. the nearest equivalent system<br>
is an 11/24. The memory size is set to 2M in unibus.h. This may be extended up to at least 3M bytes which is less than the amount of<br>
free PSRAM attached to the ESP32. An updated version of ULTRIX 3.1 has been included in the images directory. This has<br>
been sysgened for an 11/24 with an FP11 FPU and 2 additional DL lines. This is very easy to do using sysgen in the /usr/sys/conf directory.<br>
This image includes f77 and the man system along with most well recognised utilties.<br>
<br>
Ian Schofield
Sept 2024
<br>

