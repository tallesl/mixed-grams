#include "cli_args.h"

#include <stdlib.h>  // for exit, and strtol
#include <string.h>  // for strcmp

#include "error_raising.h" // for error

void check_for_help(const int argc, const char* const* argv) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp("--help", argv[i]))
            continue;

        printf("Usage: tokenize FILE --n-gram [number of characters] --allowed-chars [list of characters to keep]\n");
        exit(0);
    }
}

const char* get_first_as_is(const int argc, const char* const* argv) {
    if (argc == 1)
        error("No argument was passed.");

    if (!strncmp("--", argv[1], 2))
        error("First argument shouldn't be a \"--\" argument.");

    return argv[1];
}

const char* get_next_as_is(const int argc, const char* const* argv, const char* arg_name) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(arg_name, argv[i]))
            continue;

        if (++i == argc)
            error("No value passed for %s.", arg_name);

        return argv[i];
    }

    error("%s was not passed.", arg_name);
#pragma GCC diagnostic ignored "-Wreturn-type"
} // ignoring the warning because the error call before the end will exit the program
#pragma GCC diagnostic warning "-Wreturn-type"

long get_next_as_integer(const int argc, const char* const* argv, const char* arg_name) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(arg_name, argv[i]))
            continue;

        if (++i == argc)
            error("No value passed for %s.", arg_name);

        char* str_end;
        long value = strtol(argv[i], &str_end, 10);

        if (*str_end)
            error("Invalid value for %s.", arg_name);

        return value;
    }

    error("%s was not passed.", arg_name);

#pragma GCC diagnostic ignored "-Wreturn-type"
} // ignoring the warning because the error call before the end will exit the program
#pragma GCC diagnostic warning "-Wreturn-type"
