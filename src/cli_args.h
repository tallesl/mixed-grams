#pragma once

// Checks for help in the cli arguments, prints help message if argument is present, and exits the program.
void check_for_help(const int argc, const char* const* argv);

// Checks if any cli argument was passed, checks if the first argument is not an "--" argument, and returns it.
const char* get_first_as_is(const int argc, const char* const* argv);

// Parses the given "--" argument as an integer and returns it.
// Exits the program with error if argument is invalid or absent.
long get_next_as_integer(const int argc, const char* const* argv, const char* arg_name);

// Parses the given "--" argument as an string and returns it.
// Exits the program with error if argument is invalid or absent.
const char* get_next_as_is(const int argc, const char* const* argv, const char* arg_name);
