#ifndef KORS_LOGSTREAM_H
#define KORS_LOGSTREAM_H

#include <sstream>

#ifdef KORS_LOGGER_QT_SUPPORT
#include <QDebug>
#endif

namespace kors::logger {
class Stream
{
public:
    Stream() = default;

    inline Stream& operator<<(bool t) { m_ss << t; return *this; }
    inline Stream& operator<<(char t) { m_ss << t; return *this; }
    inline Stream& operator<<(signed short t) { m_ss << t; return *this; }
    inline Stream& operator<<(unsigned short t) { m_ss << t; return *this; }
    inline Stream& operator<<(char16_t t) { m_ss << t; return *this; }
    inline Stream& operator<<(char32_t t) { m_ss << t; return *this; }
    inline Stream& operator<<(signed int t) { m_ss << t; return *this; }
    inline Stream& operator<<(unsigned int t) { m_ss << t; return *this; }
    inline Stream& operator<<(signed long t) { m_ss << t; return *this; }
    inline Stream& operator<<(unsigned long t) { m_ss << t; return *this; }
    inline Stream& operator<<(float t) { m_ss << t; return *this; }
    inline Stream& operator<<(double t) { m_ss << t; return *this; }
    inline Stream& operator<<(const void* t) { m_ss << t; return *this; }
    inline Stream& operator<<(const char* t) { m_ss << t; return *this; }

    inline Stream& operator<<(const std::string& t) { m_ss << t; return *this; }
    inline Stream& operator<<(const std::thread::id& t) { m_ss << t; return *this; }

#ifdef KORS_LOGGER_QT_SUPPORT
    inline Stream& operator<<(qint64 t) { qt_to_ss(t); return *this; }
    inline Stream& operator<<(quint64 t) { qt_to_ss(t); return *this; }
    inline Stream& operator<<(QChar t) { qt_to_ss(t); return *this; }
    inline Stream& operator<<(const QString& t) { qt_to_ss(t); return *this; }
    inline Stream& operator<<(const QStringRef& t) { qt_to_ss(t); return *this; }
    inline Stream& operator<<(QStringView t) { qt_to_ss(t); return *this; }
    inline Stream& operator<<(QLatin1String t) { qt_to_ss(t); return *this; }
    inline Stream& operator<<(const QByteArray& t) { qt_to_ss(t); return *this; }
#endif

    inline std::string str() const { return m_ss.str(); }

private:

#ifdef KORS_LOGGER_QT_SUPPORT
    template<typename T>
    inline void qt_to_ss(const T& t)
    {
        QString str;
        QDebug q(&str);
        q << t;
        m_ss << str.toStdString();
    }

#endif

    std::stringstream m_ss;
};
}

#endif //KORS_LOGSTREAM_H
