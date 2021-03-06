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
#include "USBSerial.h"
#include <string>
#define ALARM_OFF       0                                                       // names for the alarm status, power up in ALARM_OFF
#define ALARM_ON        1                                                       // robot travels randomly in this state
#define ALARM_SNOOZE    2                                                       // robot doesn't travel in this state, but there is a timer waiting to put it back in ALARM_ON
#define ALARM_ON_HAZARD 3                                                       // if a hazard is present when the alarm is on, the movement is no longer random 
#define ALARM_SHUTOFF   4                                                       // ALARM_OFF is idle state, this is the state for when the alarm is physically turned off
#define NULL            0
#define SNOOZE_TIME     1                                                       // minutes of snooze to do


///   Declaring all global variables, mutex's, and volatile globals
Mutex uLCD_mutex;                                                               // LCD will be written by updating alarm time, updating current time, and when alarm goes off
Mutex PC_mutex;                                                                 // The PC Serial port can be written by either Bluetooth or the Pi when debugging
volatile unsigned int alarmState = ALARM_OFF;                                   // power up with alarm off
volatile uint8_t buffy[128];
struct tm alarm_time;                                                           // The alarm time, to be written by the Pi at the beginning of execution

unsigned int flashRed = 0xFF0000;                                               // the color to flash when the alarm is going off (RED)
void dist (int distance);                                                       // declaring the interrupt routine for sonar
void SnoozeOver();                                                              // The snooze Timeout's callback function
Timeout Snooze;                                                                 // The timeout for 1-4 minutes when you snooze the alarm
Timeout IncTime;                                                                // To minimize traffic between Pi and mbed, we will set up a real-time clock to increment current_time
volatile unsigned int waitingMin = 0;                                           // This lets us know whether to attach something to IncTime timeout

///   Declaring all hardware object
RawSerial Bluetooth(p13, p14);                                                  // Adafruit Bluetooth Module:   p13 - TX, p14 - RX
//RawSerial PC(USBTX, USBRX);                                                   // debug connection to the PC:  USBTX - TX, USBRX - RX 
USBSerial Pi;                                                                   // Pi USB connection from breakout to D+, D-
ultrasonic Sonar(p6, p7, .1, 1, &dist);                                         // the sonar sensor (1 of them);
uLCD_4DGL uLCD(p28, p27, p30);                                                  // LCD screen for time, status: p28 - TX, p27 - RX, p30 - reset
Motor leftWheel(p23, p21, p22);                                                 // H-bridge for left motor:     p23 - PWMA, p21 - fwd (AI1), p22 - rev (AI2)
Motor rightWheel(p26, p24, p25);                                                // H-bridge for right motor:    p26 - PWMB, p24 - fwd (BI1), p25 - rev (BI2)
DigitalIn AlarmOff(p20, PullUp);                                                // push-button that turns off motors on mbed, and speaker on Pi
DigitalIn AlarmSnooze(p19, PullUp);                                             // push-button that snoozes alarm for 1 minute by default
DigitalOut led1(LED1);                                                          // LEDs on mbed for debugging
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);



void checkAlarm()                                                               // The thread which both checks the alarm time and prints to uLCD
{
    char cTime[32];
    char aTime[32];
    time_t seconds;
    while (1)
    {
        if (alarm_time.tm_min == localtime(&seconds)->tm_min &&
            alarm_time.tm_hour == localtime(&seconds)->tm_hour &&
            alarmState != ALARM_SHUTOFF && alarmState != ALARM_SNOOZE && 
            alarmState != ALARM_ON_HAZARD) alarmState = ALARM_ON;               // turn the alarm on, when it is time for the alarm to go off
        if (alarmState == ALARM_ON && AlarmOff == 0) 
            alarmState = ALARM_SHUTOFF;                                         // AlarmOff being depressed results in logic 0 because it is pulled up in the design
        if (alarmState == ALARM_ON && AlarmSnooze == 0)
        {
            alarmState = ALARM_SNOOZE;
            Snooze.attach(&SnoozeOver, 60.0 * SNOOZE_TIME);                     // Start a Timeout for SNOOZE_TIME minutes to snooze
        }
        if (alarmState == ALARM_ON)
        {
            uLCD_mutex.lock();
            uLCD.filled_rectangle(0, 0, 127, 127, flashRed);                    // flash the whole screen
            uLCD_mutex.unlock();
            flashRed = (flashRed == 0xFF0000) ? 0x000000 : 0xFF0000;            // invert the red to black   
        }
        else
        {
            if (flashRed == 0)                                                  // if flashRed is 0 (BLACK), then the screen is red and needs to be cleared quickly
            {
                uLCD_mutex.lock();
                uLCD.filled_rectangle(0, 0, 127, 127, flashRed);                // fill the screen with black
                uLCD_mutex.unlock();
                flashRed = 0xFF0000;                                            // set flashRed make to 0xFF0000 (RED) so this code segment runs once at most per alarm_off
            }                 
            seconds = time(NULL);                                               // Tell RTC to update                                       
            strftime(cTime, 32, "%I:%M %p\n", localtime(&seconds));             // turn the localtime(&seconds) time_t struct into a string (character array)
            strftime(aTime, 32, "%I:%M %p\n", &alarm_time);                     // turn the alarm_time tm struct into a string "aTime"
            uLCD_mutex.lock();
            uLCD.locate(0,0);
            uLCD.printf("%d\r\n", alarmState);                                  // The state of the machine, mainly for debugging purposes
            uLCD.locate(0,3);
            uLCD.printf("%s\r\n", cTime);
            uLCD.locate(0,6);
            uLCD.printf("%s\r\n", aTime);
            uLCD.color(BLUE);
            uLCD.locate(0,1);
            uLCD.printf("Current time:\r\n");
            uLCD.locate(0,4);
            uLCD.printf("Alarm time:\r\n");
            uLCD.color(WHITE);
            uLCD_mutex.unlock();
        }
        Thread::wait(10);
    }
}

void SnoozeOver()                                                               // the end of a snooze turns the alarm back on
{
    led3 = !led3;
    if (alarmState == ALARM_SNOOZE) alarmState = ALARM_ON;
}

void blueRX()                                                                   // the interrupt for an incoming Bluetooth signal
{
    char curr_in;
    while (Bluetooth.readable())
    {
        curr_in = (char) Bluetooth.getc();
        if (curr_in == 'a') alarmState = ALARM_ON;                              // TODO: Remove these once time comparisons are implemented, unless we want to leave this debug method
        else if (curr_in == 'b') alarmState = ALARM_SHUTOFF;
        else if (alarmState == ALARM_ON && curr_in == '!')                      // reacting to button hit/release
        {
            
            if (Bluetooth.getc() == 'B')                                        // '!' alone is not a button, must be followed by 'B', then [1,4]
            {
                char snoozeMins = Bluetooth.getc();                             // making assumption that one does not hold the button for very long
                //if (snoozeMins <= '4' && snoozeMins >= '1')                     
                //{
                led2 = !led2;
                alarmState = ALARM_SNOOZE;
                Snooze.attach(&SnoozeOver, 60.0 * SNOOZE_TIME);                 // Start a Timeout for some number of minutes
                //}
            }
        }
        //PC_mutex.lock();                                                      // uncomment if using PC serial to debug
        //PC.putc(curr_in);
        //PC_mutex.unlock();
    }
}

//void PCRX()                                                                   // the interrupt for an incoming PC signal
//{                                                                             // uncomment if using PC serial to debug
//    char curr_in;
//    while (PC.readable())
//    {
//        //PC_mutex.lock();
//        //curr_in = (char) PC.getc();
//        //PC_mutex.unlock();
//        Bluetooth.putc(curr_in);
//    }
//}

void setMotors()
{
    float lMotor = 0.0;
    float rMotor = 0.0;
    while (1)
    {
        switch (alarmState)
        {
        case (ALARM_ON):
            lMotor = 0.5;
            rMotor = 0.5;
                                                                                // If you want a more random movement, uncomment the following code
//            lMotor = ((float) rand()) / RAND_MAX;                               // random value between 0 (off) and 1 (forward);
//            rMotor = lMotor + 0.3 * (((float) rand())/RAND_MAX - 0.5);          // random value between lMotor-0.3, lMotor+0.3
//            if (rMotor < 0) rMotor = 0;                                         // dont want bot moving backwards, so guarantee positive motor speed 
            break;
        case (ALARM_ON_HAZARD):
            lMotor = 1.0;                                                       // turn until there is no hazard
            rMotor = 0;
            break;
        default:
            lMotor = 0.0;
            rMotor = 0.0;
        }
        leftWheel.speed(lMotor);
        rightWheel.speed(rMotor);
        Thread::wait(500);
    }
}

void checkSonar()                                                               // The sonar-checking thread
{
    while (1)
    {
        Sonar.checkDistance();
        Thread::wait(100);                                                      // check distance 10 times a second
    }   
}

void dist(int distance)                                                         // This method gets called by the ultrasonic object when checkDistance reveals a change
{   
    led3 = !led3;
    //PC_mutex.lock();                                                          // uncomment if using PC Serial to debug
    //PC.printf("Distance %d mm \r\n", distance);
    //PC_mutex.unlock();
    if (distance < 600 && alarmState == ALARM_ON) alarmState = ALARM_ON_HAZARD; // if there is a hazard, detect it and change the state
    if (distance >= 600 && alarmState == ALARM_ON_HAZARD) alarmState= ALARM_ON; // if there is no longer a hazard, change the state back to what it was
}

void MinutePassed()                                                             // IncTime timeout calls this method every minute
{
    if (alarmState == ALARM_SHUTOFF) alarmState = ALARM_OFF;                    // ALARM_OFF is idle state, cannot get to ALARM_ON from ALARM_SHUTOFF by design
}

int main() 
{   
    uLCD.cls();
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.color(0xFFFFFF);
    uLCD.baudrate(3000000);
    uLCD.printf("Set your\nalarm.");
    uint8_t buf[128];
    std::string buf_text;
    
    Pi.scanf("%s", buf);                                                        // Read current time over serial from Pi
    //PC.printf("recv: %s\r\n", buf);                                             // Print current time (in seconds) to PC
    buf_text = (const char*)buf;                                                // Convert buffer to string
    int buf_int = std::atoi(buf_text.c_str());                                  // Convert string to int 
    set_time(buf_int);                                                          // Set the RTC
    
    Pi.scanf("%s", buf);                                                        // Read alarm time over serial from Pi
    //PC.printf("Alarm recv: %s\r\n", buf);                                       // Print alarm time to PC
    buf_text = (const char*)buf;                                                // Convert buffer to string
    std::string hours = buf_text.substr(0,2);                                   // Extract alarm hours
    std::string minutes = buf_text.substr(2,3);                                 // Extract alarm minutes
    int hour_int = std::atoi(hours.c_str());                                    // Convert alarm hour string to int 
    int min_int = std::atoi(minutes.c_str());                                   // Convert alarm minute string to int
    alarm_time.tm_hour = hour_int;                                              // Set the alarm time
    alarm_time.tm_min = min_int;
    uLCD.cls();
    uLCD.text_width(2);
    uLCD.text_height(2);
    
    Sonar.startUpdates();                                                       // Start measuring the distance
    Thread thread1;
    thread1.start(callback(checkAlarm));                                        // thread1 manages the state of the alarm and status info
    Thread thread2;
    thread2.start(callback(setMotors));                                         // thread2 sets the motors given the state of the alarm device
    Thread thread3;
    thread3.start(callback(checkSonar));                                        // thread3 is sonar-checking thread. A sonar change triggers an interrupt
    Bluetooth.baud(9600);
    Bluetooth.attach(&blueRX, Serial::RxIrq);                                   // Blutooth is an interrupt 
    //PC.baud(9600);                                                              // PC UART and Bluetooth UART must match baudrates
    //PC.attach(&PCRX, Serial::RxIrq);                                            // uncomment if using PC serial to debug
    
    while(1) {
        if (waitingMin == 0)
        {
            IncTime.attach(&MinutePassed, 60.000);
            waitingMin = 1;
        }
        led1 = !led1;
        Thread::wait(500);
    }
}

