#ifndef __DATA_STRUCTURE_LIST_STRING_H__
#define __DATA_STRUCTURE_LIST_STRING_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../core/types.h"
#include "../utilities/utils_string.h"

/* --- string list data structure --- */
typedef struct STRING
{
    int len;
    char* value;
    struct STRING* next;
}STRING;

typedef struct LIST_STRING
{
    /*
    *  @LIST_STRING a single link list of STRING
    */
    int len;
    STRING* head;
}LIST_STRING;

/*----------------------------------------------------------------*/
LIST_STRING list_string_init();
void list_string_free(LIST_STRING* list);
void list_string_print(LIST_STRING list);
void list_string_append(LIST_STRING* list, const char* value);
char* list_string_compose_list_string(LIST_STRING list);
LIST_STRING list_string_parse(const char* string, const char* delimiter);
/*----------------------------------------------------------------*/

LIST_STRING list_string_init()
{
    /*
	 *	todo @list_string_init() initialize a list of string holder
	 *	usage LIST_STRING list = list_string_init()
	*/
    LIST_STRING list;
    list.len = 0;
    list.head = NULL;
    return list;
}

void list_string_free(LIST_STRING* list)
{
    /*
	 *	todo @list_string_free release memory of @list
	 *	usage
	 *		LIST_STRING list = list_string_init()
	 *		...
	 *		list_string_free(&list)
	*/
    while(list->head != NULL)
    {
        STRING* i = list->head;
        list->head = list->head->next;
        free(i->value);
        i->next = NULL;
        free(i);
    }
}

void list_string_print(LIST_STRING list)
{
    /*
	*	todo @list_string_print print @list
	*/
    int i = 0;
    while(i < list.len)
    {
        _("%s\n", list.head->value);
        i++;
        list.head = list.head->next;
    }
}

void list_string_append(LIST_STRING* list, const char* value)
{
    /*
	 *	todo @list_string_append append a @STRING to @list
	 *	usage
	 *		LIST_STRING list = list_string_init()
	 *		list_string_append(&list, "hello name");
	 *		...
	 *		list_string_free(&list)
	*/
    if (list->len == 0)
    {
        list->head = (STRING*)malloc(sizeof(STRING*));
        list->head->value = strdup(value);
        list->head->len = strlen(value);
        list->head->next = NULL;
    }
    else
    {
        STRING* i = list->head;
        while(i->next != NULL)
            i = i->next;
        STRING* next = (STRING*)malloc(sizeof(STRING*));
        next->value = strdup(value);
        next->len = strlen(value);
        next->next = NULL;
        i->next = next;
    }
    list->len++;
}

char* list_string_compose_list_string(LIST_STRING list)
{
    /*
	 *	todo @list_string_compose_list_string compose a LIST_STRING to list string
	 *	list string: ["hello","you"]
	 *	usage
	 *		LIST_STRING list = list_string_init()
	 *		list_string_append(&list, "hello");
	 *		list_string_append(&list, "you");
	 *		char* list_string = list_string_compose_list_string(list)
	 *		...
	 *		list_string_free(&list)
	*/
    char* buffer = (char*)malloc(__SIZE_EXTRA__);
    sprintf(buffer, "[");
    for (int i = 0; i < list.len; ++i)
    {
        if (i==0)
            sprintf(buffer, "%s\"%s\"", buffer, list.head->value);
        else
            sprintf(buffer, "%s,\"%s\"", buffer, list.head->value);
        list.head = list.head->next;
    }
    sprintf(buffer, "%s]", buffer);
    char* list_string = strdup(buffer);
    free(buffer);
    return list_string;
}

LIST_STRING list_string_parse(const char* string, const char* delimiter)
{
    LIST_STRING list_string = list_string_init();
    int num_element = string_count_str(string, delimiter);
    char** string_after_split = string_split(string, delimiter, num_element);
    for (int i = 0; i <= num_element; ++i)
    {
        list_string_append(&list_string, string_after_split[i]);
    }
    return list_string;
}

#endif // __DATA_STRUCTURE_LIST_STRING_H__