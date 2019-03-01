#include "../modules/database/database.h"

int main(int argc, char** argv)
{
	// DB_QUERY query = db_init_instance("test_app", "test_topic");

	/*// Test case for create application
	query.action = __DATABASE_ACTION_CREATE_APP__;
	db_execute_query(&query);
	printf("%d\n", query.status);*/

	/*// Test case for create topic
	query.action = __DATABASE_ACTION_CREATE_TOPIC__;
	db_execute_query(&query);
	printf("%d\n", query.status);*/

	/*// Test case for drop topic
	query.action = __DATABASE_ACTION_DROP_TOPIC__;
	db_execute_query(&query);
	printf("%d\n", query.status);*/

	/*// Test case for drop application
	query.action = __DATABASE_ACTION_DROP_APP__;
	db_execute_query(&query);
	printf("%d\n", query.status);*/

	// Test case for insert data
	/*query.action = __DATABASE_ACTION_INSERT__;
	query.data = strdup("name=toan&age=21&shcool=uit");
	db_execute_query(&query);
	printf("%d\n", query.status);*/

	/*// Test case for list all application
	printf("%s\n", list_string_compose_list_string(db_list_application()));*/

	// Test case for select data
	// list_string_print(_db_do_select("test_app", "test_topic"));
	list_string_print(_db_do_select_last("test_app", "test_topic"));

	// db_free_instance(&query);
	return 0;
}