#ifndef __UTILITIES_FILE_H__
#define __UTILITIES_FILE_H__

#include "../core/types.h"
#include "../data_structure/list_string.h"
#include "utils_string.h"

/*----------------------------------------------------------------*/
unsigned long file_sizeof(const char* path);
unsigned long file_count_line(const char* path);
char* file_read_text(const char* path);
void file_write_text(const char* path, const char* buffer);
char* file_get_extension(const char* path);
void file_append_line(const char* path, const char* line);
char* file_read_first_line(const char* path);
LIST_STRING file_read_text_last(const char* path, int lines);
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

LIST_STRING file_read_text_last(const char* path, int lines)
{
    /*
    *   @todo read last @lines from @path
    */
    unsigned long num_line = file_count_line(path);
    if (num_line <= lines)
        return list_string_parse(file_read_text(path), "\n");
    else
    {
        LIST_STRING list = list_string_init();
        FILE* fp = fopen(path, "r");
        char* buffer = (char*)malloc(__SIZE_LARGE__);
        int i = 0;
        for (i; i < num_line - lines; ++i)
            fgets(buffer, __SIZE_LARGE__, fp);
        for (i; i < num_line; ++i)
        {
            fgets(buffer, __SIZE_LARGE__, fp);
            list_string_append(&list, string_replace(buffer, "\n", ""));
        }
        if (buffer != NULL) free(buffer);
        return list;
    }
}

unsigned long file_count_line(const char* path)
{
    /*
    *   @todo: count how many line in file?
    */
    unsigned long lines = 0;
    FILE* fp = fopen(path, "r");
    char* buffer = (char*)malloc(__SIZE_LARGE__);
    while(fgets(buffer, __SIZE_LARGE__, fp))
        lines += 1;
    if (buffer != NULL) free(buffer);
    fclose(fp);
    return lines;
}

#endif // __UTILITIES_FILE_H__