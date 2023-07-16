#include <Arduino.h>
// charlieplexing the Uno Wifi R4

// Sat 15 Jul 18:20:09 UTC 2023

// very early in development - expect nothing interesting .. yet.

#define ESSENTIAL  0 // -1

const int cplxPin[11] = {
    28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38
//   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10
};

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

   P003  0 28
   P004  1 29
   P011  2 30
   P012  3 31
   P013  4 32
   P015  5 33
   P204  6 34
   P205  7 35
   P206  8 36
   P212  9 37
   P213 10 38
*/

const int ppedPin[11] = {
    3, 4, 11, 12, 13, 15, 204, 205, 206, 212, 213 };

//  0  1   2   3   4   5    6    7    8    9   10

/* pin: 0    mapped:  3     pin: 1    mapped:  4
   pin: 2    mapped: 11     pin: 3    mapped: 12    
*/

#define COLUMNS_AA 4


void print_pinmappings(int pin) {

    int* pinPtr = (int*) cplxPin[pin];
    int thePin = (int) pinPtr;

    if (!ESSENTIAL) {
        Serial.print(" pin: ");
        Serial.print(pin);
    }

    if (!ESSENTIAL) {
        Serial.print("  mapped: ");
        Serial.print(thePin, HEX);
        Serial.print("    ");
    }

    int remainder = (pin + 1) % COLUMNS_AA ;

    if (!ESSENTIAL) {
        // Serial.println(" this is the columns part ");
        Serial.print( " remainder: ");
        Serial.println(remainder);
        // Serial.print("  ");
    }

    if (!ESSENTIAL) {
        bool flag = 0;
        if (remainder == 0) {
            flag = -1 ;
        }
        if (flag) {
            Serial.println(); flag = 0;
        } // columnize
    }
}

void gpio_setup_cplx() {
    int* pinPtr;
    for (int pin = 0; pin < 11; pin++) {

        int* pinPtr = (int*) cplxPin[pin];
        int thePin = (int) pinPtr;

        if (!ESSENTIAL) {
            Serial.print(" proprosed: ");
            Serial.print(thePin);
            Serial.print("  ");
        }

        pinMode(thePin, OUTPUT); // charlieplexed so INPUT is also useful
        digitalWrite(thePin, HIGH); // digitalWrite(cplxPin[pinPtr], LOW);

        print_pinmappings(pin); // print_pinmappings(pinPtr);

    }
    Serial.println();
    // Serial.print(" last pin in array is: ");
    // Serial.println(cplxPin[9]); // says 37 so it's cplxPin[10]
    Serial.print(" last pin in array is: ");
    Serial.println(cplxPin[10]);
}


void set_all_cplx_inputs() {
    pinMode(28, INPUT);
    pinMode(29, INPUT);
    pinMode(30, INPUT);
    pinMode(31, INPUT);
    pinMode(32, INPUT);
    pinMode(33, INPUT);
    pinMode(34, INPUT);
    pinMode(35, INPUT);
    pinMode(36, INPUT);
    pinMode(37, INPUT);
    pinMode(38, INPUT);
}

void set_as_input(int pin) {
    pinMode(cplxPin[pin], INPUT);
}

void set_as_output(int pin) {

    int outpin = cplxPin[pin];

    Serial.print(outpin);

    if (outpin > 38) {
        Serial.println("ERROR");
        while(-1);
    }

    pinMode(outpin, OUTPUT);

    Serial.println(" is set to OUTPUT");

}

void set_high(int pin) {
    digitalWrite(cplxPin[pin], HIGH);
}

void set_low(int pin) {
    digitalWrite(cplxPin[pin], LOW);
}

unsigned long currentMicros = micros();
unsigned long int oldMicros = micros();

// bool enable_display = 0;

// 800 to 100 good
#define ABS_MIN   38 // no lower than this
#define xON_TIME   150 +         580
#define xGAP_TIME  ABS_MIN + 4 +   -12

#define pON_TIME   730
#define pGAP_TIME   26 // 26


#define jON_TIME    950
#define jGAP_TIME    32


#define ON_TIME    4000
#define GAP_TIME   4000

#define TIMEOUT    14333

#define OFF_TIME ON_TIME + GAP_TIME

bool eval_timeout() {

    unsigned long currentMicros = micros();
    unsigned long intervalAsFound = currentMicros - oldMicros;

    if (intervalAsFound > TIMEOUT) {
        oldMicros = currentMicros;
        return  -1;
    }
    return 0;
}

/*
   in forth-like interpreter:
       28 input
       31 output
       37 output 37 high 31 low
*/

void do_a_thing() {
    set_all_cplx_inputs();

    bool enable_display = eval_timeout();

    if (!enable_display) {
        pinMode(31, INPUT);
        pinMode(37, INPUT);
    }

    if (enable_display) {
        pinMode(31, OUTPUT);
        pinMode(37, OUTPUT);
        digitalWrite(31, LOW);  // 3
        digitalWrite(37, HIGH); // 9
    }
    // Serial.write('E');
}

/*
28 input ok
37 output ok
31 output ok
37 high LED lights
*/

void test_me_cplx() {
    Serial.println();
    Serial.println();
    Serial.println(" light just one cplx LED - do not care which yet ");
    Serial.println();
    Serial.println("   Sun 16 Jul 02:05:31 UTC 2023 time-slicing LED array - charlie file ");

    Serial.println();

    gpio_setup_cplx();
    // do_a_thing(); // show what we can do now
    Serial.println();
}

// END
