#include <stdint.h>
#include <stdio.h>

static const uint16_t binPins[] = {0xF000, 0xF001, 0xF002, 0xF003};

// from arduino.cc github led matrix example .h file:
static const uint8_t pins[][2] = {
    // anode, cathode
    {7, 3},  {3, 7},  {7, 4},  {4, 7},  {3, 4},  {4, 3},  {7, 8},  {8, 7},

    {3, 8},  {8, 3},  {4, 8},  {8, 4},  {7, 0},  {0, 7},  {3, 0},  {0, 3},

    {4, 0},  {0, 4},  {8, 0},  {0, 8},  {7, 6},  {6, 7},  {3, 6},  {6, 3},
    {4, 6},  {6, 4},  {8, 6},  {6, 8},  {0, 6},  {6, 0},  {7, 5},  {5, 7},
    {3, 5},  {5, 3},  {4, 5},  {5, 4},  {8, 5},  {5, 8},  {0, 5},  {5, 0},
    {6, 5},  {5, 6},  {7, 1},  {1, 7},  {3, 1},  {1, 3},  {4, 1},  {1, 4},
    {8, 1},  {1, 8},  {0, 1},  {1, 0},  {6, 1},  {1, 6},  {5, 1},  {1, 5},
    {7, 2},  {2, 7},  {3, 2},  {2, 3},  {4, 2},  {2, 4},  {8, 2},  {2, 8},
    {0, 2},  {2, 0},  {6, 2},  {2, 6},  {5, 2},  {2, 5},  {1, 2},  {2, 1},
    {7, 10}, {10, 7}, {3, 10}, {10, 3}, {4, 10}, {10, 4}, {8, 10}, {10, 8},
    {0, 10}, {10, 0}, {6, 10}, {10, 6}, {5, 10}, {10, 5}, {1, 10}, {10, 1},
    {2, 10}, {10, 2}, {7, 9},  {9, 7},  {3, 9},  {9, 3},  {4, 9},  {9, 4},
};

// today's experiment AA:
typedef struct ledBits_t {
    uint16_t dir : 1;
    uint16_t bits : 11;
    uint16_t : 4; // Unused
} ledBits_s;

static const ledBits_t bitArray[] = {
    // n: anode, k: cathode
    // {7, 3},  {3, 7},  {7, 4},  {4, 7},  {3, 4},  {4, 3},  {7, 8},  {8, 7},
    // ##bookmark
    {0, 0b00010001000}, //  7n, 3k   0x088
    {1, 0b00010001000}, //  7k, 3n

    {0, 0b00010010000}, //  7n, 4k   0x090
    {1, 0b00010010000}, //  7k, 4n

    {1, 0b00000011000}, //  4k, 3n   0x018
    {0, 0b00000011000}, //  4n, 3k

    {1, 0b00110000000}, //  8k, 7n   0x180
    {0, 0b00110000000}, //  8n, 7k

    // anode, cathode
    // {3, 8},  {8, 3},  {4, 8},  {8, 4},  {7, 0},  {0, 7},  {3, 0},  {0, 3},

    {1, 0b00100001000}, //  8k, 3n   0x108
    {0, 0b00100001000}, //  8n, 3k

      //  a9876543210
    {1, 0b00100010000}, //  8k, 4n   0x110
    {0, 0b00100010000}, //  8n, 4k

      //  a9876543210
    {0, 0b00010000001}, // this is why bit 0 still gets a '1'
    {1, 0b00010000001}, //  7k, 0n   0x081

    {1, 0x7AF}};

int xmain() {

    // arduino.cc array first:
    // printf("sizeof(pins): %x\n", sizeof(pins));
    // printf("sizeof(pins[0]): %x\n", sizeof(pins[0]));

    printf("sizeof(binPins): %x\n", sizeof(binPins));

    printf("sizeof(binPins[0]): %x\n", sizeof(binPins[0]));

    // applied learning:
    // static const ledBits_t bitArray[] = {{0x7FF, 0}, {0b111111, 1}, {0x7AF,
    // 1}};

    printf("sizeof(bitArray): %x\n", sizeof(bitArray));

    printf("sizeof(bitArray[0]): %x\n\n", sizeof(bitArray[0]));

    int members = (sizeof(bitArray) / sizeof(bitArray[0]));
    // sizeof(bitArray): c
    // sizeof(bitArray[0]): 2

    for (int n = 0; n < members; n++) {
        if (n < 10) { printf(" "); } // left pad
        printf("bitArray[%d].dir:  %d    ", n, bitArray[n].dir);
        if (n < 10) { printf(" "); } // left pad
        printf("bitArray[%d].bits: 0x%03x   \n", n, bitArray[n].bits);
    }
    printf("\n");

    // printf("static const ledBits_t bitArray[] = {\n");

    // printf("    {0, 0x7FF},         {1, 0b111111},\n");
    // printf("    {1, 0b01000010000}, // LED array pins 9 and 4, inverted (9 "
           // "low; 4 high)\n");
    // printf("    {0, 0b10000000001}, {1, 0b10000000001},\n");
    // printf("    {1, 0x7AF}};\n");

    printf("}; // Thu 20 Jul 02:23:31 UTC 2023\n\n");

    return 0;
}

int main() {
    int result = xmain();
    printf("xmain() result: \n\n", result);
    printf("Thu 20 Jul 03:03:02 UTC 2023\n\n");
    return 0;
}
;
