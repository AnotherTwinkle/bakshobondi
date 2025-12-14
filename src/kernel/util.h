#ifndef UTIL_H
#define UTIL_H

typedef unsigned  char  u8;
typedef           char  s8; 
typedef unsigned short u16;
typedef          short s16;
typedef unsigned   int u32;
typedef            int s32;


#define min(a, b) ({ __auto_type _a = (a); __auto_type _b = (b); \
                     _a < _b ? _a : _b; })

#define max(a, b) ({ __auto_type _a = (a); __auto_type _b = (b); \
                     _a > _b ? _a : _b; })

#define randchoice(arr, n) ((arr)[randint(0, (n) - 1)])

#define RAND_MAX 32767
#define TRUE 1
#define FALSE 0

void memcpy(char* dest, char* src, int num_bytes);
void memset(char* start, char c, int num_bytes);

int strlen(const char *str);
char *strcpy(char *dest, const char *src);

void reverse(char str[], int length);

char *itoa(int value);
char *itoh(int value);

void sleep(u32 ms);

int rand();
void srand(u32 seed);
int randint(int l, int r);

int roundf(float x);

float ABS(float x);
float SIGN(float x);

#endif