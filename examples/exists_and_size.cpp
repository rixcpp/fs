#include <rix/fs/fs.hpp>
#include <iostream>

int main()
{
  using namespace rix::fs;

  try
  {
    const std::filesystem::path p = "example.txt";

    if (!path_exists(p))
    {
      std::cout << "File does not exist. Creating it...\n";
      write_text(p, "Hello from rix::fs\n");
    }

    if (is_file_path(p))
    {
      const auto sz = file_size_bytes(p);
      std::cout << "File: " << p << "\n";
      std::cout << "Size: " << sz << " bytes\n";
    }
    else
    {
      std::cout << p << " is not a regular file.\n";
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
