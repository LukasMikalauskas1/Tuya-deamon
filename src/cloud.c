#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <syslog.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include "mqtt_client_interface.h"
#include "cJSON.h"
#include "tuya_cacert.h"
#include "tuya_log.h"
#include "tuya_error_code.h"
#include "tuyalink_core.h"
#include "argparse.h"
#include "cloud.h"

void on_connected(tuya_mqtt_context_t *context, void *user_data)
{
        syslog(LOG_INFO, "Connected to cloud!");
        TY_LOGI("on connected");
}

void on_disconnect(tuya_mqtt_context_t *context, void *user_data)
{
        syslog(LOG_INFO, "Disconnected from cloud!");
        closelog();
        TY_LOGI("on disconnect");
}

void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg)
{
        syslog(LOG_INFO, "on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
        TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
        switch (msg->type)
        {
        case THING_TYPE_PROPERTY_SET:
                syslog(LOG_INFO, "Property set message: %s", msg->data_string);
                TY_LOGI("Property setm message: %s", msg->data_string);
        case THING_TYPE_ACTION_EXECUTE:
                syslog(LOG_INFO, "Executing action message: %s", msg->data_string);
                TY_LOGI("Executing action message: %s", msg->data_string);
                exe_action(msg);
        default:
                syslog(LOG_INFO, "Default message: %s", msg->data_string);
                TY_LOGI("Default message: %s", msg->data_string);
                break;
        }
        printf("\r\n");
}

void cloud_disconect(tuya_mqtt_context_t *context)
{
        syslog(LOG_INFO, "Disconecting from cloud...");
        tuya_mqtt_disconnect(context);
        tuya_mqtt_deinit(context);
}

int cloud_connect(tuya_mqtt_context_t *context, struct arguments args)
{
        syslog(LOG_INFO, "Conectinng to cloud...");
        int ret = OPRT_OK;
        ret = tuya_mqtt_init(context, &(const tuya_mqtt_config_t){
                                          .host = "m1.tuyacn.com",
                                          .port = 8883,
                                          .cacert = (const uint8_t *)tuya_cacert_pem,
                                          .cacert_len = sizeof(tuya_cacert_pem),
                                          .device_id = args.device_id,
                                          .device_secret = args.device_secret,
                                          .keepalive = 100,
                                          .timeout_ms = 2000,
                                          .on_connected = on_connected,
                                          .on_disconnect = on_disconnect,
                                          .on_messages = on_messages});

        if (ret != OPRT_OK)
        {
                syslog(LOG_INFO, "mqtt conncet failed");
                exit(EXIT_FAILURE);
        }

        ret = tuya_mqtt_connect(context); // memleak

        if (ret != OPRT_OK)
        {
                syslog(LOG_INFO, "mqtt conncet failed");
                exit(EXIT_FAILURE);
        }
        return ret;
}

void exe_action(const tuyalink_message_t *msg)
{
        syslog(LOG_INFO, "Executing action...");
        FILE *file;
        char *path = NULL;
        path = get_file_path("TuyaDeamonData.txt");
        file = fopen(path, "a");
        char *data = parse_data(msg->data_string);
        fprintf(file, "Response - %s\n", data);
        free(data);
        free(path);
        fclose(file);
}

char *get_file_path(char *file_name)
{
        char *home_dir = getenv("HOME");
        char *path = (char *)malloc((strlen(home_dir) + strlen(file_name) + 2) * sizeof(char));
        sprintf(path, "%s/%s", home_dir, file_name);
        return path;
}

char *parse_data(char *string)
{
        cJSON *json_string = cJSON_Parse(string);
        cJSON *input_params = cJSON_GetObjectItem(json_string, "inputParams");
        cJSON *message = cJSON_GetObjectItem(input_params, "data");
        char *result = cJSON_Print(message);
        cJSON_Delete(json_string);
        return result;
}

struct sysinfo sys;
void send_report(tuya_mqtt_context_t *context, struct arguments args)
{
        sleep(1);
        sysinfo(&sys);
        char data[30];
        snprintf(data, sizeof(data), "{\"FreeRam\":\"%lu KB\"}", sys.freeram * sys.mem_unit / 1024);
        syslog(LOG_INFO, "Sending report to cloud");
        tuyalink_thing_property_report(context, args.device_id, data);
}
