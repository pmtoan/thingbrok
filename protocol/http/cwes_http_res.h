#ifndef __C_WES_HTTP_RESPONSE_H__
#define __C_WES_HTTP_RESPONSE_H__

#include "cwes_http_format.h"

HTTP_RES http_res_init()
{
    /*
     *	todo @http_res_init initialize a HTTP_RESPONSE
     *	usage:
     *	    HTTP_RES res = http_res_init()
    */
    HTTP_RES res;
    res.headers = list_pair_init();
    return res;
}

void http_res_free(HTTP_RES* res)
{
    /*
     *	todo @http_res_free release memory of a HTTP_RESPONSE
     *	usage:
     *	    HTTP_RES res = http_res_init()
     *	    ...
     *	    http_res_free(&res)
    */

    free(res->version);
    free(res->payload);
    list_pair_free(&(res->headers));
}

char* http_res_compose(HTTP_RES res)
{
    /*
     * 	todo @http_res_compose compose a HTTP_RESPONSE to string
     * 	usage:
     *	    HTTP_RES res = http_res_init()
     *	    ...
     * 	    char* msg_response = http_res_compose(res);
     * 	    ...
     * 	    http_res_free(&res)
    */
    char* buffer = (char*)malloc(__SIZE_EXTRA__);
    sprintf(buffer, "%s %d %s\r\n", res.version, res.status.code, res.status.status);
    for (int i = 0; i < res.headers.len; ++i)
    {
        sprintf(buffer, "%s%s: %s\r\n", buffer, res.headers.head->key, res.headers.head->value);
        res.headers.head = res.headers.head->next;
    }
    sprintf(buffer, "%sContent-Length: %d\r\n", buffer, (int)(strlen(res.payload)));
    sprintf(buffer, "%s\r\n%s", buffer, res.payload);
    char* response = strdup(buffer);
    free(buffer);
    return response;
}

void http_res_set_header(HTTP_RES* res, const char* key, const char* value)
{
    /*
     *  todo @http_res_set_header set header for a HTTP_RESPONSE
     *  usage:
     *	    HTTP_RES res = http_res_init()
     *	    ...
     *	    http_res_set_header(&res, "Header-key", "Header-value")
     * 	    char* msg_response = http_res_compose(res);
     * 	    ...
     * 	    http_res_free(&res)
    */
    list_pair_append(&(res->headers), key, value);
}

HTTP_RES http_res_build_up(HTTP_STATUS status, const char* payload)
{
    /*
     *  todo @http_res_build_up build up a basic HTTP_RESPONSE without headers
     *  usage:
     *	    HTTP_RES res = http_res_init()
     *	    ...
     *	    http_res_build_up(&res, "Hello world");
     *	    http_res_set_header(&res, "Header-key", "Header-value")
     * 	    char* msg_response = http_res_compose(res);
     * 	    ...
     * 	    http_res_free(&res)
    */
    HTTP_RES res = http_res_init();
    res.status = status;
    res.version = __HTTP_VERSION__;
    http_res_set_header(&res, "Server", __HTTP_SERVER__);
    http_res_set_header(&res, "Connection", __HTTP_CONNECTION_STATE__);
    res.payload = strdup(payload);
    return res;
}

#endif  // __C_WES_HTTP_RESPONSE_H__