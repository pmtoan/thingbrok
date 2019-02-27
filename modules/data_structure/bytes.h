#ifndef __DATA_STRUCTURE_BYTES_H__
#define __DATA_STRUCTURE_BYTES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../core/types.h"

/* --- bytes data structure --- */
typedef struct BYTES
{
    int len;    // length of array
    char* buffer; // bytes buffer
}BYTES;

/*----------------------------------------------------------------*/
BYTES bytes_init();
void bytes_free(BYTES* bytes);
void bytes_print(BYTES bytes);
void bytes_print_d(BYTES bytes);
void bytes_print_x(BYTES bytes);
int bytes_read_file(BYTES* buffer, const char* path);
int bytes_write_file(BYTES buffer, const char* path);
/*----------------------------------------------------------------*/

BYTES bytes_init()
{
    BYTES bytes;
    bytes.len = 0;
    bytes.buffer = NULL;
    return bytes;
}

void bytes_free(BYTES* bytes)
{
    free(bytes->buffer);
}

void bytes_print(BYTES bytes)
{
    bytes_print_x(bytes);
}

void bytes_print_d(BYTES bytes)
{
    for (int i = 0; i < bytes.len; ++i)
    {
        if((i != 0) && (i % 10 == 0))
            printf("\n");
        printf("%03d ", bytes.buffer[i]);
    }
    printf("\n");
}

void bytes_print_x(BYTES bytes)
{
    for (int i = 0; i < bytes.len; ++i)
    {
        if((i != 0) && (i % 10 == 0))
            printf("\n");
        printf("%02x ", bytes.buffer[i]);
    }
    printf("\n");
}

int bytes_read_file(BYTES* buffer, const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (fp == 0) return 0;
    fseek(fp, 0L, SEEK_END);
    int fs = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    buffer->len = fs;
    buffer->buffer = (char*)malloc(fs);
    fread(buffer->buffer, fs, 1, fp);
    fclose(fp);
    return fs;
}

int bytes_write_file(BYTES buffer, const char* path)
{
    FILE* fp = fopen(path, "wb");
    if (fp == 0) return 0;
    fwrite(buffer.buffer, buffer.len, 1, fp);
    fclose(fp);
    return buffer.len;
}

#endif // __DATA_STRUCTURE_BYTES_H__