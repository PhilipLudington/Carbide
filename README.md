# Carbide

**Hardened C/C++ development standards and tooling for AI-assisted programming.**

Carbide helps developers—especially those new to C—write trustworthy, safe, and maintainable C code with AI assistance. It provides coding standards optimized for AI reasoning, validation tooling, and Claude Code slash commands that enforce best practices.

## Why Carbide?

C remains one of the most powerful languages for systems programming, game development, and performance-critical applications. However, C's flexibility comes with risks—memory corruption, undefined behavior, and security vulnerabilities are common pitfalls.

Carbide addresses this by:

1. **Standards designed for AI**: Clear, unambiguous rules that Claude Code can follow consistently
2. **Guardrails that prevent bad patterns**: Static analysis and security checks catch issues early
3. **Trust through validation**: Every check that passes builds confidence in generated code

## Quick Start

### Using Carbide Commands (No Setup Required)

These commands work on any C/C++ code:

```
/carbide-review path/to/file.c    # Review code against Carbide standards
/carbide-safety path/to/file.c    # Security-focused review
```

### Creating a Carbide Project

For full validation tooling:

```
/carbide-init my-project          # Create new project with full scaffold
cd my-project
make build                        # Compile
make check                        # Run static analysis
make safety                       # Run security checks
make test                         # Run tests with sanitizers
```

## What's Included

### Standards (`STANDARDS.md`)

Comprehensive coding standards covering:
- Naming conventions
- Memory management patterns
- Error handling
- API design
- Security practices

### Slash Commands

| Command | Mode | Description |
|---------|------|-------------|
| `/carbide-init` | Setup | Create a new Carbide project |
| `/carbide-review` | Standalone | Review code against standards |
| `/carbide-check` | Integrated | Run validation tooling |
| `/carbide-safety` | Standalone | Security-focused code review |

### Validation Tooling

- **Makefile** with standard targets (`build`, `check`, `safety`, `test`, `format`, `clean`)
- **clang-tidy** configuration with curated rules
- **clang-format** configuration for consistent style
- **Sanitizer presets** (AddressSanitizer, UndefinedBehaviorSanitizer)

### Pattern Documentation

Detailed guides in `docs/patterns/`:
- `memory.md` - Memory management patterns
- `errors.md` - Error handling patterns
- `api-design.md` - C API design patterns
- `resources.md` - Resource lifecycle patterns

### Security Documentation

Security-focused guides in `docs/security/`:
- `buffer-overflow.md` - Prevention techniques
- `injection.md` - Input validation patterns
- `memory-safety.md` - Safe memory practices

## Supported Toolchains

Carbide supports cross-platform development:

| Platform | Compilers |
|----------|-----------|
| macOS | Clang, GCC |
| Linux | GCC, Clang |
| Windows | MSVC, Clang, GCC (MinGW) |

## Design Philosophy

1. **Prescriptive over permissive**: Clear rules are easier to follow than vague guidelines
2. **Explicit over implicit**: Memory ownership, error states, and lifetimes should be obvious
3. **Validated over assumed**: If it can be checked automatically, it should be
4. **Simple over clever**: Readable code beats clever optimizations
5. **Secure by default**: Security patterns should be the path of least resistance

## Project Structure

```
Carbide/
├── README.md              # This file
├── CLAUDE.md              # AI development guidance
├── STANDARDS.md           # Coding standards
├── .claude/commands/      # Slash commands
├── templates/             # Project templates
│   ├── Makefile           # Build template
│   ├── .clang-tidy        # Static analysis config
│   ├── .clang-format      # Formatting config
│   └── project/           # Full scaffold template
├── docs/
│   ├── patterns/          # Coding pattern guides
│   └── security/          # Security guides
└── examples/              # Example projects
```

## Language Standards

Carbide targets:
- **C11** for C code (widely supported, modern features)
- **C++17** for C++ implementations (when wrapping C++ in C APIs)

## Contributing

Contributions welcome! Please read `CONTRIBUTING.md` before submitting PRs.

## License

MIT License - see `LICENSE` for details.

## Acknowledgments

Carbide was inspired by patterns from the [Agentite](https://github.com/example/agentite) game engine and developed collaboratively with Claude Code.
