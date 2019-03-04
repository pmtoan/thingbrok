#include "controller/thingbrok/thingbrok_controller.h"

#define __DEF_APP_NAME__	"thingbrokctl"
#define __DEF_VERSION__		"version 1.0"
#define __DEF_IP__			"127.0.0.1"
#define __DEF_PORT__		8000
#define __DEF_KEY__			"abc"

void print_help();

int main(int argc, char const *argv[])
{
	int time_to_run = 1;
	char* ip = __DEF_IP__;
	int port = __DEF_PORT__;
	char* method = "NULL";
	char* action = "NULL";
	char* key = __DEF_KEY__;
	char* app = NULL;
	char* topic = NULL;
	char* data = NULL;
	char* query = "";

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
		else if (strcmp(argv[i], "-r") == 0) time_to_run = atoi(argv[i+1]);
		else if (strcmp(argv[i], "-q") == 0) query = strdup(argv[i+1]);
	}

	if (strcmp(action, "push") == 0)
	{
		if (app != NULL && topic != NULL && data != NULL)
		{
			for (int i = 0; i < time_to_run; ++i)
			{
				controller_control_ctl(ip, port, method, action, key, app, topic, data, query);
			}
			return 0;
		}
	}

	else if (strcmp(action, "pull") == 0)
	{
		if (app != NULL && topic != NULL)
		{
			for (int i = 0; i < time_to_run; ++i)
			{
				controller_control_ctl(ip, port, method, action, key, app, topic, "", query);
			}
			return 0;
		}
	}

	else if (strcmp(action, "app") == 0)
	{
		if (app != NULL)
		{
			for (int i = 0; i < time_to_run; ++i)
			{
				controller_control_ctl(ip, port, method, action, key, app, topic, "", query);
			}
			return 0;
		}
	}

	else if (strcmp(action, "topic") == 0)
	{
		if (app != NULL && topic != NULL)
		{
			for (int i = 0; i < time_to_run; ++i)
			{
				controller_control_ctl(ip, port, method, action, key, app, topic, "", query);
			}
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
		"\t-d: raw data pushed to thingbrok server.\n"
		"\t-r: time to send request.\n"
		"\t-q: string to query data.\n"
		"\nFree software developed by Thingler project (http://thingler.xyz)\n",
		__DEF_APP_NAME__, __DEF_VERSION__, __DEF_APP_NAME__, __DEF_APP_NAME__, __DEF_IP__, __DEF_PORT__, __DEF_KEY__
	);
}
