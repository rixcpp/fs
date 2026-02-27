# Changelog

All notable changes to this module will be documented in this file.

The format follows Keep a Changelog principles and semantic versioning.

## [0.1.0] - Initial Release

### Added

#### Path Helpers

- `normalize(path)`
- `join(base, segment)`
- `filename(path)`
- `stem(path)`
- `extension(path)`
- `parent(path)`
- `replace_extension(path, ext)`
- `split(path)`
- `to_generic_string(path)`
- `to_native_string(path)`

#### File Utilities

- `path_exists(path)`
- `is_file_path(path)`
- `is_dir_path(path)`
- `file_size_bytes(path)`
- `read_text(path)`
- `read_bytes(path)`
- `write_text(path, text)`
- `append_text(path, text)`
- `write_bytes(path, bytes)`
- `copy_file(from, to, overwrite)`
- `remove_file(path)`

#### Directory Utilities

- `create_dir(path)`
- `create_dirs(path)`
- `list_dir(path)`
- `list_dir_recursive(path)`
- `remove_dir(path)`
- `remove_all(path)`

#### Operations

- `move(from, to)`
- `rename(from, to)`
- `recursive_copy(from, to)`
- `recursive_remove(path)`
- `ensure_dir(path)`

#### Misc Utilities

- `current_path()`
- `set_current_path(path)`
- `absolute(path)`
- `canonical(path)`
- `weakly_canonical(path)`
- `temp_directory()`
- `equivalent(a, b)`

---

## Design Notes

- Header-only implementation
- No external dependencies
- Built strictly on top of `std::filesystem`
- Explicit error handling
- No ADL ambiguity with standard library functions
- Modern C++20 only

---

End of initial release.
