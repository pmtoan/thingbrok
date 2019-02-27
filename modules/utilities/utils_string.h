#ifndef __UTILITIES_STRINGS_H__
#define __UTILITIES_STRINGS_H__

#include "../core/types.h"

/*----------------------------------------------------------------*/
int string_count_cha(const char* string, char character);
int string_count_str(const char* string, const char* substr);
char* string_replace(const char* string, const char* substr, const char* replacement);
char* string_shift(const char* string, int byte);
char* string_cut(const char* string, char to);
char* string_extract(const char* string, char to);
char** string_split(const char* string, const char* delimiter, int times);
char* string_to_ascii(const char* string);
/*----------------------------------------------------------------*/

int string_count_cha(const char* string, char character)
{
    /*
     *	todo @string_count_cha count how many @character in @string
    */
    int count = 0;
    for (int i = 0; i < strlen(string); ++i)
        if (string[i] == character)
            count++;
    return count;
}

int string_count_str(const char* string, const char* substr)
{
    /*
     *   todo @string_count_str count how many @substr in @string
    */
    int count = 0;
    for(int i=0;string[i]!='\0';i++)
    {
        if (string[i] == substr[0])
        {
            int j = 0;
            for(j=0;substr[j]!='\0';j++)
            {
                if (string[i+j] != substr[j])
                    break;
            }
            if (substr[j] == '\0')
                count++;
        }
    }
    return count;
}

char* string_replace(const char* string, const char* substr, const char* replacement)
{
    /*
     *	todo @string_replace replace @substr with @replacement in @string
     *	usage
     *	    char* msg = string_replace("hello you", "you", "adam")
     *	    --->    msg = "hello adam"
    */
    char* tok = NULL;
    char* newstr = NULL;
    char* oldstr = NULL;
    int   oldstr_len = 0;
    int   substr_len = 0;
    int   replacement_len = 0;
    newstr = strdup(string);
    substr_len = strlen(substr);
    replacement_len = strlen(replacement);

    if (substr == NULL || replacement == NULL) {
        return newstr;
    }
    while ((tok = strstr(newstr, substr))) {
        oldstr = newstr;
        oldstr_len = strlen(oldstr);
        newstr = (char*)malloc(sizeof(char) * (oldstr_len - substr_len + replacement_len + 1));
        if (newstr == NULL) {
            free(oldstr);
            return NULL;
        }
        memcpy(newstr, oldstr, tok - oldstr);
        memcpy(newstr + (tok - oldstr), replacement, replacement_len);
        memcpy(newstr + (tok - oldstr) + replacement_len, tok + substr_len, oldstr_len - substr_len - (tok - oldstr));
        memset(newstr + oldstr_len - substr_len + replacement_len, 0, 1);
        free(oldstr);
    }
    return newstr;
}

char* string_shift(const char* string, int byte)
{
    /*
     *  todo @string_shift shift @string left @byte character
     *  usage
     *      char* ex = string_shift("hello", 2) --> "llo"
    */
    int len = strlen(string);
    if (len <= byte) return "";
    char* rt = (char*)malloc(len - byte + 1);
    int i = 0;
    int j = byte;
    while (string[j] != '\0')
    {
        rt[i] = string[j];
        i++; j++;
    }
    rt[i] = '\0';
    return rt;
}

char* string_extract(const char* string, char to)
{
    /*
     *  todo @string_extract get a copy string from beginning to @to in @string
     *  usage
     *      char* ex_string = string_extract("hello you", ' ')
     *      ---> return "hello"
     */
    char* buffer = (char*)malloc(strlen(string) + 1);
    int i = 0;
    while(string[i] != '\0' && string[i] != to)
    {
        buffer[i] = string[i];
        i++;
    }
    buffer[i] = '\0';
    char* rt = strdup(buffer);
    free(buffer);
    return rt;
}

char* string_cut(const char* string, char to)
{
    /*
    *  todo @string_cut get a copy string from @to to end of @string
    *  usage
    *      char* ex_string = string_cut("hello you", ' ')
    *      ---> return "you"
    */
    if (string_count_cha(string, to) < 1) return "";
    char* buffer = (char*)malloc(strlen(string) + 1);
    int i = 0;
    while(string[i] != to)
        i++;
    i++;
    int j = 0;
    while(string[i] != '\0')
    {
        buffer[j] = string[i];
        i++; j++;
    }
    buffer[j] = '\0';
    char* rt = strdup(buffer);
    free(buffer);
    return rt;
}

char** string_split(const char* string, const char* delimiter, int times)
{
    /*
     *	todo @string_split split a string into array of sub-string
     *	params @delimiter is string which string split at
     *	params @times is number split, it mean have times+1 substring in array return
     *  usage
     *      char** ex_string = string_split("hello adam hello alice hello john", "hello ", 2)
     *      --->    ex_string[0] = "adam "
     *              ex_string[1] = "alice "
     *              ex_string[2] = "hello john"
    */
    char* tmp = string_replace(string, delimiter, "\v");
    char** rt = (char**)malloc(sizeof(char*) * (times + 1));
    int i = 0;
    while(i < times)
    {
        rt[i] = string_extract(tmp, '\v');
        tmp = string_cut(tmp, '\v');
        i++;
    }
    rt[i] = strdup(string_replace(tmp, "\v", delimiter));
    return rt;
}

char* string_to_ascii(const char* string)
{
    /*
     *  todo @string_to_ascii convert a string to valid ASCII string
    */
    char* rt = string_replace(string, "%20", " ");
    rt = string_replace(rt, "%21", "!");
    rt = string_replace(rt, "%22", "\"");
    rt = string_replace(rt, "%23", "#");
    rt = string_replace(rt, "%24", "$");
    rt = string_replace(rt, "%25", "%");
    rt = string_replace(rt, "%26", "&");
    rt = string_replace(rt, "%27", "'");
    rt = string_replace(rt, "%28", "(");
    rt = string_replace(rt, "%29", ")");
    rt = string_replace(rt, "%2A", "*");
    rt = string_replace(rt, "%2B", "+");
    rt = string_replace(rt, "%2C", ",");
    rt = string_replace(rt, "%2D", "-");
    rt = string_replace(rt, "%2E", ".");
    rt = string_replace(rt, "%2F", "/");
    rt = string_replace(rt, "%30", "0");
    rt = string_replace(rt, "%31", "1");
    rt = string_replace(rt, "%32", "2");
    rt = string_replace(rt, "%33", "3");
    rt = string_replace(rt, "%34", "4");
    rt = string_replace(rt, "%35", "5");
    rt = string_replace(rt, "%36", "6");
    rt = string_replace(rt, "%37", "7");
    rt = string_replace(rt, "%38", "8");
    rt = string_replace(rt, "%39", "9");
    rt = string_replace(rt, "%3A", ":");
    rt = string_replace(rt, "%3B", ";");
    rt = string_replace(rt, "%3C", "<");
    rt = string_replace(rt, "%3D", "=");
    rt = string_replace(rt, "%3E", ">");
    rt = string_replace(rt, "%3F", "?");
    rt = string_replace(rt, "%40", "@");
    rt = string_replace(rt, "%41", "A");
    rt = string_replace(rt, "%42", "B");
    rt = string_replace(rt, "%43", "C");
    rt = string_replace(rt, "%44", "D");
    rt = string_replace(rt, "%45", "E");
    rt = string_replace(rt, "%46", "F");
    rt = string_replace(rt, "%47", "G");
    rt = string_replace(rt, "%48", "H");
    rt = string_replace(rt, "%49", "I");
    rt = string_replace(rt, "%4A", "J");
    rt = string_replace(rt, "%4B", "K");
    rt = string_replace(rt, "%4C", "L");
    rt = string_replace(rt, "%4D", "M");
    rt = string_replace(rt, "%4E", "N");
    rt = string_replace(rt, "%4F", "O");
    rt = string_replace(rt, "%50", "P");
    rt = string_replace(rt, "%51", "Q");
    rt = string_replace(rt, "%52", "R");
    rt = string_replace(rt, "%53", "S");
    rt = string_replace(rt, "%54", "T");
    rt = string_replace(rt, "%55", "U");
    rt = string_replace(rt, "%56", "V");
    rt = string_replace(rt, "%57", "W");
    rt = string_replace(rt, "%58", "X");
    rt = string_replace(rt, "%59", "Y");
    rt = string_replace(rt, "%5A", "Z");
    rt = string_replace(rt, "%5B", "[");
    rt = string_replace(rt, "%5C", "\\");
    rt = string_replace(rt, "%5D", "]");
    rt = string_replace(rt, "%5E", "^");
    rt = string_replace(rt, "%5F", "_");
    rt = string_replace(rt, "%60", "_");
    rt = string_replace(rt, "%61", "a");
    rt = string_replace(rt, "%62", "b");
    rt = string_replace(rt, "%63", "c");
    rt = string_replace(rt, "%64", "d");
    rt = string_replace(rt, "%65", "e");
    rt = string_replace(rt, "%66", "f");
    rt = string_replace(rt, "%67", "g");
    rt = string_replace(rt, "%68", "h");
    rt = string_replace(rt, "%69", "i");
    rt = string_replace(rt, "%6A", "j");
    rt = string_replace(rt, "%6B", "k");
    rt = string_replace(rt, "%6C", "l");
    rt = string_replace(rt, "%6D", "m");
    rt = string_replace(rt, "%6E", "n");
    rt = string_replace(rt, "%6F", "o");
    rt = string_replace(rt, "%70", "p");
    rt = string_replace(rt, "%71", "q");
    rt = string_replace(rt, "%72", "r");
    rt = string_replace(rt, "%73", "s");
    rt = string_replace(rt, "%74", "t");
    rt = string_replace(rt, "%75", "u");
    rt = string_replace(rt, "%76", "v");
    rt = string_replace(rt, "%77", "w");
    rt = string_replace(rt, "%78", "x");
    rt = string_replace(rt, "%79", "y");
    rt = string_replace(rt, "%7A", "z");
    rt = string_replace(rt, "%7B", "{");
    rt = string_replace(rt, "%7C", "|");
    rt = string_replace(rt, "%7D", "}");
    rt = string_replace(rt, "%7E", "~");
    return rt;
}


#endif // __UTILITIES_STRINGS_H__
