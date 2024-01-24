#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>
#include <syslog.h>

#include "argparse.h"

static char doc[] = "Deamon program that communicates with Tuya Iot cloud";

static char args_doc[] = "Args: | Device ID | Product ID | Device secret |";

static struct argp_option options[] = {
    {"Device ID", 'd', "DEVICE_ID_VALUE", 0, "device id"},
    {"Product ID", 'p', "PRODUCT_ID_VALUE", 0, "product id"},
    {"Device secret", 's', "DEVICE_SECRET_VALUE", 0, "device secret"},
    {"Start Deamon", 'D', false, 0, "start program as a daemon process"},
    {0}};

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
        struct arguments *arguments = state->input;

        switch (key)
        {
        case 'd':
                arguments->device_id = arg;
                break;
        case 'p':

                arguments->product_id = arg;
                break;
        case 's':

                arguments->device_secret = arg;
                break;
        case 'D':
                arguments->start_deamon = true;
                break;
        case ARGP_KEY_END:
                if (arguments->device_id == NULL || arguments->product_id == NULL || arguments->device_secret == NULL)
                {
                        argp_usage(state);
                }
        default:
                return ARGP_ERR_UNKNOWN;
        }
        return 0;
}

struct arguments parse_arguments(int argc, char **argv)
{
        struct arguments arguments;
        static struct argp argp = {options, parse_opt, args_doc, doc};

        arguments.device_id = NULL;
        arguments.product_id = NULL;
        arguments.device_secret = NULL;
        arguments.start_deamon = false;

        syslog(LOG_INFO, "Parsing arguments...");
        argp_parse(&argp, argc, argv, 0, 0, &arguments);

        return arguments;
}
