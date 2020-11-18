#include "mbed.h"
#include "USBSerial.h"
#include "uLCD_4DGL.h"
#include <string>
#include <cstdlib>
#define NULL 0

using namespace std;

uLCD_4DGL LCD(p28, p27, p30); // create a global uLCD object
//Pi mbed USB Slave function
// connect mbed to Pi USB
RawSerial  pc(USBTX, USBRX);
USBSerial pi;




int main(void) {
    uint8_t buf[128];
    string buf_text;
    char time_buffer[32];
    
    pi.scanf("%s", buf); //read serial data from Pi
    pi.printf("recv: %s\n", buf); //Print data back to terminal
    pc.printf("recv: %s\r\n", buf); //Print data to PC terminal
    buf_text = (const char*)buf; //convert buffer to string
    int buf_int = std::atoi(buf_text.c_str()); //convert string to int
       
    pc.printf("Bar: %i\n", buf_int); //print initialized buf int
    set_time(buf_int); //set the clock using seconds since 1970
    LCD.color(RED); //1605658168
    while (true) {
        time_t seconds = time(NULL); //start RTC
        //pc.printf("Time as seconds since January 1, 1970 = %u\n", (unsigned int)seconds);
        strftime(time_buffer, 32, "%I:%M %p\n", localtime(&seconds));
        LCD.locate(5,5);
        //LCD.printf("%s", ctime(&seconds));
        LCD.printf("%s", time_buffer); //print current time to LCD
        pc.printf("%s", ctime(&seconds)); //print full date to PC  
        //pc.printf("Time as a custom formatted string = %s", time_buffer);
        wait(1);
    }
}