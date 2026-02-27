/**
 * @file util.hpp
 * @brief Miscellaneous filesystem utilities built on top of std::filesystem.
 *
 * Copyright 2026, Gaspard Kirira.
 * https://github.com/rixcpp/rix
 *
 * Use of this source code is governed by the MIT license.
 */

#ifndef RIX_FS_UTIL_HPP
#define RIX_FS_UTIL_HPP

#include <filesystem>
#include <stdexcept>
#include <string>
#include <system_error>

namespace rix::fs
{
  /**
   * @brief Return the current working directory.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline std::filesystem::path current_path()
  {
    std::error_code ec;
    auto p = std::filesystem::current_path(ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::current_path: failed");
    }
    return p;
  }

  /**
   * @brief Set the current working directory.
   *
   * @throws std::system_error on filesystem error.
   */
  inline void set_current_path(const std::filesystem::path &p)
  {
    std::error_code ec;
    std::filesystem::current_path(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::set_current_path: failed: " + p.string());
    }
  }

  /**
   * @brief Return the absolute version of a path (lexical).
   *
   * Does not resolve symlinks.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline std::filesystem::path absolute(const std::filesystem::path &p)
  {
    std::error_code ec;
    auto abs = std::filesystem::absolute(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::absolute: failed: " + p.string());
    }
    return abs;
  }

  /**
   * @brief Return the canonical path (resolves symlinks).
   *
   * Path must exist.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline std::filesystem::path canonical(const std::filesystem::path &p)
  {
    std::error_code ec;
    auto c = std::filesystem::canonical(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::canonical: failed: " + p.string());
    }
    return c;
  }

  /**
   * @brief Return the weakly canonical path.
   *
   * Unlike canonical, does not require the entire path to exist.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline std::filesystem::path weakly_canonical(const std::filesystem::path &p)
  {
    std::error_code ec;
    auto c = std::filesystem::weakly_canonical(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::weakly_canonical: failed: " + p.string());
    }
    return c;
  }

  /**
   * @brief Return the system temporary directory.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline std::filesystem::path temp_directory()
  {
    std::error_code ec;
    auto p = std::filesystem::temp_directory_path(ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::temp_directory: failed");
    }
    return p;
  }

  /**
   * @brief Check whether two paths are equivalent (same file).
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline bool equivalent(const std::filesystem::path &a,
                                       const std::filesystem::path &b)
  {
    std::error_code ec;
    const bool eq = std::filesystem::equivalent(a, b, ec);
    if (ec)
    {
      throw std::system_error(ec,
                              "rix::fs::equivalent: failed: " +
                                  a.string() + " <-> " + b.string());
    }
    return eq;
  }

} // namespace rix::fs

#endif // RIX_FS_UTIL_HPP
