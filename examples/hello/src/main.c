/**
 * @file main.c
 * @brief Example program using the Hello library.
 */

#include "hello/hello.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    (void)argc;  // Unused
    (void)argv;  // Unused

    printf("Hello Library v%s\n", hello_get_version());
    printf("====================\n\n");

    // Example 1: Default configuration
    printf("Example 1: Default greeter\n");
    {
        Hello_Greeter *greeter = hello_greeter_create(NULL);
        if (!greeter) {
            fprintf(stderr, "Error: %s\n", hello_get_last_error());
            return EXIT_FAILURE;
        }

        char buffer[128];
        int len = hello_greeter_greet(greeter, buffer, sizeof(buffer));
        if (len >= 0) {
            printf("  %s\n", buffer);
        }

        hello_greeter_destroy(greeter);
    }

    // Example 2: Custom configuration
    printf("\nExample 2: Custom greeter\n");
    {
        Hello_GreeterConfig config = HELLO_GREETER_CONFIG_DEFAULT;
        config.name = "Carbide User";
        config.greeting = "Welcome";

        Hello_Greeter *greeter = hello_greeter_create(&config);
        if (!greeter) {
            fprintf(stderr, "Error: %s\n", hello_get_last_error());
            return EXIT_FAILURE;
        }

        char buffer[128];
        int len = hello_greeter_greet(greeter, buffer, sizeof(buffer));
        if (len >= 0) {
            printf("  %s\n", buffer);
        }

        hello_greeter_destroy(greeter);
    }

    // Example 3: Uppercase mode
    printf("\nExample 3: Uppercase greeter\n");
    {
        Hello_GreeterConfig config = HELLO_GREETER_CONFIG_DEFAULT;
        config.uppercase = true;

        Hello_Greeter *greeter = hello_greeter_create(&config);
        if (!greeter) {
            fprintf(stderr, "Error: %s\n", hello_get_last_error());
            return EXIT_FAILURE;
        }

        char buffer[128];
        hello_greeter_greet(greeter, buffer, sizeof(buffer));
        printf("  %s\n", buffer);

        hello_greeter_destroy(greeter);
    }

    // Example 4: Modifying name after creation
    printf("\nExample 4: Changing name\n");
    {
        Hello_Greeter *greeter = hello_greeter_create(NULL);
        if (!greeter) {
            fprintf(stderr, "Error: %s\n", hello_get_last_error());
            return EXIT_FAILURE;
        }

        char buffer[128];

        printf("  Before: name = \"%s\"\n", hello_greeter_get_name(greeter));
        hello_greeter_greet(greeter, buffer, sizeof(buffer));
        printf("  Greeting: %s\n", buffer);

        if (hello_greeter_set_name(greeter, "New Name")) {
            printf("  After: name = \"%s\"\n", hello_greeter_get_name(greeter));
            hello_greeter_greet(greeter, buffer, sizeof(buffer));
            printf("  Greeting: %s\n", buffer);
        }

        hello_greeter_destroy(greeter);
    }

    // Example 5: Error handling
    printf("\nExample 5: Error handling\n");
    {
        // Try to create with empty name
        Hello_GreeterConfig config = HELLO_GREETER_CONFIG_DEFAULT;
        config.name = "";

        Hello_Greeter *greeter = hello_greeter_create(&config);
        if (!greeter) {
            printf("  Expected error: %s\n", hello_get_last_error());
            hello_clear_error();
        } else {
            hello_greeter_destroy(greeter);
        }
    }

    printf("\nAll examples completed successfully!\n");
    return EXIT_SUCCESS;
}
