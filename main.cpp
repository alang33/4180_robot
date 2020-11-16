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
#define ALARM_OFF       0                 // names for the alarm status, power up in ALARM_OFF
#define ALARM_ON        1
#define ALARM_SNOOZE    2
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



///   Declaring all hardware object
RawSerial Bluetooth(p13, p14);      // Adafruit Bluetooth Module:   p13 - TX, p14 - RX
RawSerial PC(USBTX, USBRX);         // debug connection to the PC:  USBTX - TX, USBRX - RX 
//RawSerial PI(p9, p10);              // Raspberry Pi Zero:           p9 - TX, p10 - RX
//// DigitalIn Sonar???                       TODO
uLCD_4DGL uLCD(p28, p27, p30);      // LCD screen for time, status: p28 - TX, p27 - RX, p30 - reset
Motor leftWheel(p23, p21, p22);     // H-bridge for left motor:     p23 - PWMA, p21 - fwd (AI1), p22 - rev (AI2)
Motor rightWheel(p26, p24, p25);    // H-bridge for right motor:    p26 - PWMB, p24 - fwd (BI1), p25 - rev (BI2)
//DigitalIn AlarmOff(p20, PullDown);         // incase we want to use a push button instead of resetting mbed
DigitalOut led1(LED1);              // for debugging
///   End of hardware object declaration



void checkAlarm()
{
    while (1)
    {
        //if (difftime(alarm_time, current_time) < 1.0) alarmState = ALARM_ON;
        //if (alarmState == ALARM_ON && AlarmOff == 1) alarmState = ALARM_OFF;
        if (alarmState == ALARM_ON)
        {
            uLCD_mutex.lock();
            uLCD.filled_rectangle(0, 0, 127, 127, flashRed);
            uLCD_mutex.unlock();
            flashRed = (flashRed == 0xFF0000) ? 0x000000 : 0xFF0000; // invert the red to black   
        }
        if (alarmState == ALARM_OFF)
        {
            uLCD_mutex.lock();
            uLCD.cls();
            uLCD.locate(1,1);
            uLCD.printf("%d\r\n", alarmState);
            uLCD.locate(1,3);
            mktime(&current_time);
            uLCD.printf("%s\n", asctime(&current_time));
            uLCD_mutex.unlock();
        }
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
        if (alarmState == ALARM_ON)
        {
            lMotor = ((float) rand()) / RAND_MAX;  // random value between 0 (off) and 1 (forward);
            rMotor = lMotor + 0.3 * (((float) rand())/RAND_MAX - 0.5); // random value between lMotor-0.3, lMotor+0.3
            if (rMotor < 0) rMotor = 0;             // dont want bot moving backwards
        } 
        else
        {
            lMotor = 0.0;
            rMotor = 0.0;
        }
        leftWheel.speed(lMotor);
        rightWheel.speed(rMotor);
        Thread::wait(500);
    }
}

/* void detectCollision()
{
    while (1)
    {
        if sonar is this, or if the whisker detects that {
            leftWheel.speed(something) and/or
            rightWheel.speed(something)
        }
    }   
}
*/

int main() 
{   
    uLCD.cls();
    uLCD.text_width(4);
    uLCD.text_height(4);
    uLCD.color(0xFFFFFF);
    uLCD.baudrate(3000000);
    Thread thread1;
    thread1.start(callback(checkAlarm));
    Thread thread2;
    thread2.start(callback(setMotors));
    Bluetooth.baud(9600);
    Bluetooth.attach(&blueRX, Serial::RxIrq);       // Blutooth as an interrupt 
    PC.baud(9600);
    PC.attach(&PCRX, Serial::RxIrq);
    
//    srand(time(NULL));
    
    while(1) {
        led1 = !led1;
        Thread::wait(500);
    }
}
