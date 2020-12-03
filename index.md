## AlarmBot - ECE 4180

Andrew Lang, Chase Hall, Kyeomeun Jang, William Hamil

### Background
This project creates an RC robot alarm clock. The idea is that you can set an alarm clock using a web interface hosted on a Raspberry Pi.  The Pi then sends the current time and alarm time to the MBED. Once the alarm goes off, the robot will drive away and force you out of bed to turn off the alarm. The movement will be mostly random for the robot, but will include collision detection using a sonar module. The alarm can be turned off or snoozed using the Adafruit Bluetooth app. However, the alarm can only be permanently stopped by resetting the mbed. The alarm tone will be played using a Pi interfacing with Spotify. This will allow you to set the soundtrack for your alarm. An LCD will display the current time and alarm time while inactive. If the alarm is activated the LCD will flash telling you to wake up. 

### Parts List
- ARM mbed
- Raspberry Pi Zero W
- Sonar (HC-SR04)
- Sparkfun Dual H-Bridge Motor Driver
- (2x) Sparkfun Shadow Chassis
- (2x) Sparkfun Hobby Gearmotor - 140 RPM
- (2x) Sparkfun Wheel - 65mm
- (3x) DC Battery Pack
- (12x) AA Batteries
- Sparkfun MiniUSB Breakout Board
- Sparkfun USB-A Breakout Board
- uLCD-144-G2 LCD Screen
- USB-A to MiniUSB Cable
- MiniUSB to USB-A Adapter
- MicroUSB to DC Barrel Jack
- Adafruit Bluefruit LE UART Friend Bluetooth Module
- Adafruit Stereo Bonnet Pack for Raspberry Pi Zero W
- Raspberry Pi cobbler

## Setup
The following connections need to be made to the MBED for all of the components to work.  Note that while VU on the MBED does supply 5V, the Bluetooth module and motors draw more current than provided and need to be connected direclty to the battery packs.  The MBED can also be powered by connecitng the VU pin to a power rail from the battery pack.
The Raspberry Pi can be powered using the DC to microUSB adapter with its own battery pack.  The Pi GPIO pins are connected to the audio bonnet and the bonnet pins can be connected to a Pi cobbler for a breadboard connection.

### Motor Connections

| MBED | H-Bridge | L-Motor | R-Motor |
| ---- | -------- | ------- | ------- |
| 5V   | VM       |         |         |
| VOUT | VCC      |         |         |
| GND  | GND      |         |         |
| p21  | AI1      |         |         |
| p22  | AI2      |         |         |
| p23  | PWMA     |         |         |
| p24  | BI1      |         |         |
| p25  | BI2      |         |         |
| p26  | PWMB     |         |         |
| VOUT | STBY     |         |         |
|      | A01      | -       |         |
|      | A02      | +       |         |
|      | B01      |         | -       |
|      | B02      |         | +       |

### miniUSB Connections

| MBED | miniUSB |
| ---- | ------- |
| VOUT | VCC     |
| D-   | D-      |
| D+   | D+      |
|      | ID      |
| GND  | GND     |

### uLCD Connections

| MBED | uLCD |
| ---- | ---- |
| 5V   | +5V  |
| p27  | RX   |
| p28  | TX   |
| p30  | RES  |
| GND  | GND  |

### Bluetooth Connections

| MBED | Bluetooth |
| ---- | --------- |
| GND  | GND       |
| 5V   | Vin       |
|      | RTS       |
| GND  | CTS       |
| p13  | TX        |
| p14  | RX        |

### Sonar Connections (HC-SR04)

| MBED | Sonar |
| ---- | ----- |
| VOUT | VCC   |
| p6   | Trig  |
| p7   | Echo  |
| GND  | GND   |

### MBED Button Connections

| MBED | Pushbutton(1)| Pushbutton(2)| Pushbutton(3)|
| ---- | ------------ | ------------ | ------------ |
| p19  |      +       |              |              | 
| p20  |              |       +      |              | 
| GND  |      -       |       -      |       -      | 

### Raspberry Pi Button Connections

|  Pi  | Pushbutton(1)| Pushbutton(2)| Pushbutton(3)|
| ---- | ------------ | ------------ | ------------ |
| p4   |      +       |              |              | 
| p2   |              |       +      |              | 
| p3   |              |              |       +      | 
| GND  |      -       |       -      |       -      | 

### Schematic
![Robot Schematic](/Schematic.png)

### Difficulties and Future Improvements

Our main area of difficulties centered around the USB Serial library used to interface between the MBED and the Raspberry Pi.  We tried for hours on end to incorporate the included interrupts or place the character scanning code in a separate thread; however, the code would lock up every time and would not take any input.  The only way we could get the code to run properly was to put it at the begining of our Main method before the threads were started.  Some time could be put into rewriting this library to be more functional, like the primary Serial library for the MBED.  

Additionally, time could be spent on reducing the weight of the robot to allow for more agile movements.

### Demonstration Video
<iframe src="https://drive.google.com/file/d/1FtK1rr7Eu_E5E9mukUXWRYC-gY3-_CVr/preview" width="580" height="327"></iframe>

[editor on GitHub](https://github.com/alang33/4180_robot/edit/gh-pages/index.md)

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/alang33/4180_robot/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
