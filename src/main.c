#include "argparse.h"
#include "program.h"

int main(int argc, char **argv)
{
        struct arguments arguments = parse_arguments(argc, argv);
        return exe_program(arguments);
}
