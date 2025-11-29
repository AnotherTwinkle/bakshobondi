typedef unsigned  char  u8;
typedef           char  s8; 
typedef unsigned short u16;
typedef          short s16;
typedef unsigned   int u32;
typedef            int s32;

void memcpy(char* src, char* dst, int num_bytes);
void memset(char* start, char c, int num_bytes);

int strlen(const char *str);
char *strcpy(char *dest, const char *src);

void reverse(char str[], int length);

char *itoa(int value, char *buffer);
char *itoh(int value, char *buffer);

