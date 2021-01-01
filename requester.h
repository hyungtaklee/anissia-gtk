#ifndef __REQUESTOR_H
#define __REQUESTOR_H

#include <unistd.h>
#include <sys/socket.h> /* socket, connect */
#include <sys/types.h> /* getaddrinfo */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, getaddrinfo*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

// #define DEBUG

// mapping from SUN = 0 to NEW = 8
enum DAY { SUN, MON, TUE, WED, THU, FRI, SAT, ETC, NEW, FIN = 100, SUB = 200 };

struct request_data
{
    struct sockaddr_in serv_ip;
    cJSON *json_data;
    char *json_str;
};

bool requester_init(struct request_data *data);
bool requester_request(struct request_data *data, enum DAY day, int page_num, int org_num);
bool is_response_ok(char *resp);
bool parse_to_json(struct request_data *data);
char *request_parse_response (const char *resp);
char *requester_create_url(enum DAY day, int page_num, int org_num);
struct request_data *requester_create_data_struct();
char *request_create_get_req(const char *url);
bool is_totally_received(int received, char *recv_buf);

#endif
