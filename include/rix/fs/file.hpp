/**
 * @file file.hpp
 * @brief Small, explicit file helpers built on top of std::filesystem.
 *
 * Notes:
 * - Names are chosen to avoid ambiguity with std::filesystem (exists, file_size, is_directory...).
 * - Text I/O reads and writes bytes as-is (no encoding conversion).
 *
 * Copyright 2026, Gaspard Kirira.
 * https://github.com/rixcpp/rix
 *
 * Use of this source code is governed by the MIT license.
 */

#ifndef RIX_FS_FILE_HPP
#define RIX_FS_FILE_HPP

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace rix::fs
{
  /**
   * @brief Whether a path exists.
   *
   * Returns false on errors.
   */
  [[nodiscard]] inline bool path_exists(const std::filesystem::path &p) noexcept
  {
    std::error_code ec;
    return std::filesystem::exists(p, ec);
  }

  /**
   * @brief Whether a path refers to a regular file.
   *
   * Returns false on errors.
   */
  [[nodiscard]] inline bool is_file_path(const std::filesystem::path &p) noexcept
  {
    std::error_code ec;
    return std::filesystem::is_regular_file(p, ec);
  }

  /**
   * @brief Whether a path refers to a directory.
   *
   * Returns false on errors.
   */
  [[nodiscard]] inline bool is_dir_path(const std::filesystem::path &p) noexcept
  {
    std::error_code ec;
    return std::filesystem::is_directory(p, ec);
  }

  /**
   * @brief Return file size in bytes.
   *
   * @throws std::system_error on filesystem error.
   * @throws std::runtime_error if the path is not a regular file.
   */
  [[nodiscard]] inline std::uintmax_t file_size_bytes(const std::filesystem::path &p)
  {
    std::error_code ec;
    const bool ok_file = std::filesystem::is_regular_file(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::file_size_bytes: stat failed: " + p.string());
    }
    if (!ok_file)
    {
      throw std::runtime_error("rix::fs::file_size_bytes: not a regular file: " + p.string());
    }

    const auto sz = std::filesystem::file_size(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::file_size_bytes: file_size failed: " + p.string());
    }
    return sz;
  }

  /**
   * @brief Read entire file as text (bytes as-is).
   *
   * @throws std::system_error if open fails (errno).
   * @throws std::runtime_error if reading fails.
   */
  [[nodiscard]] inline std::string read_text(const std::filesystem::path &p)
  {
    std::ifstream in(p);
    if (!in.is_open())
    {
      throw std::system_error(errno, std::generic_category(), "rix::fs::read_text: open failed: " + p.string());
    }

    std::string content;
    content.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());

    if (!in.eof() && in.fail())
    {
      throw std::runtime_error("rix::fs::read_text: read failed: " + p.string());
    }

    return content;
  }

  /**
   * @brief Read entire file as bytes.
   *
   * @throws std::system_error if open fails (errno).
   * @throws std::runtime_error if reading fails.
   */
  [[nodiscard]] inline std::vector<std::byte> read_bytes(const std::filesystem::path &p)
  {
    std::ifstream in(p, std::ios::binary);
    if (!in.is_open())
    {
      throw std::system_error(errno, std::generic_category(), "rix::fs::read_bytes: open failed: " + p.string());
    }

    in.seekg(0, std::ios::end);
    const std::streampos end = in.tellg();
    in.seekg(0, std::ios::beg);
    const std::streampos begin = in.tellg();

    if (end < begin)
    {
      throw std::runtime_error("rix::fs::read_bytes: invalid size: " + p.string());
    }

    const auto size = static_cast<std::size_t>(end - begin);
    std::vector<std::byte> buf(size);

    if (size != 0)
    {
      in.read(reinterpret_cast<char *>(buf.data()), static_cast<std::streamsize>(size));

      if (static_cast<std::size_t>(in.gcount()) != size || in.fail())
      {
        throw std::runtime_error("rix::fs::read_bytes: read failed: " + p.string());
      }
    }

    return buf;
  }

  /**
   * @brief Write text to file (truncate).
   *
   * @throws std::system_error if open fails (errno).
   * @throws std::runtime_error if writing fails.
   */
  inline void write_text(const std::filesystem::path &p, std::string_view text)
  {
    std::ofstream out(p, std::ios::out | std::ios::trunc);
    if (!out.is_open())
    {
      throw std::system_error(errno, std::generic_category(), "rix::fs::write_text: open failed: " + p.string());
    }

    out.write(text.data(), static_cast<std::streamsize>(text.size()));
    if (!out)
    {
      throw std::runtime_error("rix::fs::write_text: write failed: " + p.string());
    }
  }

  /**
   * @brief Append text to file (create if missing).
   *
   * @throws std::system_error if open fails (errno).
   * @throws std::runtime_error if writing fails.
   */
  inline void append_text(const std::filesystem::path &p, std::string_view text)
  {
    std::ofstream out(p, std::ios::out | std::ios::app);
    if (!out.is_open())
    {
      throw std::system_error(errno, std::generic_category(), "rix::fs::append_text: open failed: " + p.string());
    }

    out.write(text.data(), static_cast<std::streamsize>(text.size()));
    if (!out)
    {
      throw std::runtime_error("rix::fs::append_text: write failed: " + p.string());
    }
  }

  /**
   * @brief Write bytes to file (truncate).
   *
   * @throws std::system_error if open fails (errno).
   * @throws std::runtime_error if writing fails.
   */
  inline void write_bytes(const std::filesystem::path &p, std::span<const std::byte> bytes)
  {
    std::ofstream out(p, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!out.is_open())
    {
      throw std::system_error(errno, std::generic_category(), "rix::fs::write_bytes: open failed: " + p.string());
    }

    if (!bytes.empty())
    {
      out.write(reinterpret_cast<const char *>(bytes.data()),
                static_cast<std::streamsize>(bytes.size()));
    }

    if (!out)
    {
      throw std::runtime_error("rix::fs::write_bytes: write failed: " + p.string());
    }
  }

  /**
   * @brief Copy a file.
   *
   * @throws std::system_error on filesystem error.
   * @throws std::runtime_error if copy did not happen (e.g. destination exists and overwrite=false).
   */
  inline void copy_file(const std::filesystem::path &from,
                        const std::filesystem::path &to,
                        bool overwrite = false)
  {
    std::error_code ec;

    const auto opt = overwrite
                         ? std::filesystem::copy_options::overwrite_existing
                         : std::filesystem::copy_options::none;

    const bool ok = std::filesystem::copy_file(from, to, opt, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::copy_file: failed: " + from.string() + " -> " + to.string());
    }
    if (!ok)
    {
      throw std::runtime_error("rix::fs::copy_file: not copied: " + from.string() + " -> " + to.string());
    }
  }

  /**
   * @brief Remove a file.
   *
   * Returns true if a file was removed.
   *
   * @throws std::system_error on filesystem error.
   */
  [[nodiscard]] inline bool remove_file(const std::filesystem::path &p)
  {
    std::error_code ec;
    const bool removed = std::filesystem::remove(p, ec);
    if (ec)
    {
      throw std::system_error(ec, "rix::fs::remove_file: failed: " + p.string());
    }
    return removed;
  }

} // namespace rix::fs

#endif // RIX_FS_FILE_HPP
