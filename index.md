## AlarmBot - ECE 4180

Andrew Lang, Chase Hall, Kyeomeun Jang, William Hamil

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
- uLCD-144-G2 LCD Screen
- USB A to MiniUSB Cable
- MiniUSB to USB A Adapter
- MicroUSB to DC Barrel Jack
- Adafruit Bluefruit LE UART Friend Bluetooth Module
- Adafruit Stereo Bonnet Pack for Raspberry Pi Zero W

### Background
For our project, we will be creating an RC robot alarm clock. The idea is that you can set an alarm clock using a web interface hosted on a Raspberry Pi. Once the alarm goes off, the robot will drive away and force you out of bed to turn off the alarm. The movement will be mostly random for the robot, but we will implement collision detection using a sonar module to prevent the robot form getting stuck. The alarm can be turned off using the up and down arrows on the Adafruit Bluetooth app. Once the alarm is activated, you can press 1,2,3 or 4 on the app to snooze for that amount of time in minutes. However, the alarm can only be permanently stopped by resetting the mbed. The alarm tone will be played using a Pi interfacing with Spotify. This will allow you to set the soundtrack for your alarm. We will use an LCD to display the time to help set the alarm. If the alarm is activated, the LCD will flash black and white telling you to wake up. 

### Connections
| Hardware & Pin | mbed Pin |
| -------------- | -------- |
| HC-SR04 trig   | p6       |
| HC-SR04 echo   | p7       | 
| H-bridge AI1   | p21      |
| H-bridge AI2   | p22      |
| H-bridge PWMA  | p23      |
| H-bridge BI1   | p24      |
| H-bridge BI2   | p25      |
| H-bridge PWMB  | p26      |
| H-bridge STBY  | Ground   |
| miniUSB D+     | D+       |
| miniUSB D-     | D-       |
| uLCD RX        | p27      |
| uLCD TX        | p28      |
| uLCD res       | p30      |
| Bluetooth TX   | p13      |
| Bluetooth RX   | p14      |



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
