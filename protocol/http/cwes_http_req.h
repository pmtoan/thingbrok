
#ifndef __C_WES_HTTP_REQUEST_H__
#define __C_WES_HTTP_REQUEST_H__
#include "cwes_http_format.h"

/*===================================================================================
*		FUNCTION DECLARATION
*==================================================================================*/

HTTP_REQ http_req_init();
void http_req_free(HTTP_REQ* req);
HTTP_REQ http_req_parse_msg(const char* msg);
void _http_req_parse_request_line(HTTP_REQ* req, const char* rl);
void _http_req_parse_headers(HTTP_REQ* req, const char* hd);
char* http_req_get_header(HTTP_REQ req, const char* key);
int http_req_validate(const char* msg);




/*===================================================================================
*		FUNCTION IMPLEMENT
*==================================================================================*/
HTTP_REQ http_req_parse_msg(const char* msg)
{
    /*
     *	todo: @http_req_parse_msg parse a http request msg to HTTP_REQUEST structure
     *	usage:
     *	    HTTP_REQ req = http_req_parse_msg(ms);
    */
    HTTP_REQ req = http_req_init();

    /* split http request headers and payload */
    char** headers_payload = string_split(msg, "\r\n\r\n", 1);

    /* 1. time to parse request line and headers */
    char** request_line_headers = string_split(headers_payload[0], "\r\n", 1);

    /* 1.1. time to parse request-line */
    _http_req_parse_request_line(&req, request_line_headers[0]);

    /* 1.2. time to parse headers, request_line_headers[1] */
    _http_req_parse_headers(&req, request_line_headers[1]);

    /* 2. time to parse parameters in payload if have */
    if (headers_payload[1] != NULL)
        req.payload = strdup(headers_payload[1]);

    /* release memory */
    free(headers_payload[0]);
    free(headers_payload);

    free(request_line_headers[0]);
    free(request_line_headers[1]);
    free(request_line_headers);

    return req;
}

HTTP_REQ http_req_init()
{
    /*
     *	todo: @http_req_init initialize a HTTP_REQUEST
     *	usage:
     *	    HTTP_REQ req = http_req_init()
    */
    HTTP_REQ req;
    req.headers = list_pair_init();
    req.params = list_pair_init();
    return req;
}

void http_req_free(HTTP_REQ* req)
{
    /*
     *	todo: @http_req_free release memory of a HTTP_REQUEST
     *	usage:
     *	    HTTP_REQ req = http_req_init()
     *	    ...
     *	    http_req_free(&req)
    */
    free(req->method);
    free(req->uri);
    free(req->version);
    list_pair_free(&(req->headers));
    list_pair_free(&(req->params));
    free(req->payload);
}

void _http_req_parse_request_line(HTTP_REQ* req, const char* rl)
{
    /*
    *	@_http_req_parse_request_line parse a http request-line
    */
    char** rl_pattern = string_split(rl, " ", 2);

    /* time to parse http method */
    req->method = strdup(rl_pattern[0]);

    /* parse http get parameter if have */
    if (string_count_cha(rl_pattern[1], '?') > 0)
    {
        /* get method contain parameters, ex: http://127.0.0.1/index.html?name=toan */
        char** get_uri_params = string_split(rl_pattern[1], "?", 1);
        req->params = list_pair_parse_x_www_form_urlencoded(get_uri_params[1]);
        req->uri = strdup(get_uri_params[0]);
        free(get_uri_params[0]);
        free(get_uri_params[1]);
        free(get_uri_params);
    }
    else
        /* get method not contain parameters, ex: http://127.0.0.1/index.html */
        req->uri = strdup(rl_pattern[1]);

    /* 1.1.3. parse http version */
    req->version = strdup(rl_pattern[2]);
    free(rl_pattern[0]);
    free(rl_pattern[1]);
    free(rl_pattern[2]);
    free(rl_pattern);
}

void _http_req_parse_headers(HTTP_REQ* req, const char* hd)
{
    /*
    *	@_http_req_parse_headers parse http headers
    */
    int number_of_header = string_count_str(hd, "\r\n");
    char** request_headers = string_split(hd, "\r\n", number_of_header);

    for (int i = 0; i <= number_of_header; ++i)
    {
        char** header = string_split(request_headers[i], ": ", 1);
        list_pair_append(&(req->headers), header[0], header[1]);
        free(header[0]);
        free(header[1]);
        free(header);
    }
    for (int i = 0; i < number_of_header; ++i)
        free(request_headers[i]);
    free(request_headers);
}

char* http_req_get_header(HTTP_REQ req, const char* key)
{
    /*
     * 	todo: @http_req_get_header get header value from @req with @key
     * 	usage:
     * 	    HTTP_REQ req = http_req_parse_msg(msg)
     * 	    ...
     * 	    char* cookie = http_req_get_header(req, "Cookie")
     * 	    ...
    */
    return list_pair_find(req.headers, key);
}

int http_req_validate(const char* msg)
{
    /*
     *  todo: @http_req_validate validate a http request message
     *  return 1 if request is valid, otherwise, return 0
    */
    if (!strstr(msg, " HTTP")) return 0;
    if (!strstr(msg, "GET")
    && !strstr(msg, "POST")
    && !strstr(msg, "PUT")
    && !strstr(msg, "HEAD")
    && !strstr(msg, "DELETE")
    && !strstr(msg, "PATCH")) return 0;
    return 1;
}

#endif  // __C_WES_HTTP_REQUEST_H__