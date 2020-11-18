/*
ECE 4180 Final Project
Alarm Bot by Chase Hall, William Hamil, Kyeomeun Jang, and Andrew Lang

This alarm clock is mounted on a robot, which drives away from you when the alarm sounds, making you get out of bed
and turn it off. The alarm time can be set and snoozed from a Bluetooth app, but can only be turned via a button on the robot itself
*/

///   preprocessor directives   
#include "mbed.h"
#include "rtos.h"
#include "Motor.h"
#include "uLCD_4DGL.h"
#include "ultrasonic.h"
#define ALARM_OFF       0                 // names for the alarm status, power up in ALARM_OFF
#define ALARM_ON        1                 // robot travels randomly in this state
#define ALARM_SNOOZE    2                 // robot doesn't travel in this state, but there is a timer waiting to put it back in ALARM_ON
#define ALARM_ON_HAZARD 3                 // if a hazard is present when the alarm is on, the movement is no longer random
// #include SOMETHING FOR SONAR             TODO  



///   Declaring all global variables, mutex's, and volatile globals
Mutex uLCD_mutex;  // LCD will be written by updating alarm time, updating current time, and when alarm goes off
Mutex PC_mutex;   // The PC Serial port can be written by either Bluetooth or the Pi when debugging
volatile unsigned int alarmState = ALARM_OFF;   // power up with alarm off
//volatile time_t current_time = time(NULL);      // placeholder for time being updated by Pi board
//volatile time_t alarm_time = time(NULL);
struct tm current_time = {.tm_mday = 1};
struct tm alarm_time = {0};
unsigned int flashRed = 0xFF0000;               // the color to flash when the alarm is going off (RED)
void dist (int distance);               // declaring the interrupt routine for sonar


///   Declaring all hardware object
RawSerial Bluetooth(p13, p14);          // Adafruit Bluetooth Module:   p13 - TX, p14 - RX
RawSerial PC(USBTX, USBRX);             // debug connection to the PC:  USBTX - TX, USBRX - RX 
// USBSerial Pi;                        // Pi USB connection from breakout to D+, D-
ultrasonic Sonar(p6, p7, .1, 1, &dist); // the sonar sensor (1 of them);
uLCD_4DGL uLCD(p28, p27, p30);          // LCD screen for time, status: p28 - TX, p27 - RX, p30 - reset
Motor leftWheel(p23, p21, p22);         // H-bridge for left motor:     p23 - PWMA, p21 - fwd (AI1), p22 - rev (AI2)
Motor rightWheel(p26, p24, p25);        // H-bridge for right motor:    p26 - PWMB, p24 - fwd (BI1), p25 - rev (BI2)
//DigitalIn AlarmOff(p20, PullDown);         // incase we want to use a push button instead of resetting mbed
DigitalOut led1(LED1);              // for debugging
DigitalOut led3(LED3);
DigitalOut led4(LED4);
///   End of hardware object declaration



void checkAlarm()
{
    while (1)
    {
        //if (difftime(alarm_time, current_time) < 1.0) alarmState = ALARM_ON;
        //if (alarmState == ALARM_ON && AlarmOff == 1) alarmState = ALARM_OFF;
        //if (alarmState == ALARM_ON)
//        {
//            uLCD_mutex.lock();
//            uLCD.filled_rectangle(0, 0, 127, 127, flashRed);
//            uLCD_mutex.unlock();
//            flashRed = (flashRed == 0xFF0000) ? 0x000000 : 0xFF0000; // invert the red to black   
//        }
       //if (alarmState == ALARM_OFF)
//        else
//        {
            uLCD_mutex.lock();
            uLCD.cls();
            uLCD.locate(1,1);
            uLCD.printf("%d\r\n", alarmState);
            uLCD.locate(1,3);
            mktime(&current_time);
            uLCD.printf("%s\n", asctime(&current_time));
            uLCD_mutex.unlock();
        //}
        Thread::wait(500);
    }
}

void blueRX()       // the interrupt for an incoming Bluetooth signal
{
    char curr_in;
    while (Bluetooth.readable())
    {
        curr_in = (char) Bluetooth.getc();
        if (curr_in == 'a') alarmState = ALARM_ON;
        else if (curr_in == 'b') alarmState = ALARM_OFF;
        PC_mutex.lock();
        PC.putc(curr_in);
        PC_mutex.unlock();
    }
}

void PCRX()         // the interrupt for an incoming PC signal
{
    char curr_in;
    while (PC.readable())
    {
        PC_mutex.lock();
        curr_in = (char) PC.getc();
        PC_mutex.unlock();
        Bluetooth.putc(curr_in);
    }
}
        

void setMotors()
{
    float lMotor = 0.0;
    float rMotor = 0.0;
    while (1)
    {
        switch (alarmState)
        {
        case (ALARM_ON):
            lMotor = ((float) rand()) / RAND_MAX;  // random value between 0 (off) and 1 (forward);
            rMotor = lMotor + 0.3 * (((float) rand())/RAND_MAX - 0.5); // random value between lMotor-0.3, lMotor+0.3
            if (rMotor < 0) rMotor = 0;             // dont want bot moving backwards  
            break;
        case (ALARM_ON_HAZARD):
            lMotor = 0.1;                           // TODO: once we have multiple sonars, do smart stuff here
            rMotor = 0.1;
            break;
        default:
            lMotor = 0.0;
            rMotor = 0.0;
        }
        //if (alarmState == ALARM_ON)
//        {
//            lMotor = ((float) rand()) / RAND_MAX;  // random value between 0 (off) and 1 (forward);
//            rMotor = lMotor + 0.3 * (((float) rand())/RAND_MAX - 0.5); // random value between lMotor-0.3, lMotor+0.3
//            if (rMotor < 0) rMotor = 0;             // dont want bot moving backwards
//        } 
//        else
//        {
//            lMotor = 0.0;
//            rMotor = 0.0;
//        }
        leftWheel.speed(lMotor);
        rightWheel.speed(rMotor);
        Thread::wait(500);
    }
}

void checkSonar()               // The sonar-checking thread
{
    while (1)
    {
        Sonar.checkDistance();
//        led4 = !led4;
        Thread::wait(10);       // check distance 100 times a second
    }   
}

void dist(int distance)
{   
    led3 = !led3;
    PC_mutex.lock();
    PC.printf("Distance %d mm \r\n", distance);
    PC_mutex.unlock();
    if (distance < 120 && alarmState == ALARM_ON) alarmState = ALARM_ON_HAZARD; // if there is a hazard, detect it and change the state
    if (distance >= 120 && alarmState == ALARM_ON_HAZARD) alarmState= ALARM_ON; // if there is no longer a hazard, change the state back to what it was
}

int main() 
{   
    uLCD.cls();
    uLCD.text_width(4);
    uLCD.text_height(4);
    uLCD.color(0xFFFFFF);
    uLCD.baudrate(3000000);
    Sonar.startUpdates();                                                       // Start measuring the distance
    Thread thread1;
    thread1.start(callback(checkAlarm));                                        // thread1 manages the state of the alarm and status info
    Thread thread2;
    thread2.start(callback(setMotors));                                         // thread2 sets the motors given the state of the alarm device
    Thread thread3;
    thread3.start(callback(checkSonar));                                        // thread3 is sonar-checking thread. A sonar change triggers an interrupt
    Bluetooth.baud(9600);
    Bluetooth.attach(&blueRX, Serial::RxIrq);                                   // Blutooth is an interrupt 
    PC.baud(9600);                                                              // PC UART and Bluetooth UART must match baudrates
    PC.attach(&PCRX, Serial::RxIrq);
    
    while(1) {
        led1 = !led1;
        Thread::wait(500);
    }
}
