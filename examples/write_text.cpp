#include <rix/fs/fs.hpp>
#include <iostream>

int main()
{
  using namespace rix::fs;

  try
  {
    const std::filesystem::path p = "write_example.txt";

    std::cout << "Writing to file: " << p << "\n";

    write_text(p, "First line written with rix::fs\n");
    append_text(p, "Second line appended.\n");
    append_text(p, "Third line appended.\n");

    std::cout << "Done.\n";
    std::cout << "Final content:\n";
    std::cout << "---------------------------------\n";
    std::cout << read_text(p);
    std::cout << "---------------------------------\n";
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
