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

char* hexstr(int num) {
    static char result[11];   // "0x" + 8 hex digits + '\0'
    result[0] = '0';
    result[1] = 'x';

    for (int i = 0; i < 8; i++) {
        int shift = (7 - i) * 4;
        int nibble = (num >> shift) & 0xF;

        result[2 + i] = (nibble < 10) ? ('0' + nibble)
                                      : ('A' + nibble - 10);
    }

    result[10] = '\0';
    return result;
}

void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

}
