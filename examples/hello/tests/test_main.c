/**
 * @file test_main.c
 * @brief Unit tests for the Hello library.
 *
 * Simple test framework without external dependencies.
 */

#include "hello/hello.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * Test Framework
 * ============================================================ */

static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST(name) static void test_##name(void)
#define RUN_TEST(name) run_test(test_##name, #name)

#define ASSERT(condition) do { \
    if (!(condition)) { \
        printf("    FAIL: %s:%d: %s\n", __FILE__, __LINE__, #condition); \
        return; \
    } \
} while (0)

#define ASSERT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        printf("    FAIL: %s:%d: expected %d, got %d\n", \
               __FILE__, __LINE__, (int)(expected), (int)(actual)); \
        return; \
    } \
} while (0)

#define ASSERT_STR_EQ(expected, actual) do { \
    if (strcmp((expected), (actual)) != 0) { \
        printf("    FAIL: %s:%d: expected \"%s\", got \"%s\"\n", \
               __FILE__, __LINE__, (expected), (actual)); \
        return; \
    } \
} while (0)

#define ASSERT_NULL(ptr) do { \
    if ((ptr) != NULL) { \
        printf("    FAIL: %s:%d: expected NULL\n", __FILE__, __LINE__); \
        return; \
    } \
} while (0)

#define ASSERT_NOT_NULL(ptr) do { \
    if ((ptr) == NULL) { \
        printf("    FAIL: %s:%d: unexpected NULL\n", __FILE__, __LINE__); \
        return; \
    } \
} while (0)

typedef void (*TestFunc)(void);

static void run_test(TestFunc func, const char *name) {
    g_tests_run++;
    printf("  %s... ", name);

    hello_clear_error();  // Clear any previous errors
    func();

    // If we got here without returning early, test passed
    g_tests_passed++;
    printf("OK\n");
}

/* ============================================================
 * Tests - Lifecycle
 * ============================================================ */

TEST(create_default) {
    Hello_Greeter *greeter = hello_greeter_create(NULL);
    ASSERT_NOT_NULL(greeter);
    ASSERT_STR_EQ("World", hello_greeter_get_name(greeter));
    hello_greeter_destroy(greeter);
}

TEST(create_custom_name) {
    Hello_GreeterConfig config = HELLO_GREETER_CONFIG_DEFAULT;
    config.name = "Custom";

    Hello_Greeter *greeter = hello_greeter_create(&config);
    ASSERT_NOT_NULL(greeter);
    ASSERT_STR_EQ("Custom", hello_greeter_get_name(greeter));
    hello_greeter_destroy(greeter);
}

TEST(create_empty_name_fails) {
    Hello_GreeterConfig config = HELLO_GREETER_CONFIG_DEFAULT;
    config.name = "";

    Hello_Greeter *greeter = hello_greeter_create(&config);
    ASSERT_NULL(greeter);
    ASSERT(hello_has_error());
}

TEST(destroy_null_safe) {
    // Should not crash
    hello_greeter_destroy(NULL);
}

/* ============================================================
 * Tests - Greet
 * ============================================================ */

TEST(greet_default) {
    Hello_Greeter *greeter = hello_greeter_create(NULL);
    ASSERT_NOT_NULL(greeter);

    char buffer[128];
    int len = hello_greeter_greet(greeter, buffer, sizeof(buffer));

    ASSERT(len > 0);
    ASSERT_STR_EQ("Hello, World!", buffer);

    hello_greeter_destroy(greeter);
}

TEST(greet_custom) {
    Hello_GreeterConfig config = {
        .name = "Test",
        .greeting = "Hi",
        .uppercase = false
    };

    Hello_Greeter *greeter = hello_greeter_create(&config);
    ASSERT_NOT_NULL(greeter);

    char buffer[128];
    hello_greeter_greet(greeter, buffer, sizeof(buffer));
    ASSERT_STR_EQ("Hi, Test!", buffer);

    hello_greeter_destroy(greeter);
}

TEST(greet_uppercase) {
    Hello_GreeterConfig config = HELLO_GREETER_CONFIG_DEFAULT;
    config.uppercase = true;

    Hello_Greeter *greeter = hello_greeter_create(&config);
    ASSERT_NOT_NULL(greeter);

    char buffer[128];
    hello_greeter_greet(greeter, buffer, sizeof(buffer));
    ASSERT_STR_EQ("HELLO, WORLD!", buffer);

    hello_greeter_destroy(greeter);
}

TEST(greet_null_greeter_fails) {
    char buffer[128];
    int len = hello_greeter_greet(NULL, buffer, sizeof(buffer));
    ASSERT(len < 0);
    ASSERT(hello_has_error());
}

TEST(greet_null_buffer_fails) {
    Hello_Greeter *greeter = hello_greeter_create(NULL);
    ASSERT_NOT_NULL(greeter);

    int len = hello_greeter_greet(greeter, NULL, 0);
    ASSERT(len < 0);
    ASSERT(hello_has_error());

    hello_greeter_destroy(greeter);
}

/* ============================================================
 * Tests - Get/Set Name
 * ============================================================ */

TEST(get_name) {
    Hello_GreeterConfig config = HELLO_GREETER_CONFIG_DEFAULT;
    config.name = "TestName";

    Hello_Greeter *greeter = hello_greeter_create(&config);
    ASSERT_NOT_NULL(greeter);
    ASSERT_STR_EQ("TestName", hello_greeter_get_name(greeter));
    hello_greeter_destroy(greeter);
}

TEST(get_name_null_greeter) {
    const char *name = hello_greeter_get_name(NULL);
    ASSERT_NULL(name);
    ASSERT(hello_has_error());
}

TEST(set_name) {
    Hello_Greeter *greeter = hello_greeter_create(NULL);
    ASSERT_NOT_NULL(greeter);

    ASSERT(hello_greeter_set_name(greeter, "NewName"));
    ASSERT_STR_EQ("NewName", hello_greeter_get_name(greeter));

    hello_greeter_destroy(greeter);
}

TEST(set_name_empty_fails) {
    Hello_Greeter *greeter = hello_greeter_create(NULL);
    ASSERT_NOT_NULL(greeter);

    ASSERT(!hello_greeter_set_name(greeter, ""));
    ASSERT(hello_has_error());

    // Name should be unchanged
    ASSERT_STR_EQ("World", hello_greeter_get_name(greeter));

    hello_greeter_destroy(greeter);
}

/* ============================================================
 * Tests - Error Handling
 * ============================================================ */

TEST(error_initially_clear) {
    hello_clear_error();
    ASSERT(!hello_has_error());
    ASSERT_STR_EQ("", hello_get_last_error());
}

TEST(error_set_and_get) {
    hello_set_error("Test error %d", 42);
    ASSERT(hello_has_error());
    ASSERT_STR_EQ("Test error 42", hello_get_last_error());
}

TEST(error_clear) {
    hello_set_error("Some error");
    ASSERT(hello_has_error());

    hello_clear_error();
    ASSERT(!hello_has_error());
}

/* ============================================================
 * Tests - Utility
 * ============================================================ */

TEST(version) {
    const char *version = hello_get_version();
    ASSERT_NOT_NULL(version);
    ASSERT(strlen(version) > 0);
}

/* ============================================================
 * Main
 * ============================================================ */

int main(void) {
    printf("Hello Library Tests\n");
    printf("====================\n\n");

    printf("Lifecycle tests:\n");
    RUN_TEST(create_default);
    RUN_TEST(create_custom_name);
    RUN_TEST(create_empty_name_fails);
    RUN_TEST(destroy_null_safe);

    printf("\nGreet tests:\n");
    RUN_TEST(greet_default);
    RUN_TEST(greet_custom);
    RUN_TEST(greet_uppercase);
    RUN_TEST(greet_null_greeter_fails);
    RUN_TEST(greet_null_buffer_fails);

    printf("\nGet/Set name tests:\n");
    RUN_TEST(get_name);
    RUN_TEST(get_name_null_greeter);
    RUN_TEST(set_name);
    RUN_TEST(set_name_empty_fails);

    printf("\nError handling tests:\n");
    RUN_TEST(error_initially_clear);
    RUN_TEST(error_set_and_get);
    RUN_TEST(error_clear);

    printf("\nUtility tests:\n");
    RUN_TEST(version);

    printf("\n====================\n");
    printf("Results: %d/%d passed", g_tests_passed, g_tests_run);

    g_tests_failed = g_tests_run - g_tests_passed;
    if (g_tests_failed > 0) {
        printf(" (%d FAILED)\n", g_tests_failed);
        return EXIT_FAILURE;
    }

    printf("\n");
    return EXIT_SUCCESS;
}
