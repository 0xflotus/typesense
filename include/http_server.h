#pragma once

#define H2O_USE_LIBUV 0

extern "C" {
    #include "h2o.h"
    #include "h2o/http1.h"
    #include "h2o/http2.h"
    #include "h2o/multithread.h"
}

#include <map>
#include <string>
#include <stdio.h>
#include "http_data.h"

struct request_response {
    http_req* req;
    http_res* response;
};

class HttpServer {
private:
    h2o_globalconf_t config;
    h2o_compress_args_t compress_args;
    h2o_context_t ctx;
    h2o_accept_ctx_t* accept_ctx;
    h2o_hostconf_t *hostconf;
    h2o_socket_t* listener_socket;
    h2o_multithread_queue_t* message_queue;
    h2o_multithread_receiver_t* message_receiver;
    bool exit_loop = false;

    std::vector<route_path> routes;

    std::map<std::string, void (*)(void*)> message_handlers;

    const std::string listen_address;

    const uint32_t listen_port;

    std::string ssl_cert_path;

    std::string ssl_cert_key_path;

    bool cors_enabled;

    bool (*auth_handler)(const route_path & rpath, const std::string & auth_key);

    static void on_accept(h2o_socket_t *listener, const char *err);

    int setup_ssl(const char *cert_file, const char *key_file);

    int create_listener();

    h2o_pathconf_t *register_handler(h2o_hostconf_t *hostconf, const char *path,
                                     int (*on_req)(h2o_handler_t *, h2o_req_t *));

    static const char* get_status_reason(uint32_t status_code);

    static std::map<std::string, std::string> parse_query(const std::string& query);

    static int catch_all_handler(h2o_handler_t *self, h2o_req_t *req);

    static void on_message(h2o_multithread_receiver_t *receiver, h2o_linklist_t *messages);

    static int send_401_unauthorized(h2o_req_t *req);

    static constexpr const char* SEND_RESPONSE_MSG = "send_response";

public:
    HttpServer(std::string listen_address, uint32_t listen_port,
               std::string ssl_cert_path, std::string ssl_cert_key_path, bool cors_enabled);

    ~HttpServer();

    void set_auth_handler(bool (*handler)(const route_path & rpath, const std::string & auth_key));

    void get(const std::string & path, void (*handler)(http_req & req, http_res & res), bool async = false);

    void post(const std::string & path, void (*handler)(http_req & req, http_res & res), bool async = false);

    void put(const std::string & path, void (*handler)(http_req & req, http_res & res), bool async = false);

    void del(const std::string & path, void (*handler)(http_req & req, http_res & res), bool async = false);

    void on(const std::string & message, void (*handler)(void*));

    void send_message(const std::string & type, void* data);

    void send_response(http_req* request, const http_res* response);

    void stream_response(void (*handler)(http_req* req, http_res* res, void* data), http_req & request,
                         http_res & response, void* data);

    static void response_proceed(h2o_generator_t *generator, h2o_req_t *req);

    static void response_stop(h2o_generator_t *generator, h2o_req_t *req);

    int run();

    void stop();

    void clear_timeouts(const std::vector<h2o_timeout_t*> & timeouts);

    static void on_stop_server(void *data);

    static constexpr const char* AUTH_HEADER = "x-typesense-api-key";
    static constexpr const char* STOP_SERVER_MESSAGE = "STOP_SERVER";
};