void memcpy(char* src, char* dst, int num_bytes) {
	for (int i = 0; i < num_bytes; i++) {
		*(dst + i) = *(src + i);
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

char *itoa(int value, char *buffer) {
    int i = 0;
    int is_negative = 0;

    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }

    if (value < 0) {
        is_negative = 1;
        // Handle the case for INT_MIN (-2147483648) which cannot be
        // directly negated by making it positive on some architectures.
        value = -value; 
    }

    while (value != 0) {
        int remainder = value % 10;
        buffer[i++] = remainder + '0';
        value = value / 10;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    reverse(buffer, i);

    return buffer;
}

char *itoh(int value, char *buffer) {
    // Cast to unsigned int to ensure correct bitwise shifting behavior for 32-bit signed integers.
    unsigned int num = (unsigned int)value;
    
    // "0x" prefix
    buffer[0] = '0';
    buffer[1] = 'x';

    // Loop for 8 hex digits (32 bits), processing 4 bits (1 nibble) at a time
    for (int i = 0; i < 8; i++) {
        // Calculate the shift amount to target the next nibble from the MSB (7*4=28) down to LSB (0*4=0).
        int shift = (7 - i) * 4; 
        unsigned int nibble = (num >> shift) & 0xF;

        char hex_char;
        if (nibble < 10) {
            // Digits 0-9
            hex_char = '0' + nibble;
        } else {
            // Letters A-F (A=10, B=11, ..., F=15)
            hex_char = 'A' + nibble - 10; 
        }
        
        // Write the character to the buffer, starting at index 2 (after "0x")
        buffer[2 + i] = hex_char;
    }

    // Null terminator after 0x + 8 digits = index 10
    buffer[10] = '\0'; 
    return buffer;
}