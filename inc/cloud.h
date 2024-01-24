#ifndef CLOUD_H_
#define CLOUD_H_

#include "tuyalink_core.h"
#include "argparse.h"

int cloud_connect(tuya_mqtt_context_t *context, struct arguments args);
void cloud_disconect(tuya_mqtt_context_t *context);
void on_connected(tuya_mqtt_context_t *context, void *user_data);
void on_disconnect(tuya_mqtt_context_t *context, void *user_data);
void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg);

void exe_action(const tuyalink_message_t *msg);
char *parse_data(char *string);
void send_report(tuya_mqtt_context_t *context, struct arguments args);
char *get_file_path(char *file_name);

#endif