#include <iostream>
#include <thread>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#ifdef KORS_LOGGER_QT_SUPPORT
#include <QDebug>
#include <QString>
#include <QPoint>
#endif

#include "logdefdest.h"
#include "log.h"

class Example
{
public:
    Example() = default;

    void example()
    {
        using namespace kors::logger;

        Logger* logger = Logger::instance();
        logger->setupDefault();

        //! Default output to console, catch Qt messages

        LOGE() << "This is error";
        LOGW() << "This is warning";
        LOGI() << "This is info";
        LOGD() << "This is debug"; //! NOTE Default not output

        std::thread t1([]() { LOGI() << "From thread"; });
        t1.join();

#ifdef KORS_LOGGER_QT_SUPPORT
        qCritical() << "This is qCritical";
        qWarning() << "This is qWarning";
        qDebug() << "This is qDebug"; //! NOTE Default not output

        LOGI() << QString("This is QString");
#endif

        /*
        15:21:53.454 | ERROR | main_thread     | Example    | example: This is error
        15:21:53.454 | WARN  | main_thread     | Example    | example: This is warning
        15:21:53.454 | INFO  | main_thread     | Example    | example: This is info
        15:21:53.454 | INFO  | 139855304345344 | Example    | example: From thread
        15:21:53.455 | ERROR | main_thread     | Qt         | example: This is qCritical
        15:21:53.455 | WARN  | main_thread     | Qt         | example: This is qWarning
        */

        //! Using message formatting
        LOGI("This is formatted message, arg1: %d, arg2: %d, sum: %d", 40, 2, 42);

        //! Set tag (default class::func)

        #undef LOG_TAG
        #define LOG_TAG "MYTAG"

        LOGI() << "This is info with tag";
        /*
        15:21:53.455 | INFO  | main_thread     | MYTAG      | example: This is info with tag
        */

        //! Set log level
        logger->setLevel(kors::logger::Debug);

        LOGD() << "This is debug";
#ifdef KORS_LOGGER_QT_SUPPORT
        qDebug() << "This is qDebug (for Qt always tag is class::func)";
#endif

        /*
        15:34:54.257 | DEBUG | main_thread     | MYTAG      | example: This is debug
        15:34:54.257 | DEBUG | main_thread     | Qt         | example: This is qDebug
        */

        //! --- Setup logger ---
        LOGI() << "Custom setup... ";
        //! Destination and format
        logger->clearDests();

        //! Console
        logger->addDest(new ConsoleLogDest(LogLayout("${time} | ${type|7} | ${thread} | ${tag|20} | ${message}")));

        //! File
        std::string pwd = fs::current_path();
        std::string logsDir = pwd + "/logs";
        if (!fs::exists(logsDir)) {
            fs::create_directories(logsDir);
        }
        std::string logPath = logsDir + "/myapp.log";
        logger->addDest(new FileLogDest(logPath, LogLayout("${datetime} | ${type|7} | ${thread} | ${tag|20} | ${message}")));

        /** NOTE Layout have a tags
        "${datetime}"   - yyyy-MM-ddThh:mm:ss.zzz
        "${time}"       - hh:mm:ss.zzz
        "${type}"       - type
        "${tag}"        - tag
        "${thread}"     - thread, the main thread output as "main" otherwise hex
        "${message}"    - message
        |N - min field width
         */

        LOGI() << "now log fields width is changed";

        //! Level
        logger->setLevel(kors::logger::Debug);

        //! Catch Qt message
#ifdef KORS_LOGGER_QT_SUPPORT
        logger->setIsCatchQtMsg(true);
#endif

        //! Custom types
        logger->setType("MYTRACE", true);

        //! See custom macro in log.h

        MYTRACE() << "This my trace";

        /*
        15:34:54.257 | MYTRACE | main_thread     | MYTAG      | example: This my trace
        */

        //! That type does not output
        logger->setType("MYTRACE", false); //! NOTE Type must be a debug level

        MYTRACE() << "This my trace"; //! NOTE Not output

        //! Custom LogLayout - inherits of the LogLayout and override method "output"
        //! Custom LogDest - inherits of the LogDest and override method "write"
        //! Custom log macro - see log.h
    }
};

int main(int argc, char* argv[])
{
    std::cout << "Hello World, I am Logger\n";

    Example t;
    t.example();

#undef LOG_TAG
#define LOG_TAG CLASSFUNC

    LOGI() << "Goodbye!";
}
