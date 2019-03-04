/*
 *	@EXAMPLE usage:
 *		DB_QUERY query = db_init_instance("test_app", "test_topic");
 *		query.action = __DATABASE_ACTION_CREATE_APP__;
 *		db_execute_query(&query);
 *		printf("%s", query.status);
 *		db_free_instance(&query);
 */
#ifndef __DATABASE_BASE_H__
#define __DATABASE_BASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../unix/unix_x86_64_linux_dir.h"
#include "../unix/unix_x86_64_linux_file.h"
#include "../utilities/utils_time.h"
#include "../utilities/utils_file.h"
#include "../utilities/utils_string.h"
#include "../data_structure/list_string.h"
#include "../data_structure/list_pair.h"

#define __DATABASE_STORAGE_PATH__ 	"/var/lib/thingbrok/databases"

#define __DATABASE_ACTION_NONE__			0
#define __DATABASE_ACTION_CREATE_APP__		1
#define __DATABASE_ACTION_DROP_APP__		2
#define __DATABASE_ACTION_CREATE_TOPIC__	3
#define __DATABASE_ACTION_DROP_TOPIC__		4
#define __DATABASE_ACTION_INSERT__			5
#define __DATABASE_ACTION_SELECT__			6
#define __DATABASE_ACTION_DELETE__			7

#define __DATABASE_QUERY_OK__		1
#define __DATABASE_QUERY_FAILED__	0

typedef struct DATABASE_QUERY
{
	int action;
	char* app;	// application name
	char* topic;	// topic name
	char* data;	// data query

	/* for query result*/
	int status;	// query status: ok=1 or failed=0
	LIST_STRING result;	// query result if ok
}DB_QUERY;



/********************************************************************/
DB_QUERY db_init_instance(const char* app_name, const char* topic_name);
void db_free_instance(DB_QUERY* query);
void db_execute_query(DB_QUERY* query);
LIST_STRING db_list_application();
LIST_STRING db_list_topic(const char* app_name);
LIST_STRING db_list_record(const char* app_name, const char* topic);

int _db_do_create_application(const char* app_name);
int _db_do_drop_application(const char* app_name);
int _db_do_create_topic(const char* app_name, const  char* topic_name);
int _db_do_drop_topic(const char* app_name, const  char* topic_name);
int _db_do_insert(const char* app_name, const char* topic_name, const char* data_insert);
LIST_STRING _db_do_select(const char* app_name, const char* topic_name);
LIST_STRING _db_do_select_last(const char* app_name, const char* topic_name);
LIST_STRING _db_do_select_contain_string(const char* app_name, const char* topic_name, const char* str_contain);
/********************************************************************/

DB_QUERY db_init_instance(const char* app_name, const char* topic_name)
{
	/*
	 *	@todo: initialize a DB_QUERY structure with @app_name and @topic_name
	 *	return DB_QUERY structure initialized
	 */
	DB_QUERY query;
	query.action = __DATABASE_ACTION_NONE__;
	query.app = strdup(app_name);
	query.topic = strdup(topic_name);
	query.data = NULL;
	return query;
}

void db_free_instance(DB_QUERY* query)
{
	/*
	 *	@todo: release memory for a DB_QUERY structure
	 */
	if (query->app != NULL) free(query->app);
	if (query->topic != NULL) free(query->topic);
	if (query->data != NULL) free(query->data);
	if (query->result.len != 0)
		list_string_free(&query->result);
	return;
}

void db_execute_query(DB_QUERY* query)
{
	/*
	 *	@todo: execute query and store result in @query.result
	 */
	switch (query->action) {
		case __DATABASE_ACTION_CREATE_APP__: {
			query->status = _db_do_create_application(query->app);
			break;
		}
		case __DATABASE_ACTION_DROP_APP__: {
			// do drop application
			query->status = _db_do_drop_application(query->app);
			break;
		}
		case __DATABASE_ACTION_CREATE_TOPIC__: {
			// do create topic
			query->status = _db_do_create_topic(query->app, query->topic);
			break;
		}
		case __DATABASE_ACTION_DROP_TOPIC__: {
			// do drop topic
			query->status = _db_do_drop_topic(query->app, query->topic);
			break;
		}
		case __DATABASE_ACTION_INSERT__: {
			query->status = _db_do_insert(query->app, query->topic, query->data);
			break;
		}
		case __DATABASE_ACTION_SELECT__:{
			// do select
			query->result = _db_do_select(query->app, query->topic);
			query->status = __DATABASE_QUERY_OK__;
		}
			break;
		default:
			return;
	}
}

LIST_STRING db_list_application()
{
	/*
	 *	@todo: get list of application
	 */
	char* apps_string = unix_x86_64_linux_list_dir_in_dir(__DATABASE_STORAGE_PATH__);
	LIST_STRING apps = list_string_init();
	int num_app = string_count_cha(apps_string, '\n');
	char** apps_list = string_split(apps_string, "\n", num_app);
	for (int i = 0; i < num_app; ++i)
		list_string_append(&apps, apps_list[i]);
	return apps;
}

int _db_do_create_application(const char* app_name)
{
	int result;
	char* app_path = (char*)malloc(512);
	sprintf(app_path, "%s/%s", __DATABASE_STORAGE_PATH__, app_name);
	int rs = unix_x86_64_linux_mkdir(app_path);
	if (rs != -1)
		result = __DATABASE_QUERY_OK__;
	else
		result = __DATABASE_QUERY_FAILED__;
	free(app_path);
	return result;
}

int _db_do_drop_application(const char* app_name)
{
	int result;
	char* app_path = (char*)malloc(512);
	sprintf(app_path, "%s/%s", __DATABASE_STORAGE_PATH__, app_name);
	int rs = unix_x86_64_linux_rmdir(app_path);
	if (rs != -1)
		result = __DATABASE_QUERY_OK__;
	else
		result = __DATABASE_QUERY_FAILED__;
	free(app_path);
	return result;
}

int _db_do_create_topic(const char* app_name, const  char* topic_name)
{
	int result;
	char* topic_path = (char*)malloc(512);
	sprintf(topic_path, "%s/%s/%s", __DATABASE_STORAGE_PATH__, app_name, topic_name);
	int rs = unix_x86_64_linux_mkdir(topic_path);
	if (rs != -1)
		result = __DATABASE_QUERY_OK__;
	else
		result = __DATABASE_QUERY_FAILED__;
	free(topic_path);
	return result;
}

int _db_do_drop_topic(const char* app_name, const  char* topic_name)
{
	int result;
	char* topic_path = (char*)malloc(512);
	sprintf(topic_path, "%s/%s/%s", __DATABASE_STORAGE_PATH__, app_name, topic_name);
	int rs = unix_x86_64_linux_rmdir(topic_path);
	if (rs != -1)
		result = __DATABASE_QUERY_OK__;
	else
		result = __DATABASE_QUERY_FAILED__;
	free(topic_path);
	return result;
}

int _db_do_insert(const char* app_name, const char* topic_name, const char* data_insert)
{
	/*
	 *	@todo: insert data (in X-WWW-form-urlencoded format) to a topic in application
	 */
	int result;
	char* topic_path = (char*)malloc(512);
	sprintf(topic_path, "%s/%s/%s", __DATABASE_STORAGE_PATH__, app_name, topic_name);
	if (unix_x86_64_linux_get_stat_type(topic_path) != 0)
		result = __DATABASE_QUERY_FAILED__;
	else
	{
		char* data_path = (char*)malloc(512);
		sprintf(data_path, "%s/%s.data", topic_path, topic_name);
		file_append_line(data_path, data_insert);
		file_append_line(data_path, "\n");
		free(data_path);
		result = __DATABASE_QUERY_OK__;
	}
	free(topic_path);
	return result;
}

LIST_STRING _db_do_select(const char* app_name, const char* topic_name)
{
	/*
	 *	@todo: select data from a topic of an application
	 */
	LIST_STRING rows = list_string_init();

	char* f_path = (char*)malloc(512);
	sprintf(f_path, "%s/%s/%s/%s.data", __DATABASE_STORAGE_PATH__, app_name, topic_name, topic_name);

	if (unix_x86_64_linux_get_stat_type(f_path) == -1)
		return rows;
	char* buffer = (char*)malloc(1024);
	FILE* f_pointer = fopen(f_path, "r");
	while(fgets(buffer, 1024, f_pointer))
		list_string_append(&rows, string_replace(buffer, "\n", ""));

	free(buffer);
	free(f_path);
	fclose(f_pointer);
	return rows;
}

LIST_STRING _db_do_select_last(const char* app_name, const char* topic_name)
{
	/*
	*	@todo: query last 10 rows data
	*/
	LIST_STRING rows = list_string_init();
	char* f_path = (char*)malloc(512);
	sprintf(f_path, "%s/%s/%s/%s.data", __DATABASE_STORAGE_PATH__, app_name, topic_name, topic_name);
	if (unix_x86_64_linux_get_stat_type(f_path) == -1)
		return rows;
	return file_read_text_last(f_path, 10);
	free(f_path);
	return rows;
}

LIST_STRING _db_do_select_contain_string(const char* app_name, const char* topic_name, const char* str_contain)
{
	/*
	*	@todo: query data from database and return record contain @str_contain
	*/
	LIST_STRING rows = list_string_init();

	char* f_path = (char*)malloc(512);
	sprintf(f_path, "%s/%s/%s/%s.data", __DATABASE_STORAGE_PATH__, app_name, topic_name, topic_name);

	FILE* fp = fopen(f_path, "r");
	char* buffer = (char*)malloc(__SIZE_LARGE__);
	while(fgets(buffer, __SIZE_LARGE__, fp))
		if (strstr(buffer, str_contain))
			list_string_append(&rows, string_replace(buffer, "\n", ""));

	fclose(fp);
	if (buffer != NULL) free(buffer);
	free(f_path);
	return rows;
}

#endif // __DATABASE_BASE_H__