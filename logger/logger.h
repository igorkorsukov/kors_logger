#ifndef KORS_LOGGER_H
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
#define KORS_LOGGER_H

#include <string>
#include <cstring>
#include <thread>
#include <vector>
#include <mutex>

#include "logstream.h"

namespace kors::logger {
//! Types
struct Ascii
{
    constexpr Ascii(const char* n)
        : m_size(n ? std::char_traits<char>::length(n) : 0), m_data(n) {}

    inline bool operator ==(const Ascii& s) const { return m_size == s.m_size && std::memcmp(m_data, s.m_data, m_size) == 0; }
    inline bool operator !=(const Ascii& s) const { return !this->operator==(s); }

    size_t size() const { return m_size; }
    const char* c_str() const { return m_data; }

private:
    size_t m_size = 0;
    const char* m_data = nullptr;
};

using Type = Ascii;

static constexpr Type ERROR = "ERROR";
static constexpr Type WARN = "WARN";
static constexpr Type INFO = "INFO";
static constexpr Type DEBUG = "DEBUG";

enum Level {
    Off     = 0,
    Normal  = 1,
    Debug   = 2,
    Full    = 3
};

struct Date
{
    int day = 0;
    int mon = 0;
    int year = 0;

    inline bool operator ==(const Date& d) const { return day == d.day && mon == d.mon && year == d.year; }
    inline bool operator !=(const Date& d) const { return !this->operator ==(d); }
};

struct Time
{
    int msec = 0;
    int sec = 0;
    int min = 0;
    int hour = 0;
};

struct DateTime
{
    Date date;
    Time time;

    static DateTime now();
};

//! Message --------------------------------
class LogMsg
{
public:

    LogMsg() = default;

    LogMsg(const Type& l, const std::string& t)
        : type(l), tag(t), datetime(DateTime::now()),
        thread(std::this_thread::get_id()) {}

    LogMsg(const Type& l, const std::string& t, const std::string& m)
        : type(l), tag(t), message(m), datetime(DateTime::now()),
        thread(std::this_thread::get_id()) {}

    Type type;
    std::string tag;
    std::string message;
    DateTime datetime;
    std::thread::id thread;
};

//! Layout ---------------------------------
class LogLayout
{
public:
    explicit LogLayout(const std::string& format);
    virtual ~LogLayout();

    struct PatternData {
        PatternData() = default;
        Ascii pattern = nullptr;
        std::string beforeStr;
        int index = -1;
        int count = 0;
        size_t minWidth = 0;
    };

    std::string format() const;

    virtual std::string output(const LogMsg& logMsg) const;

    virtual std::string formatPattern(const LogMsg& logMsg, const PatternData& p) const;
    virtual std::string formatDateTime(const DateTime& datetime) const;
    virtual std::string formatDate(const Date& date) const;
    virtual std::string formatTime(const Time& time) const;
    virtual std::string formatThread(const std::thread::id& thID) const;

    static PatternData parcePattern(const std::string& format, const Ascii& pattern);
    static std::vector<PatternData> patterns(const std::string& format);

private:
    std::string m_format;
    std::vector<PatternData> m_patterns;
    std::thread::id m_mainThread;
};

//! Destination ----------------------------
class LogDest
{
public:
    explicit LogDest(const LogLayout& l);
    virtual ~LogDest();

    virtual std::string name() const = 0;
    virtual void write(const LogMsg& logMsg) = 0;

    LogLayout layout() const;

protected:
    LogLayout m_layout;
};

//! Logger ---------------------------------
class Logger
{
public:

    static Logger* instance()
    {
        static Logger l;
        return &l;
    }

    void setupDefault();

    void setLevel(Level level);
    Level level() const;
    inline bool isLevel(Level level) const { return level <= m_level && level != Off; }

    std::vector<Type> types() const;
    void setTypes(const std::vector<Type>& types);
    void setType(const Type& type, bool enb);
    bool isType(const Type& type) const;

    bool isAsseptMsg(const Type& type) const;

#ifdef KORS_LOGGER_QT_SUPPORT
    static void setIsCatchQtMsg(bool arg);
#endif

    void write(const LogMsg& logMsg);

    void addDest(LogDest* dest);
    std::vector<LogDest*> dests() const;
    void clearDests();

private:
    Logger();
    ~Logger();

#ifdef KORS_LOGGER_QT_SUPPORT
    static void logMsgHandler(QtMsgType, const QMessageLogContext&, const QString&);
    static Type qtMsgTypeToString(enum QtMsgType defType);
#endif

    Level m_level = Normal;
    std::vector<LogDest*> m_dests;
    std::vector<Type> m_types;
    std::mutex m_mutex;
};

//! LogInput ---------------------------------
class LogInput
{
public:
    explicit LogInput(const Type& type, const std::string& tag)
        : m_msg(type, tag) {}

    ~LogInput()
    {
        m_msg.message = m_stream.str();
        Logger::instance()->write(m_msg);
    }

    Stream& stream() { return m_stream; }

private:
    LogMsg m_msg;
    Stream m_stream;
};
}

#endif // KORS_LOGGER_H
