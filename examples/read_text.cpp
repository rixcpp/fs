#include <rix/fs/fs.hpp>
#include <iostream>

int main()
{
  using namespace rix::fs;

  try
  {
    const std::filesystem::path p = "read_example.txt";

    if (!exists(p))
    {
      std::cout << "Creating file for demo...\n";
      write_text(p, "Hello from rix::fs read_text example.\nSecond line.\n");
    }

    const std::string content = read_text(p);

    std::cout << "Reading file: " << p << "\n";
    std::cout << "---------------------------------\n";
    std::cout << content;
    std::cout << "---------------------------------\n";
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
