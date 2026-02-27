#include <rix/fs/fs.hpp>
#include <cassert>
#include <iostream>

int main()
{
  using namespace rix::fs;

  try
  {
    const auto tmp_root = temp_directory() / "rix_fs_test_dir";
    const auto file_path = tmp_root / "hello.txt";

    // Clean previous run if any
    if (path_exists(tmp_root))
    {
      recursive_remove(tmp_root);
    }

    // ---- create directory ----
    assert(!path_exists(tmp_root));
    ensure_dir(tmp_root);
    assert(path_exists(tmp_root));
    assert(is_dir_path(tmp_root));

    // ---- write & read text ----
    write_text(file_path, "hello rix");
    assert(path_exists(file_path));
    assert(is_file_path(file_path));

    const auto content = read_text(file_path);
    assert(content == "hello rix");

    // ---- file size ----
    const auto sz = file_size_bytes(file_path);
    assert(sz == content.size());

    // ---- append ----
    append_text(file_path, "\nworld");
    const auto content2 = read_text(file_path);
    assert(content2 == "hello rix\nworld");

    // ---- copy ----
    const auto copy_path = tmp_root / "copy.txt";
    copy_file(file_path, copy_path, true);
    assert(path_exists(copy_path));
    assert(read_text(copy_path) == content2);

    // ---- move ----
    const auto moved_path = tmp_root / "moved.txt";
    move(copy_path, moved_path);
    assert(!path_exists(copy_path));
    assert(path_exists(moved_path));

    // ---- list dir ----
    const auto entries = list_dir(tmp_root);
    assert(!entries.empty());

    // ---- cleanup ----
    const auto removed_count = recursive_remove(tmp_root);
    assert(removed_count > 0);
    assert(!path_exists(tmp_root));

    std::cout << "rix::fs basic test passed\n";
  }
  catch (const std::exception &e)
  {
    std::cerr << "Test failed: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
