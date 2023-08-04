#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int      uint32_t;

char* base64_encode(const unsigned char* data, size_t input_length, size_t* output_length);
unsigned char* base64_decode(const char* encoded_data, size_t input_length, size_t* output_length);

void test_base64_encoding();
void test_base64_decoding();