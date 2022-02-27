#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define HEADER_LENGTH 8

int8_t HEADER_VALUES[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

void reverse_bytes(void * source, size_t size) {
    for (int i = 0; i < size / 2; ++i) {
        u_int8_t tmp = ((u_int8_t *)source)[i];
        ((u_int8_t *)source)[size - i - 1] = ((u_int8_t *)source)[i];
        ((u_int8_t *)source)[i] = tmp;
    }
}

int check_header(int8_t header[]) {
    for (int i = 0; i < HEADER_LENGTH; ++i) {
        if (header[i] != HEADER_VALUES[i]) {
            printf("ERROR: Invalid file format\n");
            return -1;
        }

        return 0;
    }
}

int main(int argc, char** argv) {

    if (argc == 1) {
        printf("USAGE: ./hider {filname.png}\n");
        return 1;
    }

    FILE * png_file = fopen(argv[1], "rb");
    if (png_file == NULL) {
        printf("ERROR: Cannot open %s\n", argv[1]);
        printf("%s\n", strerror(errno));
        return 1;
    }

    u_int8_t header[HEADER_LENGTH];
    int read_bytes = fread(header, 1, HEADER_LENGTH, png_file);
    if (read_bytes != HEADER_LENGTH) {
        printf("Cannot read file header\n");
        return 1;
    }

    reverse_bytes(header, HEADER_LENGTH);

    if (check_header(header)) {
        return 1;
    }

    fclose(png_file);
    return 0;
}