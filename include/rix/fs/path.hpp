/**
 * @file path.hpp
 * @brief Small, explicit path helpers built on top of std::filesystem::path.
 *
 * Copyright 2026, Gaspard Kirira.
 * https://github.com/rixcpp/rix
 *
 * Use of this source code is governed by the MIT license.
 */

#ifndef RIX_FS_PATH_HPP
#define RIX_FS_PATH_HPP

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace rix::fs
{
  /**
   * @brief Normalize a path (lexical, no filesystem access).
   *
   * Equivalent to `p.lexically_normal()`.
   */
  [[nodiscard]] inline std::filesystem::path normalize(const std::filesystem::path &p)
  {
    return p.lexically_normal();
  }

  /**
   * @brief Join two paths.
   *
   * If `rhs` is absolute, the result becomes `rhs` (filesystem semantics).
   */
  [[nodiscard]] inline std::filesystem::path join(const std::filesystem::path &lhs,
                                                  const std::filesystem::path &rhs)
  {
    return lhs / rhs;
  }

  /**
   * @brief Join a base path with multiple segments.
   */
  [[nodiscard]] inline std::filesystem::path join(const std::filesystem::path &base,
                                                  const std::vector<std::filesystem::path> &segments)
  {
    std::filesystem::path out = base;
    for (const auto &s : segments)
    {
      out /= s;
    }
    return out;
  }

  /**
   * @brief Return the filename component.
   *
   * If empty, returns an empty string.
   */
  [[nodiscard]] inline std::string filename(const std::filesystem::path &p)
  {
    return p.filename().string();
  }

  /**
   * @brief Return the stem (filename without extension).
   */
  [[nodiscard]] inline std::string stem(const std::filesystem::path &p)
  {
    return p.stem().string();
  }

  /**
   * @brief Return the extension (including the leading dot).
   *
   * Example: ".txt"
   */
  [[nodiscard]] inline std::string extension(const std::filesystem::path &p)
  {
    return p.extension().string();
  }

  /**
   * @brief Return the parent directory.
   */
  [[nodiscard]] inline std::filesystem::path parent(const std::filesystem::path &p)
  {
    return p.parent_path();
  }

  /**
   * @brief Replace extension with `ext` and return the new path.
   *
   * `ext` may be "txt" or ".txt". Empty clears the extension.
   */
  [[nodiscard]] inline std::filesystem::path replace_extension(const std::filesystem::path &p,
                                                               std::string_view ext)
  {
    std::filesystem::path out = p;

    if (ext.empty())
    {
      out.replace_extension();
      return out;
    }

    if (!ext.empty() && ext.front() != '.')
    {
      std::string tmp;
      tmp.reserve(ext.size() + 1);
      tmp.push_back('.');
      tmp.append(ext.data(), ext.size());
      out.replace_extension(tmp);
      return out;
    }

    out.replace_extension(std::string(ext));
    return out;
  }

  /**
   * @brief Ensure the path ends with a trailing separator (lexical).
   *
   * If the path is empty, returns empty.
   */
  [[nodiscard]] inline std::filesystem::path ensure_trailing_separator(const std::filesystem::path &p)
  {
    if (p.empty())
    {
      return p;
    }

    auto s = p.generic_string();
    if (!s.empty() && s.back() != '/')
    {
      s.push_back('/');
    }
    return std::filesystem::path(s);
  }

  /**
   * @brief Split a path into lexical components.
   *
   * Example: "a/b/c" -> ["a","b","c"].
   */
  [[nodiscard]] inline std::vector<std::string> split(const std::filesystem::path &p)
  {
    std::vector<std::string> parts;
    for (const auto &comp : p)
    {
      const auto s = comp.string();
      if (!s.empty())
      {
        parts.push_back(s);
      }
    }
    return parts;
  }

  /**
   * @brief Convert a path to generic string form (uses '/' separators).
   */
  [[nodiscard]] inline std::string to_generic_string(const std::filesystem::path &p)
  {
    return p.generic_string();
  }

  /**
   * @brief Convert a path to native string form (platform dependent separators).
   */
  [[nodiscard]] inline std::string to_native_string(const std::filesystem::path &p)
  {
    return p.string();
  }

} // namespace rix::fs

#endif // RIX_FS_PATH_HPP
