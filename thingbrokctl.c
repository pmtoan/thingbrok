#include "controller/thingbrok/thingbrok_controller.h"

#define __DEF_APP_NAME__	"thingbrokctl"
#define __DEF_VERSION__		"version 1.0"
#define __DEF_IP__			"127.0.0.1"
#define __DEF_PORT__		8000
#define __DEF_KEY__			"abc"

void print_help();

int main(int argc, char const *argv[])
{
	char* ip = __DEF_IP__;
	int port = __DEF_PORT__;
	char* method = "NULL";
	char* action = "NULL";
	char* key = __DEF_KEY__;
	char* app = NULL;
	char* topic = NULL;
	char* data = NULL;

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "-h") == 0) ip = strdup(argv[i+1]);
		else if (strcmp(argv[i], "-p") == 0) port = atoi(argv[i+1]);
		else if (strcmp(argv[i], "--push") == 0)
		{
			action = strdup("push");
			method = strdup("POST");
		}
		else if (strcmp(argv[i], "--pull") == 0)
		{
			action = strdup("pull");
			method = strdup("GET");
		}
		else if (strcmp(argv[i], "--app") == 0)
		{
			action = strdup("app");
			method = strdup("GET");
		}
		else if (strcmp(argv[i], "--topic") == 0)
		{
			action = strdup("topic");
			method = strdup("GET");
		}
		else if (strcmp(argv[i], "-k") == 0) key = strdup(argv[i+1]);
		else if (strcmp(argv[i], "-d") == 0) data = strdup(argv[i+1]);
		else if (strcmp(argv[i], "-a") == 0) app = strdup(argv[i+1]);
		else if (strcmp(argv[i], "-t") == 0) topic = strdup(argv[i+1]);
	}

	if (strcmp(action, "push") == 0)
	{
		if (app != NULL && topic != NULL && data != NULL)
		{
			controller_control_ctl(ip, port, method, action, key, app, topic, data);
			return 0;
		}
	}

	else if (strcmp(action, "pull") == 0)
	{
		if (app != NULL && topic != NULL)
		{
			controller_control_ctl(ip, port, method, action, key, app, topic, "");
			return 0;
		}
	}

	else if (strcmp(action, "app") == 0)
	{
		if (app != NULL)
		{
			controller_control_ctl(ip, port, method, action, key, app, topic, "");
			return 0;
		}
	}

	else if (strcmp(action, "topic") == 0)
	{
		if (app != NULL && topic != NULL)
		{
			controller_control_ctl(ip, port, method, action, key, app, topic, "");
			return 0;
		}
	}

	print_help();

	return 0;
}

void print_help()
{
	printf("%s %s\n"
		"Usage:\n"
		"\t%s -h [ip] -p [port] -k [token_key] [action] -a [app_name] -t [topic_name] -d [raw_data]\n"
		"Example:\n"
		"\t%s -h '127.0.0.1' -p 8000 -k 'abc' --push -a 'test_app' -t 'test_topic' -d 'hello world'\n"
		"Options:\n"
		"\t-h: thingbrok server ip address, default is %s.\n"
		"\t-p: thingbrok server listen port, default is %d.\n"
		"\t-k: thingbrok server token key, default is %s.\n"
		"\tactions:\n"
		"\t\t--push: push data to thingbrok server.\n"
		"\t\t--pull: pull data from thingbrok server.\n"
		"\t\t--app: create an application on thingbrok server.\n"
		"\t\t--topic: create a topic on thingbrok server.\n"
		"\t-a: application name.\n"
		"\t-t: topic name.\n"
		"\t-d: raw data pushed to thingbrok server.\n",
		__DEF_APP_NAME__, __DEF_VERSION__, __DEF_APP_NAME__, __DEF_APP_NAME__, __DEF_IP__, __DEF_PORT__, __DEF_KEY__
	);
}
