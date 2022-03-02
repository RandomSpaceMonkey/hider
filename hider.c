#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define HEADER_LENGTH 8

int8_t HEADER_VALUES[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

void reverse_bytes(void *source, size_t size)
{
    for (int i = 0; i < size / 2; ++i)
    {
        u_int8_t tmp = ((u_int8_t *)source)[size - i - 1];
        ((u_int8_t *)source)[size - i - 1] = ((u_int8_t *)source)[i];
        ((u_int8_t *)source)[i] = tmp;
    }
}

int check_header(int8_t header[])
{
    for (int i = 0; i < HEADER_LENGTH; ++i)
    {
        if (header[i] != HEADER_VALUES[i])
        {
            printf("ERROR: Invalid file format\n");
            return -1;
        }

        return 0;
    }
}

int check_args(int argc, char **argv)
{

    char usg_string[] = "USAGE: ./hider [encode/decode] {filname.png}\n";

    if (argc == 1)
    {
        printf("%s", usg_string);
        return 1;
    }

    if (argc != 3)
    {
        printf("ERROR: Invalid amount of arguments supplied\n");
        printf("%s", usg_string);
        return 1;
    }

    if (!(strcmp("encode", argv[1]) || strcmp("decode", argv[1])))
    {
        printf("ERROR: Invalid arg: %s, must be encode or decode\n", argv[1]);
        printf("%s", usg_string);
        return 1;
    }

    return 0;
}

int encode(char *str, int size, FILE *png_file)
{

    fseek(png_file, 0, SEEK_END);
    u_int32_t legnth = size;
    reverse_bytes(&legnth, 4);
    if (fwrite(&legnth, 1, 4, png_file) != 4)
    {
        return 1;
    }
    char type[] = "text";
    if (fwrite(type, 1, 4, png_file) != 4)
    {
        return 1;
    }
    if (fwrite(str, 1, size, png_file) != size)
    {
        return 1;
    }

    return 0;
}

int decode(FILE *png_file)
{
    fseek(png_file, HEADER_LENGTH, SEEK_SET);
    while (!feof(png_file))
    {
        unsigned char type[4];
        unsigned int length;
        fread(&length, 1, 4, png_file);
        reverse_bytes(&length, 4); // the length of the file is in big endian
        fread(&type, 1, 4, png_file);
        if (strcmp(type, "text"))
        {
            fseek(png_file, length + 4, SEEK_CUR);
            continue;
        }
        char data[length];
        fread(data, 1, length, png_file);
        printf("decoded data: %s\n", data);
        return 0;
    }

    return 1;
}

int main(int argc, char **argv)
{

    if (check_args(argc, argv))
    {
        return 1;
    }

    FILE *png_file = fopen(argv[2], "rb+");
    if (png_file == NULL)
    {
        printf("ERROR: Cannot open %s\n", argv[1]);
        printf("%s\n", strerror(errno));
        return 1;
    }

    u_int8_t header[HEADER_LENGTH];
    int read_bytes = fread(header, 1, HEADER_LENGTH, png_file);
    if (read_bytes != HEADER_LENGTH)
    {
        printf("ERROR: Cannot read file header\n");
        return 1;
    }

    if (check_header(header))
    {
        return 1;
    }

    char text[] = "hello world"; // the text that is encoded into the file

    if (!strcmp(argv[1], "encode"))
    {
        if (encode(text, sizeof(text), png_file))
        {
            printf("ERROR: cannot write to %s\n", argv[2]);
            return 1;
        }
    }

    if (decode(png_file))
    {
        printf("ERROR: cannot decode file: %s\n", argv[2]);
        return 1;
    }
    return 0;

    fclose(png_file);
    return 0;
}