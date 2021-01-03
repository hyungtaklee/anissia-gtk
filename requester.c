#include "requester.h"

#include "cJSON.h"

#define RECV_BUF_SIZE 8192  

bool requester_init(struct request_data *data)
{
    char hostname[16] = "www.anissia.net";
    int ret;
    struct addrinfo hints;
    struct addrinfo *serv_info;

    memset(&(data->serv_ip), 0, sizeof(data->serv_ip));
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // v4 v6 를 받으려면 AF_UNSPEC
    hints.ai_socktype = SOCK_STREAM;

    ret = getaddrinfo(hostname, NULL, &hints, &serv_info);
    if (ret != 0) {
        fprintf(stderr, "Error: Program failed to get server info\nLINE: %d\n\t:: %s\n",
                 __LINE__, gai_strerror(ret));
                return false;
    }
    memcpy(&(data->serv_ip), serv_info->ai_addr, sizeof(struct sockaddr));
    freeaddrinfo(serv_info);

    return true;
}

bool requester_request(struct request_data *data, enum DAY day, int page_num, int org_num)
{
    char *url_string;
    char *get_request_data;
    int sockfd;
    struct sockaddr_in servaddr;

    /* make URL for GET request */
    url_string = requester_create_url(day, page_num, org_num);
    if (url_string == NULL) {
        return false;
    }

    /* create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "Error: Program failed to get server info\nFILE:%s LINE: %d\n",
                __FILE__, __LINE__);
        return false;
    }

    /* set connection info */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(80);
    memcpy(&servaddr.sin_addr.s_addr,
           &(data->serv_ip.sin_addr.s_addr) ,4);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr *)(&servaddr), sizeof(servaddr)) < 0) {
        fprintf(stderr, "Error: Program failed to connect to the server\nFILE:%s LINE: %d\n",
                __FILE__, __LINE__);
        return false;
    }

    /* write a GET request to socket */
    get_request_data = request_create_get_req(url_string);
    if (get_request_data == NULL) {
        return false;
    }
    int total = strlen(get_request_data);
    int sent = 0;
    while (sent < total) {
        int wrote = send(sockfd, get_request_data + sent, total - sent, 0);
        if (wrote < 0) {
            fprintf(stderr, "Program failed to send a request to the server\nFILE:%s LINE: %d\n",
                __FILE__, __LINE__);
            return false;
        }
        sent += wrote;
    }

    free(get_request_data);

    /* get response from server (JSON data)
     * I referred https://stackoverflow.com/questions/22077802/
     * simple-c-example-of-doing-an-http-post-and-consuming-the-response
     */
    char *receive_buf = malloc(sizeof(char) * RECV_BUF_SIZE);
    memset(receive_buf, 0, sizeof(char) * RECV_BUF_SIZE); // _msize(p)
    total = RECV_BUF_SIZE - 1;
    int received = 0;
    ssize_t bytes;
    while (received < total) {
        bytes = recv(sockfd, receive_buf + received, total - received, 0);
        if (bytes < 0) {
            fprintf(stderr, "Error: Program failed to receive a response from server\nFILE:%s LINE: %d\n",
                    __FILE__, __LINE__);  
            goto err;
        }
        if (bytes == 0)
            break;

        received += bytes;

        if (is_totally_received(received, receive_buf))
            break;
    }

    if (received == total) {
        fprintf(stderr, "Error: Response from the server might exceed the buffer size\n");
        goto err;
    }

    close(sockfd);

    /* check whether response is OK */
    int ret;
    if (!is_response_ok(receive_buf))
        goto err;

    /* make raw string to std::string */
    data->json_str = request_parse_response(receive_buf);
    free(receive_buf);

    return true;

    // terminate function when error occurs after allocating 'receive_buf'
err:
    fprintf(stderr, "Error: error occues while request data\tFILE: %s\n", __FILE__);
    free(receive_buf);
    close(sockfd);
    return false;
}

bool is_response_ok(char *resp) {
    char search[] = "HTTP/1.1 200";

    if (strstr(resp, search) == NULL)
        return false;
    else
        return true;
}

struct request_data *requester_create_data_struct()
{
    struct request_data *new_data;

    new_data = malloc(sizeof(struct request_data));
    if (new_data == NULL) {
        fprintf(stderr, "Error: failed to make new data structure\nFILE: %s\tLine:%d\n",
                __FILE__, __LINE__);

        return NULL;
    }

    memset(new_data, 0, sizeof(struct request_data));
    
    return new_data;
}

char *request_parse_response (const char *resp)
{
    char *pos;
    char *new_string;
    size_t new_len;

    pos = strstr(resp, "\r\n\r\n");
    pos = pos + 4;
    if (pos == NULL) {
        new_len = strlen("[]");
        new_string = malloc(new_len + 1);
        strcpy(new_string, "[]");
    }
    else {
        new_len = strlen(pos);
        new_string = malloc(new_len + 1);
        strcpy(new_string, pos);
    }

    return new_string;
}

bool parse_to_json(struct request_data *data)
{
    if (data->json_str == NULL)
        return false;
    

    data->json_data = cJSON_Parse(data->json_str);

    free(data->json_str);

    // DEBUG
    // printf("FILE: %s\tLINE: %d\n%s\n\n", __FILE__, __LINE__,
    //         cJSON_Print(data->json_data));

    return true;
}

char *requester_create_url(enum DAY day, int page_num, int org_num)
{
    char *new_url;
    size_t temp_size;

    /* Set URL */
    if (day < NEW && day >= SUN && page_num == -1 && org_num == -1) {
        temp_size = strlen("/anitime/list?w=");
        new_url = malloc(temp_size + 10); /* + 10 is for day */

        sprintf(new_url, "%s%d", "/anitime/list?w=", day);
    }
    else if (day == FIN && page_num > -1 && org_num == -1) {
        temp_size = strlen("/anitime/end?p=");
        new_url = malloc(temp_size + 10);

        sprintf(new_url, "%s%d", "/anitime/end?p=", page_num);
    }
    else if (day == SUB && page_num == -1 && org_num > -1) {
        temp_size = strlen("/anitime/cap?i=");
        new_url = malloc(temp_size + 10);

        sprintf(new_url, "%s%d", "/anitime/cap?i=", org_num);
    }
    else { // error
        fprintf(stderr, "Error: occurs while set url\tFILE: %s\tLINE: %d\n",
                __FILE__, __LINE__);
        return NULL;
    }

    return new_url;
}

char *request_create_get_req(const char *url)
{
    char *req_cstr = NULL;
    size_t str_lenght = 0;

    str_lenght += strlen("GET ");
    str_lenght += strlen(url);
    str_lenght += strlen(" HTTP/1.1\r\nHost: www.anissia.net\r\n\r\n");
    
    req_cstr = malloc(str_lenght + 1);

    sprintf(req_cstr, "%s%s%s", "GET ", url, 
            " HTTP/1.1\r\nHost: www.anissia.net\r\n\r\n");

    return req_cstr;
}

bool is_totally_received(int received, char *recv_buf)
{
    size_t len = 0;
    char *pos = strstr(recv_buf, "Content-Length:");
    int content_len;
    
    if (pos == NULL)
        return false;

    pos = pos + 16;
    content_len = strtol(pos, NULL, 10);

    pos = strstr(recv_buf, "\r\n\r\n");
    pos = pos + 4;
    
    if (content_len == strlen(pos))
        return true;
    else
        return false;
}

bool parse_json_info(struct ani_info *dest, const cJSON *src_json)
{
    cJSON *trav = src_json->child;
    int int_data[8] = {0, };
    char *str_data[8] = {0, };
    int i;
    wchar_t *title_data;

    /*
    * parsing order
    * i (id) valueint
    * s (title) valuestring
    * t (time) valuestring
    * g (genre) valuestring
    * l (url) valuestring
    * a (status) valueint
    * sd (start date) valuestring
    * ed (end date) valuestring 
    */
    const bool ani_info_copy[8] = {true, false, false, false, false, true, false, false};

    for (i = 0; i < 8; i++) {
        if (trav == NULL) {
            return false;
        }
        if (ani_info_copy[i]) {
            int_data[i] = trav->valueint;
        }
        else {
            size_t len = malloc_usable_size(trav->valuestring);
            str_data[i] = malloc(len);
            memcpy(str_data[i], trav->valuestring, len);
        }
        trav = trav->next;
    }

    dest->id = int_data[0];
    dest->title = str_data[1];
    dest->time = str_data[2];
    dest->genre = str_data[3];
    dest->url = str_data[4];
    dest->status = int_data[5];
    dest->start_date = str_data[6];
    dest->end_date = str_data[7];

    return true;
}
void clean_ani_info(struct ani_info *info)
{
    /* DEBUG */
    print_ani_info(info);

    if (info->title)
        free(info->title);
    
    if (info->time)
        free(info->time);

    if (info->genre)
        free(info->genre);
    
    if (info->url)
        free(info->url);
    
    if (info->start_date)
        free(info->start_date);

    if (info->end_date)
        free(info->end_date);
}

void print_ani_info(const struct ani_info *a)
{
    g_print("id: %d\nstatus: %d\ntime: %s\ntitle: ", a->id, a->status, a->time);
    g_print("%s", a->title);
    g_print("\ngenre: %s\nstart_date: %s\nend_date: %s\nurl: %s\n",
           a->genre, a->start_date, a->end_date, a->url);
}