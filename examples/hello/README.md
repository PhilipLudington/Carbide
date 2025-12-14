# Hello - Carbide Example Project

A minimal example demonstrating Carbide patterns for safe, maintainable C code.

## Features Demonstrated

- **Opaque types** with create/destroy lifecycle
- **Config structs** with default macros
- **Thread-local error handling**
- **Const correctness**
- **Comprehensive testing**
- **Carbide Makefile** with all standard targets

## Building

```bash
# Build the project
make build

# Run the example
make run

# Run tests (with sanitizers)
make test

# Run static analysis
make check

# Run security checks
make safety

# Format code
make format

# Clean build artifacts
make clean
```

## Project Structure

```
hello/
├── include/hello/
│   └── hello.h          # Public API header
├── src/
│   ├── hello.c          # Library implementation
│   └── main.c           # Example program
├── tests/
│   └── test_main.c      # Unit tests
├── Makefile             # Build system
└── README.md            # This file
```

## API Overview

```c
// Create a greeter with default settings
Hello_Greeter *greeter = hello_greeter_create(NULL);

// Or with custom configuration
Hello_GreeterConfig config = HELLO_GREETER_CONFIG_DEFAULT;
config.name = "Developer";
config.greeting = "Welcome";
Hello_Greeter *greeter = hello_greeter_create(&config);

// Generate greeting
char buffer[128];
hello_greeter_greet(greeter, buffer, sizeof(buffer));
printf("%s\n", buffer);  // "Welcome, Developer!"

// Clean up
hello_greeter_destroy(greeter);
```

## Error Handling

```c
Hello_Greeter *greeter = hello_greeter_create(NULL);
if (!greeter) {
    fprintf(stderr, "Error: %s\n", hello_get_last_error());
    return EXIT_FAILURE;
}
```

## License

MIT License - See repository root for details.
