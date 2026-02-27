# fs

Minimal modern filesystem utilities for C++20.

`fs` provides deterministic wrappers around `std::filesystem`
with explicit error handling and zero hidden behavior.

Header-only by default. Zero external dependencies.

---

## Download

https://github.com/rixcpp/rix

(Part of the Rix modular utility library)

---

## Why fs?

Filesystem access is required in:

- Configuration loading
- Asset management
- Build systems
- Logging systems
- Temporary file handling
- CLI tooling
- Deployment scripts

This module provides:

- Path normalization and helpers
- Safe existence checks
- Explicit file read/write
- Directory listing
- Recursive operations
- Canonical path resolution
- Deterministic error handling

No implicit encoding.
No hidden symlink resolution.
No silent failures.

Just minimal deterministic filesystem tools.

---

## Installation

### Using Rix (umbrella)

```cmake
find_package(rix REQUIRED)
target_link_libraries(my_app PRIVATE rix::fs)
```

### Using rix::all

```cmake
target_link_libraries(my_app PRIVATE rix::all)
```

### Manual

```bash
git clone https://github.com/rixcpp/rix.git
```

Add the `include/` directory from `modules/fs` to your project.

---

## Quick Examples

### Check Path Existence

```cpp
#include <rix/fs/file.hpp>
#include <iostream>

int main()
{
    if (rix::fs::path_exists("data.txt"))
    {
        std::cout << "File exists\n";
    }
}
```

### Read / Write Text

```cpp
#include <rix/fs/file.hpp>
#include <iostream>

int main()
{
    rix::fs::write_text("example.txt", "hello");
    auto content = rix::fs::read_text("example.txt");

    std::cout << content << "\n";
}
```

### File Size

```cpp
#include <rix/fs/file.hpp>
#include <iostream>

int main()
{
    try
    {
        auto size = rix::fs::file_size_bytes("example.txt");
        std::cout << size << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }
}
```

### Directory Listing

```cpp
#include <rix/fs/dir.hpp>
#include <iostream>

int main()
{
    for (auto& entry : rix::fs::list_dir("."))
    {
        std::cout << entry << "\n";
    }
}
```

### Recursive Copy

```cpp
#include <rix/fs/ops.hpp>

int main()
{
    rix::fs::recursive_copy("assets", "backup_assets", true);
}
```

---

## API Overview

### Path Helpers (`rix::fs::path`)

```cpp
normalize(path);
join(a, b);
filename(path);
stem(path);
extension(path);
parent(path);
replace_extension(path, ext);
```

### File Helpers (`rix::fs::file`)

```cpp
path_exists(path);
is_file_path(path);
is_dir_path(path);
file_size_bytes(path);
read_text(path);
read_bytes(path);
write_text(path, text);
append_text(path, text);
write_bytes(path, span);
copy_file(from, to, overwrite);
remove_file(path);
```

### Directory Helpers

```cpp
create_dir(path);
create_dirs(path);
list_dir(path);
list_dir_recursive(path);
remove_dir(path);
remove_all(path);
```

### Operations

```cpp
move(from, to);
rename(from, to);
recursive_copy(from, to);
recursive_remove(path);
ensure_dir(path);
```

### Utilities

```cpp
current_path();
set_current_path(path);
absolute(path);
canonical(path);
weakly_canonical(path);
temp_directory();
equivalent(a, b);
```

---

## Design Principles

- Explicit over implicit
- No hidden fallback logic
- No ADL ambiguity with std
- Deterministic error propagation
- Modern C++20 only
- Minimal surface area

This module intentionally does not provide:

- Async file I/O
- File watchers
- Memory-mapped files
- Encoding detection
- Compression
- Platform abstraction layers

Build those on top.

---

## Error Handling

Non-throwing checks use `std::error_code`.

Throwing operations use:

- `std::system_error` for filesystem failures
- `std::runtime_error` for logical misuse

Behavior is explicit and predictable.

---

## Performance Notes

- Thin wrapper over `std::filesystem`
- No dynamic polymorphism
- No heap allocations beyond STL usage
- No hidden caching
- No global state

Designed for clarity and determinism.

---

## Tests

```bash
cmake --preset dev-ninja
cmake --build --preset dev-ninja
ctest --preset dev-ninja
```

Tests verify:

- Path helpers
- File read/write integrity
- Directory operations
- Recursive operations
- Exception correctness
- Cleanup correctness

---

## License

MIT License\
Copyright (c) Gaspard Kirira

