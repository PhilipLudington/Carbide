/**
 * @file hello.c
 * @brief Hello library implementation.
 */

#include "hello/hello.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * Constants
 * ============================================================ */

#define HELLO_VERSION "1.0.0"
#define ERROR_BUFFER_SIZE 1024
#define MAX_NAME_LENGTH 256
#define MAX_GREETING_LENGTH 64

/* ============================================================
 * Private Types
 * ============================================================ */

struct Hello_Greeter {
    char *name;
    char *greeting;
    bool uppercase;
};

/* ============================================================
 * Thread-Local Error Handling
 * ============================================================ */

static _Thread_local char g_error_buffer[ERROR_BUFFER_SIZE];
static _Thread_local bool g_has_error = false;

__attribute__((format(printf, 1, 2)))
void hello_set_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    (void)vsnprintf(g_error_buffer, sizeof(g_error_buffer), fmt, args);
    va_end(args);
    g_has_error = true;
}

const char *hello_get_last_error(void) {
    return g_has_error ? g_error_buffer : "";
}

bool hello_has_error(void) {
    return g_has_error;
}

void hello_clear_error(void) {
    g_has_error = false;
    g_error_buffer[0] = '\0';
}

/* ============================================================
 * Private Functions
 * ============================================================ */

/**
 * Duplicate a string, returning NULL on failure.
 */
static char *safe_strdup(const char *str) {
    if (!str) return NULL;

    size_t len = strlen(str);
    char *copy = malloc(len + 1);
    if (!copy) {
        hello_set_error("Failed to allocate string of length %zu", len);
        return NULL;
    }

    memcpy(copy, str, len + 1);
    return copy;
}

/**
 * Convert string to uppercase in place.
 */
static void str_to_upper(char *str) {
    if (!str) return;
    for (char *p = str; *p; p++) {
        *p = (char)toupper((unsigned char)*p);
    }
}

/**
 * Validate name length.
 */
static bool validate_name(const char *name) {
    if (!name) {
        hello_set_error("Name cannot be NULL");
        return false;
    }

    size_t len = strlen(name);
    if (len == 0) {
        hello_set_error("Name cannot be empty");
        return false;
    }

    if (len >= MAX_NAME_LENGTH) {
        hello_set_error("Name too long (%zu chars, max %d)",
                        len, MAX_NAME_LENGTH - 1);
        return false;
    }

    return true;
}

/* ============================================================
 * Public Functions - Lifecycle
 * ============================================================ */

Hello_Greeter *hello_greeter_create(const Hello_GreeterConfig *config) {
    // Use default config if none provided
    Hello_GreeterConfig default_config = HELLO_GREETER_CONFIG_DEFAULT;
    if (!config) {
        config = &default_config;
    }

    // Validate configuration
    const char *name = config->name ? config->name : "World";
    const char *greeting = config->greeting ? config->greeting : "Hello";

    if (!validate_name(name)) {
        return NULL;  // Error already set
    }

    // Allocate greeter
    Hello_Greeter *greeter = calloc(1, sizeof(Hello_Greeter));
    if (!greeter) {
        hello_set_error("Failed to allocate Hello_Greeter");
        return NULL;
    }

    // Copy name
    greeter->name = safe_strdup(name);
    if (!greeter->name) {
        free(greeter);
        return NULL;  // Error already set
    }

    // Copy greeting
    greeter->greeting = safe_strdup(greeting);
    if (!greeter->greeting) {
        free(greeter->name);
        free(greeter);
        return NULL;  // Error already set
    }

    greeter->uppercase = config->uppercase;

    return greeter;
}

void hello_greeter_destroy(Hello_Greeter *greeter) {
    if (!greeter) return;

    free(greeter->name);
    free(greeter->greeting);
    free(greeter);
}

/* ============================================================
 * Public Functions - Operations
 * ============================================================ */

int hello_greeter_greet(const Hello_Greeter *greeter,
                        char *buffer,
                        size_t buffer_size) {
    if (!greeter) {
        hello_set_error("greeter is NULL");
        return -1;
    }

    if (!buffer || buffer_size == 0) {
        hello_set_error("Invalid output buffer");
        return -1;
    }

    // Format greeting
    int written = snprintf(buffer, buffer_size, "%s, %s!",
                           greeter->greeting, greeter->name);

    if (written < 0) {
        hello_set_error("Formatting failed");
        return -1;
    }

    if ((size_t)written >= buffer_size) {
        hello_set_error("Buffer too small (need %d, have %zu)",
                        written + 1, buffer_size);
        // Still return required size (excluding null terminator)
    }

    // Apply uppercase if configured
    if (greeter->uppercase) {
        str_to_upper(buffer);
    }

    return written;
}

const char *hello_greeter_get_name(const Hello_Greeter *greeter) {
    if (!greeter) {
        hello_set_error("greeter is NULL");
        return NULL;
    }
    return greeter->name;
}

bool hello_greeter_set_name(Hello_Greeter *greeter, const char *name) {
    if (!greeter) {
        hello_set_error("greeter is NULL");
        return false;
    }

    if (!validate_name(name)) {
        return false;  // Error already set
    }

    char *new_name = safe_strdup(name);
    if (!new_name) {
        return false;  // Error already set
    }

    free(greeter->name);
    greeter->name = new_name;

    return true;
}

/* ============================================================
 * Public Functions - Utility
 * ============================================================ */

const char *hello_get_version(void) {
    return HELLO_VERSION;
}
