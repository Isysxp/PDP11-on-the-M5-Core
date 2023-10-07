# PDP11-on-the-M5-Core

  This application is based upon the Pico 1140 project (https://github.com/Isysxp/Pico_1140) which implements a reasonable emulation
of a DEC PDP11/40 processor with RK11 and RL01/2 disk drives. The idea was to shrink what used to be an entire rack full of stuff
into a tiny box that can be lost behind the console terminal. (See photgraph: https://photos.google.com/photo/AF1QipPFVmZxG_FcibzDAUMDVP4PwSy7cSt0lIZ5ATov)
The M5Core2 module from M5Stack
(https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit) is a remarkable system containing an ESP32 with 4Mb PSRAM,
a 320x240 touch screen TFT, an SD Card and a number of other interesting devices. It also has its own internal Lithium cell that
can be charged from the USB socket. This will run your whole datacentre for some hours!!!! <br>
  The PDP11/40 emulation includes 124 KW of RAM, an FP11 floating point unit, the EIS instruction set,
a DL11 console serial port, an RL01/2 drive, an RK11 drive and a KL11 serial interface. It will run a number of DEC operating systems
including Unix V6, RT11, RSX11/M and RSTS. The disk images which are SIMH compatible are stored on the SD card and selected via the touch screen.
The console KL11 and the DL11 may be acceessed via telnet to port 2000 for the console and 23 for the DL11.<br>
   The app can be built via the Arduino IDE and requires a number of extra libraries as below:
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
Finally, add all of the files in the /images directory to the root of a FAT32 formatted SD Card.
<br>
  First, use the 'Select' button to select the image you wish to boot. At this point it is a good idea to telnet to port 2000 to connect to the console terminal.
Then, click the 'Boot' button and the console should show that the selected OS has booted. The 'Reset' button does a hadr reset of the ESP32 which will
disconnect all of the network links to starts over. NB is you are running an old Unix, do sync a lot before doing this! If the PDP11 halts, this will also
cause a hard reset. <br>
  To find out what is happening, connect a terminal to the USB port (I use Teraterm) and this will generate some more output and act in parallel with
the console telnet session. In general, it is a good idea to test your disk images in Simh before adding them to the SDCard.
To add a new image, make sure that the the extension is either .RK05 for RK boot or .RL02 for an RL01/2 boot. This extension and the file
size are used to determine if the image is an RL01, RL02 or RK11 and to select the correct bootstrap. NB the emulation is a PDP11/40 with 18 bit addressing
and only kernel and user space. It will not run 22 bit OSs or those that need I/D mapping etc. However, it will run RSX11/M 4.6 and RSTS/E V9 which
were still used in the early 90's. I have included an RK image of Unix V6 (type unix at the @ prompt) and it supports a a second user via telnet to port 23.
Login with root or if you really think you can cut it ... with ken!!!!<br>

Have fun,
Ian Schofield
October 2013
