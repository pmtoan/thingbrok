#include "../modules/mysql/mysql_client.h"
#include "../modules/mysql/mysql_utils.h"

int main(int argc, char const *argv[])
{
    const MYSQL_INFO my_info_1 = {"127.0.0.1", 0, "root", "", ""};

    printf("Test create database: %s\n", mysql_create_database(my_info_1, "thingler"));

    const MYSQL_INFO my_info = {"127.0.0.1", 0, "root", "", "thingler"};

    printf("Test create table: %s\n", mysql_create_table(my_info, "thingler_test_table", "name,school,age"));

    printf("Test insert into table\n");
    for (int i = 0; i < 10; ++i)
    {
        printf("%s\n", mysql_insert(my_info, "thingler_test_table", "name=toan&school=uit&age=21"));
    }

    printf("Test select from table\n");
    MYSQL_RES* result = mysql_custom_query(my_info, "SELECT * from thingler_test_table");
    if (result == NULL)
        puts("Query error");
    else
        printf("%s\n", mysql_result_to_json_string(result));

    printf("Test drop table: %s\n", mysql_drop_table(my_info, "thingler_test_table"));

    printf("Test drop database: %s\n", mysql_drop_database(my_info, "thingler_test_table"));

    return 0;
}