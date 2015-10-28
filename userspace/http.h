/*
    RExOS - embedded RTOS
    Copyright (c) 2011-2015, Alexey Kramarenko
    All rights reserved.
*/

#ifndef HTTP_H
#define HTTP_H

typedef enum {
    HTTP_GET,
    HTTP_HEAD,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_CONNECT,
    HTTP_OPTIONS,
    HTTP_TRACE
} HTTP_METHOD;

typedef enum {
    HTTP_0_9,
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0,
} HTTP_VERSION;

typedef struct {
    HTTP_METHOD method;
    HTTP_VERSION version;
    char* url;
    unsigned int url_len;
    char* head;
    unsigned int head_len;
    char* body;
    unsigned int body_len;
} HTTP_REQUEST;

typedef enum {
    HTTP_RESPONSE_CONTINUE = 100,
    HTTP_RESPONSE_SWITCHING_PROTOCOLS = 101,
    HTTP_RESPONSE_OK = 200,
    HTTP_RESPONSE_CREATED = 201,
    HTTP_RESPONSE_ACCEPTED = 202,
    HTTP_RESPONSE_NON_AUTHORITATIVE_INFORMATION = 203,
    HTTP_RESPONSE_NO_CONTENT = 204,
    HTTP_RESPONSE_RESET_CONTENT = 205,
    HTTP_RESPONSE_PARTIAL_CONTENT = 206,
    HTTP_RESPONSE_MULTIPLE_CHOICES = 300,
    HTTP_RESPONSE_MOVED_PERMANENTLY = 301,
    HTTP_RESPONSE_FOUND = 302,
    HTTP_RESPONSE_SEE_OTHER = 303,
    HTTP_RESPONSE_NOT_MODIFIED = 304,
    HTTP_RESPONSE_USE_PROXY = 305,
    HTTP_RESPONSE_TEMPORARY_REDIRECT = 307,
    HTTP_RESPONSE_BAD_REQUEST = 400,
    HTTP_RESPONSE_UNATHORIZED = 401,
    HTTP_RESPONSE_PAYMENT_REQUIRED = 402,
    HTTP_RESPONSE_FORBIDDEN = 403,
    HTTP_RESPONSE_NOT_FOUND = 404,
    HTTP_RESPONSE_METHOD_NOT_ALLOWED = 405,
    HTTP_RESPONSE_NOT_ACCEPTABLE = 406,
    HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED = 407,
    HTTP_RESPONSE_REQUEST_TIMEOUT = 408,
    HTTP_RESPONSE_CONFLICT = 409,
    HTTP_RESPONSE_GONE = 410,
    HTTP_RESPONSE_LENGTH_REQUIRED = 411,
    HTTP_RESPONSE_PRECONDITION_FAILED = 412,
    HTTP_RESPONSE_PAYLOAD_TOO_LARGE = 413,
    HTTP_RESPONSE_URI_TOO_LONG = 414,
    HTTP_RESPONSE_UNSUPPORTED_MEDIA_TYPE = 415,
    HTTP_RESPONSE_RANGE_NOT_SATISFIABLE = 416,
    HTTP_RESPONSE_EXPECTATION_FAILED = 417,
    HTTP_RESPONSE_UPGRADE_REQUIRED = 426,
    HTTP_RESPONSE_INTERNAL_SERVER_ERROR = 500,
    HTTP_RESPONSE_NOT_IMPLEMENTED = 501,
    HTTP_RESPONSE_BAD_GATEWAY = 502,
    HTTP_RESPONSE_SERVICE_UNAVAILABLE = 503,
    HTTP_RESPONSE_GATEWAY_TIMEOUT = 504,
    HTTP_RESPONSE_HTTP_VERSION_NOT_SUPPORTED = 505
} HTTP_RESPONSE;

HTTP_RESPONSE http_parse_request(char* data, unsigned int size, HTTP_REQUEST* req);
void test_print(HTTP_RESPONSE resp);

#endif // HTTP_H