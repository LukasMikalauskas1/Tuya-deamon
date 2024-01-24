#ifndef ARGPARSE_
#define ARGPARSE_

#include <stdbool.h>
#include <argp.h>

struct arguments
{
        char *device_id;
        char *product_id;
        char *device_secret;
        bool start_deamon;
};

struct arguments parse_arguments(int argc, char **argv);
error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif