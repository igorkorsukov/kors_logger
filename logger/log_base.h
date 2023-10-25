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

#ifndef KORS_LOG_BASE_H
#define KORS_LOG_BASE_H

#include "helpful.h"
#include "logger.h"

#ifndef FUNC_INFO
#if defined(_MSC_VER)
    #define FUNC_INFO __FUNCSIG__
#else
    #define FUNC_INFO __PRETTY_FUNCTION__
#endif
#endif

//! Format
#define CLASSNAME(sig) kors::logger::Helpful::className(sig)
#define FUNCNAME(sig) kors::logger::Helpful::methodName(sig)

//! Log

#ifndef LOG_TAG
#define LOG_TAG CLASSNAME(FUNC_INFO)
#endif

#define IF_LOGLEVEL(level)  if (kors::logger::Logger::instance()->isLevel(level))

#define LOG_STREAM(type, tag) kors::logger::LogInput(type, tag).stream()
#define LOG(type, tag)  LOG_STREAM(type, tag) << FUNCNAME(FUNC_INFO) << ": "

#define LOGE()      IF_LOGLEVEL(kors::logger::Normal) LOG(kors::logger::ERROR, LOG_TAG)
#define LOGW()      IF_LOGLEVEL(kors::logger::Normal) LOG(kors::logger::WARN, LOG_TAG)
#define LOGI()      IF_LOGLEVEL(kors::logger::Normal) LOG(kors::logger::INFO, LOG_TAG)
#define LOGD()      IF_LOGLEVEL(kors::logger::Debug) LOG(kors::logger::DEBUG, LOG_TAG)

//! Helps
#define DEPRECATED LOGD() << "This function deprecated!!";
#define DEPRECATED_USE(use) LOGD() << "This function deprecated!! Use:" << use;
#define NOT_IMPLEMENTED LOGW() << "Not implemented!!";
#define NOT_IMPL_RETURN NOT_IMPLEMENTED return
#define NOT_SUPPORTED LOGW() << "Not supported!!";
#define NOT_SUPPORTED_USE(use) LOGW() << "Not supported!! Use:" << use;

#endif // KORS_LOG_BASE_H
