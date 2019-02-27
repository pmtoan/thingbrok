#ifndef __MYSQL_CLIENT_CONNECTOR_H__
#define __MYSQL_CLIENT_CONNECTOR_H__

/*
*	Compile with flag: -lmysqlclient
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../core/types.h"
#include "../data_structure/list_string.h"
#include "../data_structure/list_pair.h"
#include "../utilities/utils_string.h"
#include "../utilities/utils_time.h"

#define __MYSQL_DEFAULT_DATA_TYPE__ 	"text"
#define __MYSQL_DEFAULT_CHARACTER_SET__ "utf8"
#define __MYSQL_DEFAULT_COLLATE			"utf8_unicode_ci"

typedef struct MYSQL_INFO
{
	char* server;	/* mysql server ip */
	int port;		/* mysql port number, using 0 for default port */
	char* username;
	char* password;
	char* database;
}MYSQL_INFO;

/*
*	Usage
*		const MYSQL_INFO my_server = {"127.0.0.1", 0, "root", "password", "database_name"};
*/

/*********************************************************************/
char* mysql_create_database(MYSQL_INFO my_info, const char* db_name);
char* mysql_drop_database(MYSQL_INFO my_info, const char* db_name);
char* mysql_create_table(MYSQL_INFO my_info, const char* table_name, const char* columns);
char* mysql_drop_table(MYSQL_INFO my_info, const char* table_name);
char* mysql_insert(MYSQL_INFO my_info, const char* table_name, const char* data);
MYSQL_RES* mysql_custom_query(MYSQL_INFO my_info, const char* query);

char* _mysql_do_query(MYSQL_INFO my_info, const char* query);
/*********************************************************************/



char* _mysql_do_query(MYSQL_INFO my_info, const char* query)
{
	MYSQL *mysql = NULL;
	mysql = mysql_init(mysql);
	if (!mysql) {
		return "Unexpected error";
	}

	if (!mysql_real_connect(
			mysql,          	/* MYSQL structure to use */
			my_info.server,     /* server hostname or IP address */
			my_info.username,	/* mysql user */
			my_info.password,	/* password */
			my_info.database,	/* default database to use, NULL for none */
			my_info.port,		/* port number, 0 for default */
			NULL,				/* socket file or named pipe name */
			CLIENT_FOUND_ROWS	/* connection flags */ ))
	{
		mysql_close(mysql);
		return "Connect failed";
	}

	else
	{
		if (mysql_real_query(mysql, query, strlen(query))) {
			mysql_close(mysql);
			return "Query failed";
		} else {
			mysql_close(mysql);
			return "Query OK";
		}
	}
}

char* mysql_create_database(MYSQL_INFO my_info, const char* db_name)
{
	/*
	*	@todo: create a database
	*	@param my_info: MySQL server information
	*	@param db_name: database name which created
	*/
	if (string_count_cha(db_name, ' ') > 0)
		return "Database name not valid";

	char* query = (char*)malloc(__SIZE_LARGE__);
	sprintf(
			query,
			"CREATE DATABASE %s CHARACTER SET %s COLLATE %s",
			db_name,
			__MYSQL_DEFAULT_CHARACTER_SET__,
			__MYSQL_DEFAULT_COLLATE
			);

  	return _mysql_do_query(my_info, query);

}

char* mysql_drop_database(MYSQL_INFO my_info, const char* db_name)
{
	/*
	*	@todo: drop a database
	*	@param my_info: MySQL server information
	*	@param db_name: database name which dropped
	*/
	if (string_count_cha(db_name, ' ') > 0)
		return "Database name not valid";

	if (string_count_cha(db_name, ' ') > 0)
		return "Database name not valid";

	char* query = (char*)malloc(__SIZE_LARGE__);
	sprintf(query, "DROP DATABASE %s", db_name);

	return _mysql_do_query(my_info, query);
}

char* mysql_create_table(MYSQL_INFO my_info, const char* table_name, const char* list_columns)
{
	/*
 	*	@todo: create a table
 	*	@param my_info: MySQL server information
 	*	@param table_name: table name which created
 	*	@columns: list of table columns name ,each column separate by a comma (,)
	*/
	if (string_count_cha(table_name, ' ') > 0)
		return "Table name not valid";

	int number_of_column = string_count_cha(list_columns, ',');
	char** columns_string = string_split(list_columns, ",", number_of_column);
	LIST_STRING columns = list_string_init();

	for (int j = 0; j <= number_of_column; ++j)
	{
		list_string_append(&columns, columns_string[j]);
		free(columns_string[j]);
	}
	free(columns_string);

	char* query = (char*)malloc(__SIZE_LARGE__);
	sprintf(query, "CREATE TABLE %s(", table_name);
	for (int i = 0; i < columns.len; ++i)
	{
		if (i != columns.len - 1)
			sprintf(query, "%s%s %s,", query, columns.head->value, __MYSQL_DEFAULT_DATA_TYPE__);
		else
			sprintf(query, "%s%s %s)", query, columns.head->value, __MYSQL_DEFAULT_DATA_TYPE__);
		columns.head = columns.head->next;
	}

	return _mysql_do_query(my_info, query);
}

char* mysql_drop_table(MYSQL_INFO my_info, const char* table_name)
{
	/*
 	*	@todo: drop a table
 	*	@param my_info: MySQL server information
 	*	@param table_name: table name which dropped
	*/
	if (string_count_cha(table_name, ' ') > 0)
		return "Table name not valid";

	char* query = (char*)malloc(__SIZE_LARGE__);
	sprintf(query, "DROP TABLE %s", table_name);

	return _mysql_do_query(my_info, query);

}

char* mysql_insert(MYSQL_INFO my_info, const char* table_name, const char* data)
{
	/*
	 *	@todo: insert data into a table
	 * 	@param my_info: MySQL server information
	 * 	@param table_name: table name which data inserted to
	 * 	@param data: data inserted into table in x-www-form-urlencoded format
	*/
	if (string_count_cha(table_name, ' ') > 0)
		return "Table name not valid";

	LIST_PAIR values = list_pair_parse_x_www_form_urlencoded(data);
	char* query = (char*)malloc(__SIZE_LARGE__);
	sprintf(query, "INSERT INTO %s(", table_name);

	LIST_PAIR buffer = values;
	for (int i = 0; i < buffer.len; ++i)
	{
		if (i != buffer.len - 1)
			sprintf(query, "%s%s,", query, buffer.head->key);
		else
			sprintf(query, "%s%s)", query, buffer.head->key);
		buffer.head = buffer.head->next;
	}

	sprintf(query, "%s VALUES(", query);
	buffer = values;
	for (int i = 0; i < buffer.len; ++i)
	{
		if (i != buffer.len - 1)
			sprintf(query, "%s\"%s\",", query, buffer.head->value);
		else
			sprintf(query, "%s\"%s\")", query, buffer.head->value);
		buffer.head = buffer.head->next;
	}
	list_pair_free(&values);
	return _mysql_do_query(my_info, query);
}

MYSQL_RES* mysql_custom_query(MYSQL_INFO my_info, const char* query)
{
	/*
	 *	@todo execute a custom SQL statement, use for select statement
	 *	@return MYSQL_RES* structure
	*/
	MYSQL *mysql = NULL;
	mysql = mysql_init(mysql);
	if (!mysql) {
		return NULL;
	}

	if (!mysql_real_connect(
			mysql,          	/* MYSQL structure to use */
			my_info.server,     /* server hostname or IP address */
			my_info.username,	/* mysql user */
			my_info.password,	/* password */
			my_info.database,	/* default database to use, NULL for none */
			my_info.port,		/* port number, 0 for default */
			NULL,				/* socket file or named pipe name */
			CLIENT_FOUND_ROWS	/* connection flags */ ))
	{
		mysql_close(mysql);
		return NULL;
	}

	else
	{
		mysql_query(mysql, query);
		return mysql_store_result(mysql);
	}
}

#endif // __MYSQL_CLIENT_CONNECTOR_H__