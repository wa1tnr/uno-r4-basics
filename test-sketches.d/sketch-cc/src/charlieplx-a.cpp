#include <Arduino.h>
// charlieplexing the Uno Wifi R4

// Sat 15 Jul 18:20:09 UTC 2023

// very early in development - expect nothing interesting .. yet.

#define ESSENTIAL  0 // -1

const int cplxPin[11] = {
    28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38
//   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10
};

int pinArray[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

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
    Serial.println("gpio_setup_cplx() is running..");
    for (int pin = 0; pin < 11; pin++) {

        int* pinPtr = (int*) cplxPin[pin];
        int thePin = (int) pinPtr;

        if (!ESSENTIAL) {
            Serial.print(" proprosed: ");
            Serial.print(thePin);
            Serial.print("  ");
        }

        pinMode(thePin, OUTPUT);
        digitalWrite(thePin, HIGH); // digitalWrite(cplxPin[pinPtr], LOW);
        Serial.print("DEBUG what is going on no output LED"); // okay nothing printed so too many define guards in c preprocessing ;)

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

// void set_as_input(int pin) { }

// void set_as_output(int pin) { }

void set_high(int pin) {
    digitalWrite(cplxPin[pin], HIGH);
}

void set_low(int pin) {
    digitalWrite(cplxPin[pin], LOW);
}

unsigned long int oldMicros = micros();

#define TIMEOUT    933

bool eval_timeout() {
    unsigned long currentMicros = micros();
    unsigned long intervalAsFound = currentMicros - oldMicros;

    if (intervalAsFound > TIMEOUT) {
        oldMicros = currentMicros;
        return  -1;
    }
    return 0;
}

void vid_blank() {
    set_all_cplx_inputs();
    oldMicros = micros();
}

void light_l82() {
    pinMode(38, OUTPUT);
    pinMode(28, OUTPUT);
    digitalWrite(38, HIGH); // 10
    digitalWrite(28, LOW);  //  0
}

// void Xunlight_l82() { }

void l82() {
    bool enable_display = eval_timeout();
    if (enable_display) {
        light_l82(); vid_blank();
    }
}

extern int pop();

extern void dup();

void l82d() { /* ( n -- ) */
    vid_blank();
    // dup(); pop();

    for (int count = pop(); count > 0; count--) {
        light_l82(); delayMicroseconds(33);
        vid_blank(); delayMicroseconds(12000);
    }
    pop();
    vid_blank();
}

/*
   in forth-like interpreter:
       28 input
       31 output
       37 output 37 high 31 low
*/


void light_l94() {
    pinMode(31, OUTPUT);
    pinMode(37, OUTPUT);
    digitalWrite(31, LOW);  // 3
    digitalWrite(37, HIGH); // 9
}

// void Xunlight_l94() { }

void l94() {
    bool enable_display = eval_timeout();

    if (!enable_display) {
    }

    if (enable_display) {
        light_l94();
        vid_blank();
    }
}

uint16_t c_array = 0; // no bits set



/*  setting it up this way (see below) allows things,
    since it compiles clean without any comment mark-up.

    In particular #ifdef 0  .. #endif  vs  slash star comment star slash foo

    One of them does not really permit/allow/like bad code inside as a container

    But clean code that is obviously not for a real program is okay.

    Pretty sure that slash star any text star slash is the one that is safer,
    but has the disadvantage of a loss of syntax highlighting, in rvim.

    So, #ifdef is preferred, but more restrictive what can be in it.
*/

/*
char myJunkStr[] = " " \
    "  $  gforth"                                    \
                                                     \
    " decimal 28 hex . decimal 38 hex . 1C 26  ok"   \
    " decimal 28 2 base ! . decimal 11100  ok"       \
    " decimal 38 2 base ! . decimal 100110  ok"      ;
*/

/*

   32:   100000
         100000   and:
         100000   that's why.
         111111   0x3F

         hex 20 negate 1 + negate bina   . 11111  ok
         hex 1F                   bina   . 11111  ok

*/

void print_c_array() {
    int p =  c_array; // normalize
    Serial.print("    c_array: ");
    Serial.print(p);
    Serial.print("   ");
}


void write_pos(uint8_t pos, uint8_t got) {
    pos = pos + 28;
    if (got == 1) { pinMode(pos, OUTPUT); }
    if (got == 2) { pinMode(pos, OUTPUT); }
    if (got == 1 || got == 2) { got = got - 1;  digitalWrite(pos, got); return; }
    pinMode(pos, INPUT); // finally tri-stated
}

void show_Array() {
    for (uint8_t pos = 0; pos < 11; pos++) {
        int got = pinArray[pos];
        write_pos(pos, got);
    }
}

void write_Array() { // in 'reading'
    bool enable_display = eval_timeout();
    if (enable_display) {
        vid_blank();
        show_Array();
        while(eval_timeout());
    }
}

void write_to_Array(uint8_t pos, uint8_t got) {
    pinArray[pos] = got;
}

void acld() {
    int p = pop();
    uint8_t pos = (uint8_t) p;
    write_to_Array(pos, (uint8_t) 1);
} /* ( n -- ) array clr bit dynamic */

// setb
void asbd() {
    int p = pop();
    uint8_t pos = (uint8_t) p;
    write_to_Array(pos, (uint8_t) 2);
} /* ( n -- ) array clr bit dynamic */

void post_arrayd() {
    for (int pos = 0; pos < 11; pos++) {
        Serial.print(pinArray[pos]);
    }
    Serial.println();
}

void do_a_thing() {
    l82();
    l94();
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

#define TIMESTAMP "GOT IT: Sun 16 Jul 04:08:12 UTC 2023"
void test_me_cplx() {
    Serial.println();
    Serial.println();
    Serial.println(" light individual LEDs one after another with video blanking - GOT IT");
    Serial.println();
    Serial.println("   " TIMESTAMP  "  time-slicing LED array - charlie file ");

    Serial.println();

    gpio_setup_cplx();
    // do_a_thing(); // show what we can do now
    Serial.println();
}

// END
