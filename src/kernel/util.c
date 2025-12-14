#include "util.h"
#include "kernel/pit.h"

static u32 next = 1;

void memcpy(char* dest, char* src, int num_bytes) {
	for (int i = 0; i < num_bytes; i++) {
		*(dest + i) = *(src + i);
	}
}

void memset(char* start, char c, int num_bytes) {
	for (int i = 0; i < num_bytes; i++) {
		*(start + i) = c;
	}
}

int strlen(const char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

char *strcpy(char *dest, const char *src) {
    char *original_dest = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;
}

void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* itoa(int value) {
    static char buffer[12]; // enough for 32-bit int + sign + null
    int i = 0;
    int is_negative = 0;

    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    while (value != 0) {
        buffer[i++] = (value % 10) + '0';
        value /= 10;
    }

    if (is_negative) buffer[i++] = '-';

    buffer[i] = '\0';
    reverse(buffer, i);
    return buffer;
}


char* itoh(int value) {
    static char buffer[11]; // "0x" + 8 digits + null
    unsigned int num = (unsigned int)value;
    
    buffer[0] = '0';
    buffer[1] = 'x';
    for (int i = 0; i < 8; i++) {
        int shift = (7 - i) * 4;
        unsigned int nibble = (num >> shift) & 0xF;
        buffer[2 + i] = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
    }
    buffer[10] = '\0';
    return buffer;
}


void sleep(u32 ms) {
    u32 start = get_ticks();

    while (1) {
        __asm__ volatile("hlt");   // wait for next timer tick
        u32 now = get_ticks();
        if ((now - start) >= ms)
            break;
    }
}

static u32 rng_state = 1;

void srand(u32 seed) {
    if (seed == 0) seed = 1;  // avoid zero state
    rng_state = seed;
}

int rand() {
    u32 x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng_state = x;
    return x;
}

int randint(int l, int r) {
    int span = r - l + 1;
    u32 limit = 0xFFFFFFFF - (0xFFFFFFFF % span);  // avoid bias

    u32 x;
    do {
        x = rand();
    } while (x >= limit);

    return l + (x % span);
}

int roundf(float x) {
    int xi = (int)x;
    float diff = x - xi;

    if (diff >= 0.5f)
        return xi + 1;
    else if (diff <= -0.5f)
        return xi - 1;
    else
        return xi;
}

float ABS(float x) {
    return (x < 0.0f) ? -x : x;
}

float SIGN(float x) {
    if (x > 0.0f) return 1.0f;
    if (x < 0.0f) return -1.0f;
    return 0.0f;
}

