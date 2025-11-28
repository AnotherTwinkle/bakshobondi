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