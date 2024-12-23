#include <stdio.h>

typedef struct { unsigned char string[16]; } hash_MD5;

int MD5_sum(const char* adresse, hash_MD5* sum);

int MP3_MD5_sum(const wchar_t* adresse, hash_MD5* sum);