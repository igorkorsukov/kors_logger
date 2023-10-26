/*
MIT License

Copyright (c) 2020 Igor Korsukov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "logdefdest.h"

#include <iostream>
#include <cassert>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#error compiler must either support c++17
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace kors::logger;

MemLogDest::MemLogDest(const LogLayout& l)
    : LogDest(l)
{
}

std::string MemLogDest::name() const
{
    return "MemLogDest";
}

void MemLogDest::write(const LogMsg& logMsg)
{
    m_stream << m_layout.output(logMsg) << std::endl;
}

std::string MemLogDest::content() const
{
    return m_stream.str();
}

// FileLogDest
FileLogDest::FileLogDest(const std::string& path, const std::string& name, const std::string& ext, const LogLayout& l)
    : LogDest(l), m_path(path), m_name(name), m_ext(ext)
{
    rotate();
}

FileLogDest::~FileLogDest()
{
    if (m_file.is_open()) {
        m_file.close();
    }
}

std::string FileLogDest::name() const
{
    return "FileLogDest";
}

void FileLogDest::rotate()
{
    if (m_file.is_open()) {
        m_file.close();
    }

    fs::path path = m_path;
    if (!fs::exists(path)) {
        bool ok = fs::create_directories(path);
        assert(ok);
        if (!ok) {
            std::clog << "failed create dir: " << path << std::endl;
            return;
        }
    }

    auto formatDate = [](const Date& d) {
        std::string str;
        str.reserve(10);

        str.append(std::to_string(d.year + 1900));

        if (d.mon < 11) {
            str.push_back('0');
        }
        str.append(std::to_string(d.mon + 1));

        if (d.day < 10) {
            str.push_back('0');
        }
        str.append(std::to_string(d.day));

        return str;
    };

    m_rotateDate = DateTime::now().date;
    std::string dateStr = formatDate(m_rotateDate);
    std::string filePath = m_path + "/" + m_name + "-" + dateStr + "." + m_ext;

    m_file.open(filePath, std::ios_base::out | std::ios_base::app);
    if (!m_file.is_open()) {
        std::clog << "failed open log file: " << path << std::endl;
    }
}

void FileLogDest::write(const LogMsg& logMsg)
{
    if (m_rotateDate != logMsg.datetime.date) {
        rotate();
    }

    m_file << m_layout.output(logMsg) << std::endl;
    m_file.flush();
}

// OutputDest
ConsoleLogDest::ConsoleLogDest(const LogLayout& l)
    : LogDest(l)
{
}

std::string ConsoleLogDest::name() const
{
    return "ConsoleLogDest";
}

void ConsoleLogDest::write(const LogMsg& logMsg)
{
    auto colorCode = [](Color c){
        switch (c) {
        case Color::None:    return "\033[0m";
        case Color::Black:   return "\033[1;30m";
        case Color::Red:     return "\033[1;31m";
        case Color::Green:   return "\033[1;32m";
        case Color::Yellow:  return "\033[1;33m";
        case Color::Blue:    return "\033[1;34m";
        case Color::Magenta: return "\033[1;35m";
        case Color::Cyan:    return "\033[1;36m";
        case Color::White:   return "\033[1;37m";
        }
        return "\033[0m";
    };

    std::string msg;
    msg.reserve(100);
    msg.append(colorCode(logMsg.color));
    msg.append(m_layout.output(logMsg));
    msg.append("\033[0m");
#ifdef _WIN32
    std::wstring temp = std::wstring(msg.begin(), msg.end());
    OutputDebugString(temp.c_str());
    OutputDebugString(L"\n");
#else
    std::cout << msg << std::endl;
#endif
}
