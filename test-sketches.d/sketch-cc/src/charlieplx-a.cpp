#include <Arduino.h>
// charlieplexing the Uno Wifi R4

// Mon 17 Jul 23:58:00 UTC 2023

// very early in development - expect nothing interesting .. yet.

// #define ESSENTIAL 0 // -1
#define ESSENTIAL -1 // 0

const int cplxPin[11] = {
    28, 29, 30, 31, 32, 33,
    34, 35, 36, 37, 38
    //   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10
};

int pinArray[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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

const int ppedPin[11] = {3, 4, 11, 12, 13, 15, 204, 205, 206, 212, 213};

//  0  1   2   3   4   5    6    7    8    9   10

/* pin: 0    mapped:  3     pin: 1    mapped:  4
   pin: 2    mapped: 11     pin: 3    mapped: 12
*/

#define COLUMNS_AA 4

void print_pinmappings(int pin) {

    int *pinPtr = (int *)cplxPin[pin];
    int thePin = (int)pinPtr;

    if (!ESSENTIAL) {
        Serial.print(" pin: ");
        Serial.print(pin);
    }

    if (!ESSENTIAL) {
        Serial.print("  mapped: ");
        Serial.print(thePin, HEX);
        Serial.print("    ");
    }

    int remainder = (pin + 1) % COLUMNS_AA;

    if (!ESSENTIAL) {
        Serial.print(" remainder: ");
        Serial.println(remainder);
    }

    if (!ESSENTIAL) {
        bool flag = 0;
        if (remainder == 0) {
            flag = -1;
        }
        if (flag) {
            Serial.println();
            flag = 0;
        } // columnize
    }
}

void gpio_setup_cplx() {
    int *pinPtr;
    Serial.println("gpio_setup_cplx() is running..");
    for (int pin = 0; pin < 11; pin++) {

        int *pinPtr = (int *)cplxPin[pin];
        int thePin = (int)pinPtr;

        if (!ESSENTIAL) {
            Serial.print(" proprosed: ");
            Serial.print(thePin);
            Serial.print("  ");
        }

        pinMode(thePin, OUTPUT);
        digitalWrite(thePin, HIGH); // digitalWrite(cplxPin[pinPtr], LOW);

        print_pinmappings(pin); // print_pinmappings(pinPtr);
    }
    Serial.println();
}

unsigned long int test_counter_iterations[11];

void clear_counter_array() {
    for (int pos = 0; pos < 11; pos++) {
        test_counter_iterations[pos] = 0;
    }
}

bool timeout_little = 0;

void increment_test_counter() {
    test_counter_iterations[0]++;
    int seconds = 8;
    // unsigned long int goal = (((377 * seconds) / 3) * 1);
    //  377 i+terations * 8 seconds desired qty
    //  / 3 gives wall clock seconds
    // if (test_counter_iterations[0] == goal) { return -1; }
    // return 0;
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

void set_high(int pin) { digitalWrite(cplxPin[pin], HIGH); }

void set_low(int pin) { digitalWrite(cplxPin[pin], LOW); }

unsigned long int oldMicros = micros();

unsigned long int TIMEOUT = 493;

extern int pop();
extern void dup();

/*
   in forth-like interpreter:
       28 input
       31 output
       37 output 37 high 31 low
*/

uint16_t c_array = 0; // no bits set

void print_c_array() {
    int p = c_array; // normalize
    Serial.print("    c_array: ");
    Serial.print(p);
    Serial.print("   ");
}

bool eval_timeout() {
    unsigned long currentMicros = micros();
    unsigned long intervalAsFound = currentMicros - oldMicros;

    if (intervalAsFound > TIMEOUT) {
        oldMicros = currentMicros;
        return -1;
    }
    return 0;
}

unsigned long old_blanking_micros = 0;
// delay after clearing video

void hold_on_thar() {
    unsigned long duration = 0;
    do {
        duration = micros() - old_blanking_micros;
    } while (duration < 101);
    // so 32k is like 58 Hz vid refresh
    // whereas 22k is more like 68 Hz - still feel it but not obvious
    old_blanking_micros = micros();
}

void write_pos(uint8_t pos, uint8_t got) {
    pos = pos + 28;
    bool shouldGot = (got == 0);
    if (shouldGot) {
        pinMode(pos, INPUT); // finally tri-stated
        return;
    }
    // one of three tri-state states handled just above.
    // here are the other two:
    got = got - 1;
    // storage has logic level encoded as 1 or 2 - instead of 0 or 1

    pinMode(pos, OUTPUT);
    digitalWrite(pos, got); // this is the moment an LED potentially turns ON

    pos = pos - 28;
    test_counter_iterations[pos]++; // counter, per port pin, for metrics
}

void show_Array() {
    for (uint8_t pos = 0; pos < 11; pos++) {
        int got = pinArray[pos];
        write_pos(pos, got);
    }
}

int blanking_counter = 0;

const int blank_periods = 188;

void vid_blank() {
    blanking_counter++;
    bool blanked_plenty = (blanking_counter > blank_periods);

    if (blanked_plenty) {
        blanking_counter = 0;
        // first blank then pause:
        set_all_cplx_inputs();
        hold_on_thar();
    }
}

// depends: eval_timeout, show_Array, vid_blank

unsigned long master_counter = 0;

unsigned long old_loop_micros = micros();
unsigned long loop_duration = 0;

unsigned long explicit_blanking_t = 0;

void write_Charlie_pixel_array() { // in 'reading' not called even one time in
                                   // this .cpp file
                                   // - see main.cpp
    unsigned long loop_micros = micros();

    master_counter++;
    bool enable_display = eval_timeout();
    if (enable_display) {
        unsigned long delta_t = micros() - explicit_blanking_t;
        if (delta_t > 9999) {
            explicit_blanking_t = micros();
            show_Array();
            set_all_cplx_inputs();
        }
        vid_blank();
    }
    if (!enable_display) {
        vid_blank(); // best spot for the effect
    }
    loop_duration = loop_micros - old_loop_micros;
    old_loop_micros = loop_micros;
}

void write_to_Array(uint8_t pos, uint8_t got) {
    pinArray[pos] = got;
} // got can be 0 1 or 2 for tri-stating

void clear_display_data() {
    clear_counter_array(); // clear their respective counters as well
    for (uint8_t pos = 0; pos < 11; pos++) {
        write_to_Array(pos, 0); // set to INPUT
    }
}

void clear_things() {
    master_counter = 0;
    set_all_cplx_inputs();
    clear_counter_array();
}

void acld() {
    int p = pop();
    uint8_t pos = (uint8_t)p;
    write_to_Array(pos, (uint8_t)1);
    clear_things(); // acld and asbd are single-shot; this initializes metrics
                    // for measurement
} /* ( n -- ) array clr bit dynamic */

// setb
void asbd() {
    int p = pop();
    uint8_t pos = (uint8_t)p;
    write_to_Array(pos, (uint8_t)2);
    clear_things();
} /* ( n -- ) array clr bit dynamic */

void post_arrayd() {
    for (int pos = 0; pos < 11; pos++) {
        Serial.print(pinArray[pos]);
    }
    Serial.println();
}

extern void push(int n);

void l82d() { /* ( n -- ) */
    push(0);
    acld();
    push(10);
    asbd();
}

void light_l94() {
    pinMode(31, OUTPUT);
    pinMode(37, OUTPUT);
    digitalWrite(31, LOW);  // 3
    digitalWrite(37, HIGH); // 9
}

void l94() {
    bool enable_display = eval_timeout();
    if (enable_display) {
        light_l94();
        vid_blank();
    }
}

/*
const int ref = 0;
const int xcplxPin[22] = {
    28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10
};
*/

/*
28 input ok
37 output ok
31 output ok
37 high LED lights
*/

#define TIMESTAMP "Mon 17 Jul 23:58:00 UTC 2023"

void report_findings_test_timings() {
    Serial.println("report findings: the 'res' word.");
    Serial.print(" last loop duraton, in uSec: ");
    Serial.print(loop_duration);
    Serial.print("    ");
    Serial.print("\n master counter: ");
    Serial.println(master_counter);

    for (int pos = 0; pos < 11; pos++) {
        unsigned long output = test_counter_iterations[pos];
        // float test_float = 22 / 7;
        Serial.print(" counter: ");
        Serial.print(output);
        Serial.print(" ratio: ");
        char buffer[64];
        buffer[0] = '\000';
        float calculation = master_counter / 0.9999999999;
        float ratio = output / calculation;
        float r_ratio = 1 / ratio;
        Serial.print(r_ratio);
        Serial.println();
    }

    Serial.println("fin");
}

void test_me_cplx() {
    clear_counter_array();

    Serial.println();
    Serial.println();
    Serial.println(" ex. 0 asb 1 acl parr   use 0-10 as TOS for asb or acl");
    Serial.println();
    Serial.println("   " TIMESTAMP "  time-slicing LED array - TEST 17b ");

    Serial.println();

    gpio_setup_cplx();
    // do_a_thing(); // show what we can do now
    Serial.println();
}

// END
