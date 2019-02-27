#ifndef __UTILITIES_FILE_H__
#define __UTILITIES_FILE_H__

#include "../core/types.h"
#include "../data_structure/bytes.h"
#include "utils_string.h"

/*----------------------------------------------------------------*/
unsigned long file_sizeof(const char* path);
char* file_read_text(const char* path);
void file_write_text(const char* path, const char* buffer);
BYTES file_read_bin(const char* path);
void file_write_bin(const char* path, BYTES buffer);
char* file_get_extension(const char* path);
void file_append_line(const char* path, const char* line);
char* file_read_first_line(const char* path);
/*----------------------------------------------------------------*/

unsigned long file_sizeof(const char* path)
{
    /*
     *  todo @file_sizeof get size of @path
     *  if @path is not a file, return -1
    */
    FILE* fp = fopen(path, "r");
    if (fp == NULL) return -1;
    fseek(fp, 0L, SEEK_END);
    unsigned long fs = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    fclose(fp);
    return fs;
}

char* file_read_text(const char* path)
{
    /*
     *  todo @file_read_text read all text from @path
     *  if @path is not found, return NULL
    */
    FILE* fp = fopen(path, "r");
    if (fp == NULL) return NULL;
    int fs = file_sizeof(path);
    char* buffer = (char*)malloc(fs + 1);
    fread(buffer, fs, 1, fp);
    buffer[fs] = '\0';
    fclose(fp);
    return buffer;
}

void file_write_text(const char* path, const char* buffer)
{
    /*
    *  todo @file_write_text write @buffer to @path
    */
    FILE* fp = fopen(path, "w");
    if (fp == NULL) return;
    fwrite(buffer, strlen(buffer), 1, fp);
    fclose(fp);
}

BYTES file_read_bin(const char* path)
{
    BYTES bytes = bytes_init();
    bytes_read_file(&bytes, path);
    return bytes;
}

void file_write_bin(const char* path, BYTES buffer)
{
    bytes_write_file(buffer, path);
    return;
}

char* file_get_extension(const char* path)
{
    /*
     *  todo @file_get_extension extension of a file
     *  usage:
     *      char* file_get_extension("hello.txt") -> txt
    */
    if (string_count_cha(path, '.') == 0) return "";
    char* ext = strdup(path);
    while(string_count_cha(ext, '.') > 0)
        ext = string_cut(ext, '.');
    return ext;
}

void file_append_line(const char* path, const char* line)
{
    /*
     *  @todo: applend a line text to file
     */
    char* buffer = (char*)malloc(file_sizeof(path) + strlen(line) + 1);
    char* content = file_read_text(path);
    if (content == NULL)
        file_write_text(path, line);
    else
    {
        sprintf(buffer, "%s%s", content, line);
        file_write_text(path, buffer);
        free(content);
    }
    free(buffer);
    return;
}

char* file_read_first_line(const char* path)
{
    FILE* fp = fopen(path, "r");
    char* buffer = (char*)malloc(1024);
    fgets(buffer, 1024, fp);
    fclose(fp);
    return string_replace(buffer, "\n", "");
}

#endif // __UTILITIES_FILE_H__