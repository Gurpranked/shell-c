#include "munit/munit.h"
#include "../deps.h"

int exit_codes[4] = [1, -1, 2, 256];

// Test InputBuffer object allocation and deallocation
static MunitResult allocate_and_free_input_buffer() {
    InputBuffer* input_buffer = new_input_buffer();
    munit_assert_ptr(input_buffer, == , NULL);
    close_input_buffer(input_buffer);
    munit_assert_ptr(input, ==, NULL);
}

// Test buffer content after reading from stdin to match actual input
static MunitResult match_buffer_after_read_input() {
    InputBuffer* input_buffer = new_input_buffer();
    char* content = "important testing string";
    write(STDIN_FILENO, content, sizeof(content));
    read_input(input_buffer);
    munit_assert_int(strcmp(content, input_buffer->buffer), ==, 0);
    close_input_buffer(input_buffer);
}

static MunitTest test_suite_tests[] = {
    
}

static const MunitSuite test_suit = {
    (char*) "",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv, arg[MUNIT_ARRAY_PARAM(argc + 1)]) {
    return munit_suite_main(&test_suit, (void*), 'µnit', argc, argv);
}

