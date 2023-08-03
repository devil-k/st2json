#include "base64.h"

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* base64_encode(const unsigned char* data, size_t input_length, size_t* output_length) {
    *output_length = 4 * ((input_length + 2) / 3); // Calculate the output length
    char* encoded_data = (char*)malloc(*output_length + 1); // Allocate memory for the encoded data
    if (encoded_data == NULL) {
        return NULL; // Memory allocation failed
    }

    size_t i, j;
    for (i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = base64_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 0 * 6) & 0x3F];
    }

    // Pad the encoded data with '=' characters if necessary
    if (input_length % 3 != 0) {
        for (i = 0; i < 3 - (input_length % 3); i++) {
            encoded_data[*output_length - 1 - i] = '=';
        }
    }

    encoded_data[*output_length] = '\0'; // Null-terminate the string
    return encoded_data;
}

int base64_decode_char(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    if (c == '=') return -1; // Padding character
    return -2; // Invalid character
}

unsigned char* base64_decode(const char* encoded_data, size_t input_length, size_t* output_length) {
    if (input_length % 4 != 0) {
        return NULL; // Invalid Base64 input length
    }

    *output_length = input_length / 4 * 3; // Calculate the output length
    if (encoded_data[input_length - 1] == '=') (*output_length)--;
    if (encoded_data[input_length - 2] == '=') (*output_length)--;

    unsigned char* decoded_data = (unsigned char*)malloc(*output_length);
    if (decoded_data == NULL) {
        return NULL; // Memory allocation failed
    }

    size_t i, j;
    for (i = 0, j = 0; i < input_length;) {
        int sextet_a = base64_decode_char(encoded_data[i++]);
        int sextet_b = base64_decode_char(encoded_data[i++]);
        int sextet_c = base64_decode_char(encoded_data[i++]);
        int sextet_d = base64_decode_char(encoded_data[i++]);

        if (sextet_a == -2 || sextet_b == -2 || sextet_c == -2 || sextet_d == -2) {
            free(decoded_data);
            return NULL; // Invalid Base64 character found
        }

        uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + sextet_d;

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = triple & 0xFF;
    }

    return decoded_data;
}

void test_base64_encoding() {
    const char* input_data = "Hello, World!"; // Input data to be encoded
    size_t input_length = strlen(input_data);
    size_t output_length;
    char* encoded_data = base64_encode((const unsigned char*)input_data, input_length, &output_length);

    if (encoded_data != NULL) {
        printf("Base64 encoded data: %s\n", encoded_data);
        free(encoded_data); // Remember to free the dynamically allocated memory
    } else {
        printf("Base64 encoding failed.\n");
    }
}

void test_base64_decoding() {
    const char* encoded_data = "SGVsbG8sIFdvcmxkIQ=="; // Input data to be decoded
    size_t input_length = strlen(encoded_data);
    size_t output_length;
    unsigned char* decoded_data = base64_decode(encoded_data, input_length, &output_length);

    if (decoded_data != NULL) {
        printf("Base64 decoded data: %s\n", decoded_data);
        free(decoded_data); // Remember to free the dynamically allocated memory
    } else {
        printf("Base64 decoding failed.\n");
    }
}
