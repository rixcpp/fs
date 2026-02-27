#include <rix/fs/fs.hpp>
#include <iostream>

int main()
{
  using namespace rix::fs;

  try
  {
    const std::filesystem::path dir = ".";

    std::cout << "Listing directory: " << dir << "\n";
    std::cout << "---------------------------------\n";

    const auto entries = list_dir(dir);

    for (const auto &entry : entries)
    {
      if (is_directory(entry))
      {
        std::cout << "[DIR ] ";
      }
      else if (is_regular_file(entry))
      {
        std::cout << "[FILE] ";
      }
      else
      {
        std::cout << "[OTHR] ";
      }

      std::cout << entry.filename().string() << "\n";
    }

    std::cout << "---------------------------------\n";
    std::cout << "Total entries: " << entries.size() << "\n";
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
