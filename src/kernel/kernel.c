#include "../drivers/screen.h"
void main() {
	SCREEN_INIT();
	char *msg = "Hello, world!\n";

	for(int i = 0; i < 50; i++) {
		print_char('0' + i, -1, -1, 0);
		print(msg);
	}

}

