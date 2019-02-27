#ifndef __THINGLER_BROKER_CONTROLLER_H__
#define __THINGLER_BROKER_CONTROLLER_H__

#include "../../modules/utilities/utils_string.h"
#include "../../modules/utilities/utils_time.h"
#include "../../modules/data_structure/list_pair.h"
#include "../../modules/data_structure/list_string.h"
#include "../../modules/database/database.h"
#include "../../protocol/http/cwes_http.h"

#define __PUSH__    "/push"
#define __PULL__    "/pull"

char* controller_control_everything(HTTP_REQ req, const char* api_token);
char* _controller_control_pull(HTTP_REQ req);
char* _controller_control_push(HTTP_REQ req);


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

    if (strcmp(req.uri, __PUSH__) == 0) {
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


    return http_res_compose(
        http_res_build_up(
            __HTTP_NOT_FOUND__,
            "{\"msg\": \"path invalid !\"}"
        ));
}

char* _controller_control_pull(HTTP_REQ req)
{
    LIST_STRING data_rows = _db_do_select(list_pair_find(req.params, "app_name"), list_pair_find(req.params, "topic_name"));
    char* data = list_string_compose_list_string(data_rows);
    list_string_free(&data_rows);
    return data;
}

char* _controller_control_push(HTTP_REQ req)
{
    if (req.payload != NULL)
        if (strcmp(req.payload, "") != 0)
		{
        	int rs = _db_do_insert(list_pair_find(req.params, "app_name"), list_pair_find(req.params, "topic_name"), req.payload);
        	if (rs == __DATABASE_QUERY_OK__) return "{\"msg\":\"query ok\"}";
		}

    return "{\"msg\":\"query failed\"}";
}


#endif // __THINGLER_BROKER_CONTROLLER_H__