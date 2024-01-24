#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <syslog.h>
#include <signal.h>

#include "mqtt_client_interface.h"
#include "cJSON.h"
#include "tuya_log.h"
#include "tuya_error_code.h"
#include "tuyalink_core.h"

#include "argparse.h"
#include "cloud.h"
#include "program.h"

bool connected = false;
tuya_mqtt_context_t client_instance;

int exe_program(struct arguments args)
{
        int ret = OPRT_OK;

        openlog("Tuya_deamon", LOG_PID, LOG_USER);
        syslog(LOG_INFO, "=====TUYA_DEAMON=====");

        signal(SIGTERM, signal_handler);
        signal(SIGTSTP, signal_handler);

        ret = start_deamon(args.start_deamon);
        if (ret == EXIT_FAILURE)
                return EXIT_FAILURE;

        // Waits till the program conncets to the cloud to prevent memory leak
        tuya_mqtt_context_t *client = &client_instance;

        while (!connected)
        {
                signal(SIGINT, SIG_IGN);
                ret = cloud_connect(client, args);
                if (ret != OPRT_OK)
                {
                        TY_LOGE("Error: conection to toya Iot cloud failed");
                        syslog(LOG_ERROR, "Error: conection to toya Iot cloud failed");
                        cloud_disconect(client);
                        return EXIT_FAILURE;
                }
                connected = true;
                signal(SIGINT, signal_handler);
        }

        while (true)
        {
                tuya_mqtt_loop(client);
                send_report(client, args);
        }

        return ret;
}

void signal_handler(int signal)
{
        syslog(LOG_INFO, "Teminating program...");
        cloud_disconect(&client_instance);
        closelog();
        syslog(LOG_INFO, "Program terminated!");
        exit(EXIT_SUCCESS);
}

int start_deamon(bool is_deamon)
{
        if (is_deamon)
        {
                if (daemon(0, 0))
                {
                        syslog(LOG_ERROR, "Error: failed to start program as a deamon");
                        return EXIT_FAILURE;
                }
                syslog(LOG_INFO, "Started program as a deamon!");
        }
        return EXIT_SUCCESS;
}