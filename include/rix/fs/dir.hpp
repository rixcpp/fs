/**
 * @file dir.hpp
 * @brief Directory helpers built on top of std::filesystem.
 *
 * Copyright 2026, Gaspard Kirira.
 * https://github.com/rixcpp/rix
 *
 * Use of this source code is governed by the MIT license.
 */

#ifndef RIX_FS_DIR_HPP
#define RIX_FS_DIR_HPP

#include <filesystem>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

namespace rix::fs
{
  /**
   * @brief Create a directory.
   *
   * Returns true if the directory was created.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline bool create_dir(const std::filesystem::path &p)
  {
    std::error_code ec;
    const bool created = std::filesystem::create_directory(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::create_dir: failed: " + p.string());
    }
    return created;
  }

  /**
   * @brief Create a directory and all missing parent directories.
   *
   * Returns true if at least one directory was created.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline bool create_dirs(const std::filesystem::path &p)
  {
    std::error_code ec;
    const bool created = std::filesystem::create_directories(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::create_dirs: failed: " + p.string());
    }
    return created;
  }

  /**
   * @brief List entries (non-recursive) inside a directory.
   *
   * Returns a vector of paths (full paths).
   *
   * @throws std::runtime_error if path is not a directory.
   * @throws std::system_error on iteration error.
   */
  [[nodiscard]] inline std::vector<std::filesystem::path>
  list_dir(const std::filesystem::path &p)
  {
    std::error_code ec;
    const bool ok_dir = std::filesystem::is_directory(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::list_dir: stat failed: " + p.string());
    }
    if (!ok_dir)
    {
      throw std::runtime_error("rix::fs::list_dir: not a directory: " + p.string());
    }

    std::vector<std::filesystem::path> entries;

    for (const auto &entry : std::filesystem::directory_iterator(p, ec))
    {
      if (ec)
      {
        throw std::system_error(ec, "rix::fs::list_dir: iteration failed: " + p.string());
      }
      entries.push_back(entry.path());
    }

    return entries;
  }

  /**
   * @brief List entries recursively inside a directory.
   *
   * Returns a vector of paths (full paths).
   *
   * @throws std::runtime_error if path is not a directory.
   * @throws std::system_error on iteration error.
   */
  [[nodiscard]] inline std::vector<std::filesystem::path>
  list_dir_recursive(const std::filesystem::path &p)
  {
    std::error_code ec;
    const bool ok_dir = std::filesystem::is_directory(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::list_dir_recursive: stat failed: " + p.string());
    }
    if (!ok_dir)
    {
      throw std::runtime_error("rix::fs::list_dir_recursive: not a directory: " + p.string());
    }

    std::vector<std::filesystem::path> entries;

    for (const auto &entry : std::filesystem::recursive_directory_iterator(p, ec))
    {
      if (ec)
      {
        throw std::system_error(ec, "rix::fs::list_dir_recursive: iteration failed: " + p.string());
      }
      entries.push_back(entry.path());
    }

    return entries;
  }

  /**
   * @brief Remove an empty directory.
   *
   * Returns true if removed.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline bool remove_dir(const std::filesystem::path &p)
  {
    std::error_code ec;
    const bool removed = std::filesystem::remove(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::remove_dir: failed: " + p.string());
    }
    return removed;
  }

  /**
   * @brief Remove a directory tree recursively.
   *
   * Returns number of removed files/directories.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline std::uintmax_t remove_all(const std::filesystem::path &p)
  {
    std::error_code ec;
    const auto count = std::filesystem::remove_all(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::remove_all: failed: " + p.string());
    }
    return count;
  }

} // namespace rix::fs

#endif // RIX_FS_DIR_HPP
