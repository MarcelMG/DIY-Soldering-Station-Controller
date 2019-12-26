# DIY-Soldering-Station-Controller
A soldering station controller compatible with [Weller RT active tips](https://www.weller-tools.com/professional/USA/us/Professional/Soldering+technology/Soldering+tips+_+nozzles/Soldering+tips/RT+Active+Tips)
The design is kept simple, with a rotary encoder with internal pushbutton to set the temperature and
a TM1637 4-digit 7-segment LED display. When the pushbutton is pressed for >3s, the current target temperature is stored as default value in the non-volatile EEPROM memory of the microcontroller. When the iron handle is put in the stand, the iron goes into standby mode and reduces it's temperature to 100°C. Originally I planned to use a hall sensor to detect the handle in the stand, but I chose a simpler solution by connencting an input pin of the microcontroller with internal pullup resistor to the stand. Since the metal part of the iron tip is grounded, it can sets the pin low and thus enables to detect the iron in the stand.
![solderingstation](https://github.com/MarcelMG/DIY-Soldering-Station-Controller/raw/master/solderingstation.jpg)

The microcontroller used is a Microchip ATTiny24A. The program code just fits barely into the small flash memory (2kB), so a variant like the ATtiny44 (4kB) or 88 (8kB) might be more appropriate. The display is a TM1637 4-digit 7-segment LED display module, those can be found cheaply on ebay etc. One big advantage of these modules is, that they need only 2 GPIO pins to communicate with the display using an I²C-ish interface. The rotary encoder used has an internal pushbutton, the one I used is salvaged from an old radio. The power supply is a also salvaged from an old laptop or similar, it supplies 12V DC at something around 3.6A. The Weller RT tips are rated at 44W max., so a quite beefy power supply is needed. The Weller RT tips use a standard 3,5mm stereo audio jack connector with the following pinout:
* tip = heating element
* middle ring = thermocouple sensor
* sleeve (back ring) = GND

The thermocouple signal is amplified with a low input offset voltage rail-to-rail OpAmp and fed to the microcontrollers internal ADC. The µC's internal bandgap voltage reference is used. Two RC-lowpass filters in the signal path remove noise in the signal. It is important to only measure the thermocouple sensor voltage when the heating element is off, or else erroneous readings will be produced. To achieve this, the heater is controlled with a center-aligned (a.k.a. phase correct) PWM. In the middle of the low-cycle of the PWM, an interrupt triggers an AD-conversion. The PWM duty-cycle is computed with a P-controller to control the temperature. The heating element is switched with a p-MOSFET power transistor. To drive it's gate with 12V from the microcontrollers 5V level, another small nMOSFET is used. A PCB for the design has been designed and manufactured. The microcontroller is programmed via an 6-pin ISP connector.
![pcb](https://github.com/MarcelMG/DIY-Soldering-Station-Controller/raw/master/pcb.png)

This is the connection diagram (pinout) of the PCB:
![pinout_annotated](https://github.com/MarcelMG/DIY-Soldering-Station-Controller/raw/master/pinout_annotated.png)

