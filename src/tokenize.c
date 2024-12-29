#include <stdio.h> // for printf

#include "cli_args.h" // for parse_first, parse_integer, parse_string

int main(const int argc, const char* const* argv) {
    check_for_help(argc, argv);

    const char* filepath = get_first_as_is(argc, argv);
    const int ngram = get_next_as_integer(argc, argv, "--n-gram");
    const char* allowed_chars = get_next_as_is(argc, argv, "--allowed-chars");

    printf("%s was called:\n", argv[0]);
    printf("%s is the filepath, n-gram is %i and allowed-chars is %s.\n", filepath, ngram, allowed_chars);

    return 0;
}
