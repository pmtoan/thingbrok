#ifndef __C_WES_HTTP_FORMAT_H__
#define __C_WES_HTTP_FORMAT_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../modules/data_structure/list_pair.h"
#include "../../modules/data_structure/list_string.h"
#include "../../modules/utilities/utils.h"

#define __HTTP_PORT__ 80
#define __HTTPS_PORT__ 443

#define __HTTP_VERSION__ "HTTP/1.1"
#define __HTTP_SERVER__ "cwes 1.1 (unix)"
#define __HTTP_CONNECTION_STATE__ "Close"

#define __HTTP_ROOT__ "/var/lib/cwes/html"

#define __HTTP_TEXT_PLAIN__			"text/plain"
#define __HTTP_TEXT_HTML__			"text/html"
#define __HTTP_TEXT_CSS__			"text/css"
#define __HTTP_TEXT_JAVASCRIPT__	"text/javascript"
#define __HTTP_APPLICATION_JSON__ 	"application/json"
#define __HTTP_APPLICATION_JAVASCRIPT__ 	"application/javascript"

/*===================================================================================
*		DATA STRUCTURE DECLARATION
*==================================================================================*/
typedef struct HTTP_STATUS
{
    int code;
    char* status;
}HTTP_STATUS;

typedef struct HTTP_REQUEST
{
    char* method;	// http request method
    char* version;	// http version
    char* uri;	// http request uri
    LIST_PAIR headers;	// http headers
    LIST_PAIR params;	// http request parameters
    char* payload;
}HTTP_REQ;

typedef struct HTTP_RESPONSE
{
    char* version;	// http version
    HTTP_STATUS status; // http response status
    LIST_PAIR headers;	// http response headers
    char* payload;	// http response payload
}HTTP_RES;

const HTTP_STATUS __HTTP_OK__ 						= {200, "OK"};
const HTTP_STATUS __HTTP_CREATED__ 					= {201, "Created"};
const HTTP_STATUS __HTTP_ACCEPTED__ 				= {202, "Accepted"};
const HTTP_STATUS __HTTP_NO_CONTENT__ 				= {204, "No Content"};
const HTTP_STATUS __HTTP_FOUND__ 					= {302, "Found"};
const HTTP_STATUS __HTTP_NOT_MODIFIED__ 			= {304, "Not Modified"};
const HTTP_STATUS __HTTP_TEMPORARY_REDIRECT__ 		= {307, "Temporary Redirect"};
const HTTP_STATUS __HTTP_PERMANENT_REDIRECT__ 		= {308, "Permanent Redirect"};
const HTTP_STATUS __HTTP_BAD_REQUEST__ 				= {400, "Bad Request"};
const HTTP_STATUS __HTTP_UNAUTHORIZED__ 			= {401, "Unauthorized "};
const HTTP_STATUS __HTTP_FORBIDDEN__ 				= {403, "Forbidden"};
const HTTP_STATUS __HTTP_NOT_FOUND__ 				= {404, "Not Found"};
const HTTP_STATUS __HTTP_METHOD_NOT_ALLOWED__ 		= {405, "Method Not Allowed"};
const HTTP_STATUS __HTTP_INTERNAL_SERVER_ERROR__ 	= {500, "Internal Server Error"};
const HTTP_STATUS __HTTP_BAD_GATEWAY__ 				= {502, "Bad Gateway"};

#endif // __C_WES_HTTP_FORMAT_H__