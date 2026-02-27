/**
 * @file ops.hpp
 * @brief Higher-level filesystem operations (move, rename, recursive copy).
 *
 * Copyright 2026, Gaspard Kirira.
 * https://github.com/rixcpp/rix
 *
 * Use of this source code is governed by the MIT license.
 */

#ifndef RIX_FS_OPS_HPP
#define RIX_FS_OPS_HPP

#include <filesystem>
#include <stdexcept>
#include <string>
#include <system_error>

namespace rix::fs
{
  /**
   * @brief Rename or move a file or directory.
   *
   * Equivalent to std::filesystem::rename.
   *
   * @throws std::system_error on filesystem error.
   */
  inline void move(const std::filesystem::path &from,
                   const std::filesystem::path &to)
  {
    std::error_code ec;
    std::filesystem::rename(from, to, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::move: failed: " + from.string() + " -> " + to.string());
    }
  }

  /**
   * @brief Rename a path.
   *
   * Alias of move for semantic clarity.
   */
  inline void rename(const std::filesystem::path &from,
                     const std::filesystem::path &to)
  {
    move(from, to);
  }

  /**
   * @brief Recursively copy file or directory.
   *
   * If `overwrite` is true, existing files are replaced.
   *
   * @throws std::system_error on filesystem error.
   */
  inline void recursive_copy(const std::filesystem::path &from,
                             const std::filesystem::path &to,
                             bool overwrite = false)
  {
    std::error_code ec;

    auto options = std::filesystem::copy_options::recursive;

    if (overwrite)
    {
      options |= std::filesystem::copy_options::overwrite_existing;
    }

    std::filesystem::copy(from, to, options, ec);
    if (ec)
    {
      throw std::system_error(ec,
                              "rix::fs::recursive_copy: failed: " +
                                  from.string() + " -> " + to.string());
    }
  }

  /**
   * @brief Remove a file or directory tree recursively.
   *
   * Returns number of removed entries.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline std::uintmax_t recursive_remove(const std::filesystem::path &p)
  {
    std::error_code ec;
    const auto count = std::filesystem::remove_all(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::recursive_remove: failed: " + p.string());
    }
    return count;
  }

  /**
   * @brief Ensure directory exists (create if missing).
   *
   * Safe for concurrent repeated calls.
   *
   * @throws std::system_error on filesystem error.
   */
  inline void ensure_dir(const std::filesystem::path &p)
  {
    std::error_code ec;

    if (std::filesystem::exists(p, ec))
    {
      if (ec)
      {
        throw std::system_error(ec, "rix::fs::ensure_dir: exists failed: " + p.string());
      }
      if (!std::filesystem::is_directory(p, ec))
      {
        throw std::runtime_error("rix::fs::ensure_dir: not a directory: " + p.string());
      }
      return;
    }

    if (!std::filesystem::create_directories(p, ec) && ec)
    {
      throw std::system_error(ec, "rix::fs::ensure_dir: create failed: " + p.string());
    }
  }

} // namespace rix::fs

#endif // RIX_FS_OPS_HPP
