#include <Arduino.h>
// working program - bare bones - Tue 11 Jul 17:33:04 UTC 2023

// closest upstream work is probably:

// rp2040-wokwi-ser_interpret/interpreter-c/interpreter-c.ino

// https://github.com/wa1tnr/rp2040-wokwi-ser_interpret/blob/main/interpreter-c/interpreter-c.ino
// the interpreter-c.json there is equally suitable for a test on wokwi
// however wokwi only has the old school Uno R3 ;)

#include <unistd.h>

// buf_ptr = * & buffer; \

#define lcl_printf() \
      buf_ptr = & buffer[0]; \
      memcpy(buffering, buf_ptr, sizeof buffer); \
      print_buffer();

/* test program includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h> // strlen only

/* Structure of a dictionary entry */
typedef struct {
  const char *name;
  void (*function)();
} entry;

/* Data stack for parameter passing
   This "stack" is circular,
   like a Green Arrays F18A data stack,
   so overflow and underflow are not possible
   Number of items must be a power of 2 */
const int STKSIZE = 8;
const int STKMASK = 7;
int stack[STKSIZE];
int p = 0;

/* TOS is Top Of Stack */
#define TOS stack[p]
/* NAMED creates a string in flash */
#define NAMED(x, y) const char x[]=y

/* Terminal Input Buffer for interpreter */
const byte maxtib = 16;
char tib[maxtib];
/* buffer required for strings read from flash */
char namebuf[maxtib];
byte pos;

/* push n to top of data stack */
void push(int n) {
  p = (p + 1)& STKMASK;
  TOS = n;
}

/* return top of stack */
int pop() {
  int n = TOS;
  p = (p - 1)& STKMASK;
  return n;
}

/* Global delay timer */
int spd = 15;

/* top of stack becomes current spd */
NAMED(_speed, "speed");
void speed() {
  spd = pop();
}

/* discard top of stack */
NAMED(_drop, "drop");
void drop() {
  pop();
}

/* recover dropped stack item */
NAMED(_back, "back");
void back() {
  for (int i = 1; i < STKSIZE; i++) drop();
}

/* copy top of stack */
NAMED(_dup, "dup");
void dup() {
  push(TOS);
}

/* exchange top two stack items */
NAMED(_swap, "swap");
void swap() {
  int a;
  int b;
  a = pop();
  b = pop();
  push(a);
  push(b);
}

/* copy second on stack to top */
NAMED(_over, "over");
void over() {
  int a;
  int b;
  a = pop();
  b = pop();
  push(b);
  push(a);
  push(b);
}
/* add top two items */
NAMED(_add, "+");
void add() {
  int a = pop();
  TOS = a + TOS;
}

/* bitwise and top two items */
NAMED(_and, "and");
void and_() {
  int a = pop();
  TOS = a & TOS;
}

/* inclusive or top two items */
NAMED(_or, "or");
void or_() {
  int a = pop();
  TOS = a | TOS;
}

/* exclusive or top two items */
NAMED(_xor, "xor");
void xor_() {
  int a = pop();
  TOS = a ^ TOS;
}

/* invert all bits in top of stack */
NAMED(_invert, "invert");
void invert() {
  TOS = ~(TOS);
}

/* negate top of stack */
NAMED(_negate, "negate");
void negate() {
  TOS = -(TOS);
}

/* print carriage return */
NAMED(_cr, "cr");
void cr() {
  Serial.print('\n');
}

/* destructively display top of stack, decimal */
NAMED(_dot, ".");
void dot() {
  Serial.print(pop());
  Serial.print(" ");
}

/* destructively display top of stack, hex */
NAMED(_dotHEX, ".h");
void dotHEX() {
  Serial.print(0xffff & pop(), HEX);
  Serial.print(" ");
}

/* display whole stack, hex */
NAMED(_dotShex, ".sh");
void dotShex() {
  for (int i = 0; i < STKSIZE; i++) dotHEX();
}

/* display whole stack, decimal */
NAMED(_dotS, ".s");
void dotS() {
  for (int i = 0; i < STKSIZE; i++) dot();
}

/* delay TOS # of milliseconds */
NAMED(_delay, "delay");
void del() {
  delay(pop());
}

/* Toggle pin at TOS and delay(spd), repeat... */
NAMED(_wiggle, "wiggle");
void wiggle() {
  int a = pop();
  pinMode(a, OUTPUT);
  for (int i = 0; i < 20; i++) {
    digitalWrite(a, HIGH);
    delay(20);
    digitalWrite(a, LOW);
    delay(400);
  }
}

/* TOS is pin number, set it HIGH */
NAMED(_high, "high");
void high() {
  digitalWrite(pop(), HIGH);
}

/* set TOS pin LOW */
NAMED(_low, "low");
void low() {
  digitalWrite(pop(), LOW);
}

/* read TOS pin */
NAMED(_in, "in");
void in() {
  TOS = digitalRead(TOS);
}

/* make TOS pin an input */
NAMED(_input, "input");
void input() {
  pinMode(pop(), INPUT);
}

/* make TOS pin an output */
NAMED(_output, "output");
void output() {
  pinMode(pop(), OUTPUT);
}

/* make TOS pin an input with weak pullup */
NAMED(_input_pullup, "input_pullup");
void input_pullup() {
  pinMode(pop(), INPUT_PULLUP);
}

/* dump 16 bytes of RAM in hex with ascii on the side */
void dumpRAM() {
  char buffer[5] = "";
  char *ram;
  int p = pop();
  ram = (char*)p;
  sprintf(buffer, "%4x", p);
  Serial.print(buffer);
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    char c = *ram++;
    sprintf(buffer, " %2x", (c & 0xff));
    Serial.print(buffer);
  }
  ram = (char*)p;
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    buffer[0] = *ram++;
    if (buffer[0] > 0x7f || buffer[0] < ' ') buffer[0] = '.';
    buffer[1] = '\0';
    Serial.print(buffer);
  }
  push(p + 16);
}

/* dump 256 bytes of RAM */
NAMED(_dumpr, "dump");
void rdumps() {
  for (int i = 0; i < 16; i++) {
    Serial.println();
    dumpRAM();
  }
}

/* End of Forth interpreter words */
/* ******************************************** */
/* Beginning of application words */




/* End of application words */
/* ******************************************** */
/* Now build the dictionary */

/* empty words don't cause an error */
NAMED(_nop, " ");
void nop() { }

/* Forward declaration required here */
NAMED(_words, "words");
void words();

/* table of names and function addresses in flash */
const entry dictionary[] = {
  {_nop, nop},
  {_words, words},
  {_dup, dup},
  {_drop, drop},
  {_back, back},
  {_swap, swap},
  {_over, over},
  {_add, add},
  {_and, and_},
  {_or, or_},
  {_xor, xor_},
  {_invert, invert},
  {_negate, negate},
  {_cr, cr},
  {_dotS, dotS},
  {_dotShex, dotShex},
  {_dot, dot},
  {_dotHEX, dotHEX},
  {_delay, del},
  {_high, high},
  {_low, low},
  {_in, in},
  {_input, input},
  {_output, output},
  {_input_pullup, input_pullup},
  {_wiggle, wiggle},
  {_dumpr, rdumps},
  {_speed, speed}
};

/* Number of words in the dictionary */
const int entries = sizeof dictionary / sizeof dictionary[0];

/* Display all words in dictionary */
void words() {
  for (int i = entries - 1; i >= 0; i--) {
    strcpy(namebuf, dictionary[i].name);
    Serial.print(namebuf);
    Serial.print(" ");
  }
}

/* Find a word in the dictionary, returning its position */
int locate() {
  // for (int i = entries; i >= 0; i--) {
  for (int i = (entries - 1); i >= 0; i--) {
    strcpy(namebuf, dictionary[i].name);
    if (!strcmp(tib, namebuf)) {
        return i;
    }
  }
  return 0;
}

/* Is the word in tib a number? */
int isNumber() {
  char *endptr;
  strtol(tib, &endptr, 0);
  if (endptr == tib) return 0;
  if (*endptr != '\0') return 0;
  return 1;
}

/* Convert number in tib */
int number() {
  char *endptr;
  return (int) strtol(tib, &endptr, 0);
}

char ch;

void ok() {
  if (ch == '\n') Serial.println("ok");
}

/* Incrementally read command line from serial port */
byte reading() {
  if (!Serial.available()) return 1;
  ch = Serial.read();
  Serial.write(ch); // for current environment - SerialMonitor may double-print on this
  if (ch == '\r') return 1;
  if (ch == '\n') return 0;
  if (ch == ' ') {
      return 0;
  }
  if (pos < maxtib) {
    tib[pos++] = ch;
    tib[pos] = 0;
  }
  return 1;
}

/* Block on reading the command line from serial port */
/* then echo each word */

void readword() {
  pos = 0;
  tib[0] = 0;
  while (reading());
  // Serial.print(tib);
  // Serial.print(" ");
}

/* Run a word via its name */
void runword() {
  int place = locate();
  if (place != 0) {
    dictionary[place].function();
    ok();
    return;
  }
  if (isNumber()) {
    push(number());
    ok();
    return;
  }
  Serial.println("?");
}


/* test program */
/* sprintf write and friends */

/* Tue 28 Dec 03:16:49 UTC 2021 */

/* write and friends */

// #include <stdio.h>
// #include <unistd.h>
// #include <stdint.h>
// #include <string.h> // strlen only

char buffering[64];

void print_buffer(void) {
  Serial.print(buffering);
}

void space_it(void) {
    sprintf(buffering, "%c", ' ');
    // Serial.print(buffering);
    print_buffer();
} // good 18:06 UTC 28 Dec 2021

void newline(void) {
    sprintf(buffering, "%c", '\n');
    print_buffer();
}

int test_program_a(void) {
    newline();
    space_it();
    
    char buffer[64]; // 32 also 64
    char* buf_ptr;

    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 'c';
    buffer[3] = '\000';

    buf_ptr = buffer;

    int buf_size, buf_ptr_size;

    buf_size = sizeof(buffer); // captures "abc\000" size
    buf_ptr_size = sizeof(buf_ptr);

    int buf_len = strlen(buffer);

    size_t gottem;

    sprintf(buffering, "%c", '\'');
    print_buffer();

    memcpy(buffering, buf_ptr, sizeof buffer);
    print_buffer();


    sprintf(buffering, "%c%c", '\'', ' ');
    print_buffer();

    sprintf(buffering, "%s ", " is the buffer contents");
    print_buffer();

    // gottem = write(1, buf_ptr, sizeof(buf_ptr));

    sprintf(buf_ptr, "\n         sizeof(buf_ptr) is  %d", buf_ptr_size);
    // memcpy dest source size:

    lcl_printf();
/*
    buf_ptr = * & buffer; // way overdone - a test only.
    memcpy (buffering, buf_ptr, sizeof buffer );
    // printf(buf_ptr);
    print_buffer();
*/

    // sprintf(buf_ptr, "%s", "\n         sizeof(buffer)  is ", '\000');

    // lcl_printf();
    /*
    buf_ptr = * & buffer;
    memcpy(buffering, buf_ptr, sizeof buffer);
    print_buffer();
    */

    // extra copy:
    // buf_size = sizeof(buffer[0]); // captures "abc\000" size

    // snprintf(buf_ptr, buf_size, "%d%c%c", buf_size, '\n', '\000'); // related to string length, possibly
    // lcl_printf();
    // lcl_printf();
    /* buf_ptr = * & buffer;
    memcpy(buffering,buf_ptr, sizeof buffer);
    print_buffer(); // 32
    */
    // printf(buf_ptr);

    // buf_ptr = buffer;
    // snprintf(buf_ptr, sizeof(buf_ptr), "%s", "         strlen(buffer)  is ", '\000');
    // lcl_printf();
    /* buf_ptr = * & buffer;
    memcpy(buffering, buf_ptr, sizeof buffer);
    print_buffer();
    */

    // sprintf(buf_ptr, " %d\n", buf_len); // related to string length, possibly
    // lcl_printf();
    /* buf_ptr = * & buffer;
    memcpy(buffering, buf_ptr, sizeof buffer);
    print_buffer();
    */

    // printf(buf_ptr);

    // uint16_t adrs;
    // adrs = (uint16_t) & buf_ptr;

    // sprintf(buf_ptr, "%s", "adrs (& buf_ptr) in hex is        ");
    // lcl_printf();

    /* buf_ptr = * & buffer;
    memcpy(buffering, buf_ptr, sizeof buffer);
    print_buffer();
    */
    // print the buffer's address in ram
    // sprintf(buf_ptr, "0x%.8X\n", adrs);
    // lcl_printf();
    /*
    buf_ptr = * & buffer;
    memcpy(buffering, buf_ptr, sizeof buffer);
    print_buffer();
    */
    // printf(buf_ptr);


    // sprintf(buf_ptr, "%s", "adrs (& buf_ptr) in decimal is  ");
    // lcl_printf();

    // sprintf(buf_ptr, "%c%.11u\n\n", ' ', adrs);
    // lcl_printf();
}

/* Arduino main loop */

void setup() {
  /* functioning program Uno R4 WiFi 17:32 UTC 11 July 2023 */
  /* basic printing works - press RESET after upload or add timing delays to see output */
  Serial.begin(115200); // this
  delay(500);
  // while (!Serial);  not on uno r4 wifi as it has a bridge chip ESP32 iirc
  Serial.println();
  delay(500);
  Serial.println();
  delay(500);
  Serial.println();
  delay(500);
  Serial.println ("Forth-like interpreter:");
  Serial.println();
  Serial.println("dvlp-aa  8ffb024  Wed Jul 12 00:11:02 UTC 2023");
  words();
  Serial.println(" ");
  // Serial.println("NOT_READY");
  // test_program_a();
  // Serial.println("TRAPPED_READY");
  // while(-1);
  Serial.println("READY");
}

void loop() {
  readword();
  runword();
}
/* END */ /* UNTESTED EDIT */
