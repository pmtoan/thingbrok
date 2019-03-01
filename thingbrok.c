#include "controller/thingbrok/thingbrok_controller.h"

#define __DEF_APP_NAME__	"Thingbrok"
#define __DEF_VERSION__		"ver 1.0"
#define __DEF_HTTP_PORT__	8000
#define __DEF_API_TOKEN__	"abc"
#define __DEF_MSG__     	"what do you want?"

void signal_handler(int sig);
void print_help();

int server;
pid_t pid_first;
pid_t pid_second;
int status;
int listen_port;
char* api_key;

int main(int argc, char const *argv[])
{
	if (argc > 1)
		if (strcmp(argv[1], "--help") == 0)
		{
			print_help();
			exit(0);
		}
	listen_port = __DEF_HTTP_PORT__;
	api_key = strdup(__DEF_API_TOKEN__);
	for (int i = 1; i < argc; ++i)
	{
		if (strstr(argv[i] , "--port"))
			listen_port = atoi(argv[i+1]);
		else if (strstr(argv[i], "--key"))
			api_key = strdup(argv[i+1]);
	}

	signal(SIGINT,signal_handler);

    server = unix_x86_64_linux_tcp_socket_open_listener(listen_port);
    _("%s - [INFO] Broker server running on 0::%d with key=%s\n", time_get_timestamp(), listen_port, api_key);
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect;
    char* message = (char*)malloc(__SIZE_EXTRA__);
    while(1)
    {
        connect = accept(server,(struct sockaddr *)&client_addr,&address_size);
        struct sockaddr_in *coming = (struct sockaddr_in *)&client_addr;
        unsigned char *ip = (unsigned char *)&coming->sin_addr.s_addr;
        unsigned short port = coming->sin_port;
        if (connect < 0)
        {
            /* Can't accept connect from client */
            _("%s - [ERROR] %d.%d.%d.%d:%-8d ",
                   time_get_timestamp(),
                   ip[0], ip[1], ip[2], ip[3], port);
            continue;
        }

        pid_first = fork();
        if (pid_first == 0)
        {
            signal(SIGINT, SIG_DFL);
            pid_second = fork();
            if (pid_second == 0)
			{
				signal(SIGINT, SIG_DFL);
				unix_x86_64_linux_tcp_socket_read_msg(connect, message, __SIZE_EXTRA__);
				if (http_req_validate(message) != 1)
				{
					unix_x86_64_linux_tcp_socket_send_msg(connect, __DEF_MSG__);
					close(connect);
					exit(0);
				}
				HTTP_REQ req = http_req_parse_msg(message);
				_("%s - [INFO] %d.%d.%d.%d:%-8d%s %s %s\n",
						time_get_timestamp(),
						ip[0], ip[1], ip[2], ip[3], port,
						req.version, req.method, req.uri);
				unix_x86_64_linux_tcp_socket_send_msg(connect, controller_control_everything(req, api_key));
				close(connect);
			}
            close(connect);
            exit(0);
        }
		wait(NULL);
        close(connect);
    }
}

void signal_handler(int sig)
{
    _("\n%s - [INFO] Broker server exited\n", time_get_timestamp());
    close(server);
    exit(0);
}

void print_help()
{
	printf("%s - Broker for application %s\n"
		"\nUsage\n"
        "\tthingbrok --port [listen-port] --key [api_token_key]\n"
		"\nExample\n"
        "\tthingbrok --port 8080 --key abc\n"
		"\nOptions\n"
        "\t--port: Port number Thingbrok listen on\n"
        "\t--key:  API token key Thingbrok use. Client request with header Broker-Token: [api_key]\n"
		"\t        If run without --key option, key will be 'abc'\n"
		"\t--help: Show Thingbrok help information\n"
        "\nFree software developed by Thingler project\n",
        __DEF_APP_NAME__, __DEF_VERSION__
  	);
}
