#ifndef __THINGLER_BROKER_CONTROLLER_H__
#define __THINGLER_BROKER_CONTROLLER_H__

#include "../../modules/unix/unix_x86_64_linux_tcp_socket.h"
#include "../../modules/utilities/utils_string.h"
#include "../../modules/utilities/utils_time.h"
#include "../../modules/data_structure/list_pair.h"
#include "../../modules/data_structure/list_string.h"
#include "../../modules/database/database.h"
#include "../../protocol/http/cwes_http.h"

#define __PUSH__    "/push"
#define __PULL__    "/pull"
#define __APP__     "/app"
#define __TOPIC__   "/topic"

/************************************************************************/
char* controller_control_everything(HTTP_REQ req, const char* api_token);

char* _controller_control_pull(HTTP_REQ req);
char* _controller_control_push(HTTP_REQ req);
char* _controller_control_app(HTTP_REQ req);
char* _controller_control_topic(HTTP_REQ req);

void controller_control_ctl(
    const char* ip,
    int port,
    const char* method,
    const char* action,
    const char* key,    
    const char* app,    
    const char* topic,
    const char* data,
    const char* query   
);
/************************************************************************/

char* controller_control_everything(HTTP_REQ req, const char* api_token) {
    /*
     *  todo @controller_control_everything check and make decision for all http request
     */

    if (strcmp(http_req_get_header(req, "Broker-Token"), api_token) != 0) {
        return http_res_compose(
                http_res_build_up(
                        __HTTP_UNAUTHORIZED__,
                        "{\"msg\": \"you don't have permission !\"}"
                ));
    }

    if (strcmp(req.uri, __PUSH__) == 0 && strcmp(req.method, "POST") == 0) {
        return http_res_compose(
                http_res_build_up(
                        __HTTP_OK__,
                        _controller_control_push(req)
                ));
    }
    else if (strcmp(req.uri, __PULL__) == 0)
    {
        return http_res_compose(
                http_res_build_up(
                        __HTTP_OK__,
                        _controller_control_pull(req)
                ));
    }

    else if (strcmp(req.uri, __APP__) == 0)
    {
        return http_res_compose(
                http_res_build_up(
                        __HTTP_OK__,
                        _controller_control_app(req)
                ));
    }

    else if (strcmp(req.uri, __TOPIC__) == 0)
    {
        return http_res_compose(
                http_res_build_up(
                        __HTTP_OK__,
                        _controller_control_topic(req)
                ));
    }

    return http_res_compose(
        http_res_build_up(
            __HTTP_NOT_FOUND__,
            "{\"msg\": \"path invalid !\"}"
        ));
}

char* _controller_control_pull(HTTP_REQ req)
{
    /*
    *   @todo: route a pull request
    */
    LIST_STRING data_rows;
    char* query = list_pair_find(req.params, "query");
    if (strcmp(query, "") != 0)
        data_rows = _db_do_select_contain_string(
            list_pair_find(req.params, "app_name"),
            list_pair_find(req.params, "topic_name"),
            query
        );
    else
        data_rows = _db_do_select_last(
            list_pair_find(req.params, "app_name"),
            list_pair_find(req.params, "topic_name")
        );
    char* data = list_string_compose_list_string(data_rows);
    list_string_free(&data_rows);
    return data;
}

char* _controller_control_push(HTTP_REQ req)
{
    /*
    *   @todo: route a push request
    */
    if (req.payload != NULL)
        if (strcmp(req.payload, "") != 0)
		{
        	int rs = _db_do_insert(list_pair_find(req.params, "app_name"), list_pair_find(req.params, "topic_name"), req.payload);
        	if (rs == __DATABASE_QUERY_OK__) return "{\"msg\":\"query ok\"}";
		}

    return "{\"msg\":\"query failed\"}";
}

char* _controller_control_app(HTTP_REQ req)
{
    /*
    *   @todo: route a create application request
    */
    int rs = _db_do_create_application(list_pair_find(req.params, "app_name"));
    if (rs == __DATABASE_QUERY_OK__) return "{\"msg\":\"query ok\"}";
    return "{\"msg\":\"query failed\"}";
}

char* _controller_control_topic(HTTP_REQ req)
{
    /*
    *   @todo: route a create topic request
    */
    int rs = _db_do_create_topic(list_pair_find(req.params, "app_name"), list_pair_find(req.params, "topic_name"));
    if (rs == __DATABASE_QUERY_OK__) return "{\"msg\":\"query ok\"}";
    return "{\"msg\":\"query failed\"}";
}

void controller_control_ctl(
    const char* ip,
    int port,
    const char* method,
    const char* action,
    const char* key,    
    const char* app,    
    const char* topic,
    const char* data,
    const char* query   
){
    /*
    *   @todo: make request to thingbrok server, called by thingbrokctl
    */
    int sd = unix_x86_64_linux_tcp_socket_connect(ip, port);
    if (sd == -1) return;
    char* msg = (char*)malloc(__SIZE_EXTRA__);
    sprintf(
        msg,
        "%s /%s?app_name=%s&topic_name=%s&query=%s HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Broker-Token: %s\r\n\r\n"
        "%s",
        method, action, app, topic, query, ip, port, key, data
    );
    unix_x86_64_linux_tcp_socket_send_msg(sd, msg);
    unix_x86_64_linux_tcp_socket_read_msg(sd, msg, __SIZE_EXTRA__);
    printf("%s\n", string_split(msg, "\r\n\r\n", 1)[1]);
    free(msg);
    close(sd);
}

#endif // __THINGLER_BROKER_CONTROLLER_H__