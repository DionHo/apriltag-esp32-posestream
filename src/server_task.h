#ifndef SERVER_TASK_H
#define SERVER_TASK_H

void setup_http_handlers();

void setup_tcp_server();
void loop_tcp_server();

void setup_websocketserver();
void loop_websocketserver();

#endif // SERVER_TASK_H