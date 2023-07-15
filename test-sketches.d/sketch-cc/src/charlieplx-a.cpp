#include <Arduino.h>
// charlieplexing the Uno Wifi R4

// Sat 15 Jul 18:20:09 UTC 2023

// very early in development - expect nothing interesting .. yet.


/* variant.cpp says:

   P003 is D28
   P004 is D29
   P011 is D30
   P012 is D31
   P013 is D32
   P015 is D33
   P204 is D34
   P205 is D35
   P206 is D36
   P212 is D37
   P213 is D38

   they are continguous D28 thru D38

   https://github.com/arduino/ArduinoCore-renesas/blob/main/variants/UNOWIFIR4/variant.cpp#L59-L69

*/

/*

   P003  0
   P004  1
   P011  2
   P012  3
   P013  4
   P015  5
   P204  6
   P205  7
   P206  8
   P212  9
   P213 10
*/

const int mappedPin[11] = {
    3, 4, 11, 12, 13, 15, 204, 205, 206, 212, 213 };

//  0  1   2   3   4   5    6    7    8    9   10

/* pin: 0    mapped:  3     pin: 1    mapped:  4
   pin: 2    mapped: 11     pin: 3    mapped: 12    
*/

void gpio_setup_cplx() {
    for (int pin = 0; pin < 4; pin++) {
        Serial.print(" pin: ");
        Serial.print(pin);
        Serial.print("    mapped: ");
        Serial.print(mappedPin[pin]);
        Serial.print("    ");
    }
    Serial.println();
}

void setup_charlie() {
    gpio_setup_cplx();
}

void test_me_cplx() {
    Serial.println();
    Serial.println();
    Serial.println(" test me in the new charlieplex cpp file ");
    Serial.println();
    Serial.println("   Sat 15 Jul 18:20:09 UTC 2023 charlieplexing cpp file ");
    Serial.println();
    gpio_setup_cplx();
    Serial.println();
}
