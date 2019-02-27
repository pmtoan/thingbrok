#ifndef __MYSQL_UTILITIES_H__
#define __MYSQL_UTILITIES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../core/types.h"
#include "../data_structure/list_string.h"
#include "../data_structure/list_pair.h"
#include "../utilities/utils_string.h"
#include "../utilities/utils_time.h"

char* mysql_result_to_json_string(MYSQL_RES* result)
{
    char* json = (char*)malloc(__SIZE_EXTRA__);
    sprintf(json, "{");

    int total_rows = mysql_num_rows(result);
    int num_fields = mysql_num_fields(result);

    if (num_fields > 0)
    {
        MYSQL_FIELD *field;
        LIST_STRING headers = list_string_init();
        while(field = mysql_fetch_field(result))
        {
            list_string_append(&headers, field->name);
        }
        sprintf(json, "%s\"header\":%s", json, list_string_compose_list_string(headers));
        list_string_free(&headers);
    }

    LIST_STRING full_data = list_string_init();
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {
        LIST_STRING row_data = list_string_init();
        for(int i = 0; i < num_fields; i++)
            list_string_append(&row_data, row[i]);
        list_string_append(&full_data, list_string_compose_list_string(row_data));
        list_string_free(&row_data);
    }
    sprintf(
        json,
        "%s,\"body\":%s",
        json,
        string_replace(
            string_replace(
                list_string_compose_list_string(full_data),
                "\"[",
                "["
            ),
            "]\"",
            "]"
        )
    );
    list_string_free(&full_data);

    LIST_PAIR footers = list_pair_init();
    char* rows = (char*)malloc(__SIZE_TINY__);
    char* columns = (char*)malloc(__SIZE_TINY__);
    sprintf(rows, "%d", total_rows);
    sprintf(columns, "%d", num_fields);
    list_pair_append(&footers, "rows", rows);
    list_pair_append(&footers, "columns", columns);
    sprintf(json,"%s,\"footer\":%s", json, list_pair_compose(footers));
    list_pair_free(&footers);
    free(rows);
    free(columns);

    sprintf(json, "%s}", json);
    return json;
}


#endif // __MYSQL_UTILITIES_H__