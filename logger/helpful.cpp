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

#include "helpful.h"

using namespace kors::logger;

static const char* Colon("::");
static const char ArgBegin('(');
static const char Space(' ');

std::string Helpful::className(const std::string& sig)
{
    std::size_t endFunc = sig.find_first_of(ArgBegin);
    if (endFunc == std::string::npos) {
        return sig;
    }

    std::size_t beginFunc = sig.find_last_of(Colon, endFunc);
    if (beginFunc == std::string::npos) {
        return std::string();
    }

    std::size_t beginClassColon = sig.find_last_of(Colon, beginFunc - 2);
    std::size_t beginClassSpace = sig.find_last_of(Space, beginFunc - 2);

    std::size_t beginClass = std::string::npos;
    if (beginClassColon == std::string::npos) {
        beginClass = beginClassSpace;
    } else if (beginClassSpace == std::string::npos) {
        beginClass = beginClassColon;
    } else {
        beginClass = std::max(beginClassColon, beginClassSpace);
    }

    if (beginClass == std::string::npos) {
        beginClass = 0;
    } else {
        beginClass += 1;
    }

    return sig.substr(beginClass, (beginFunc - 1 - beginClass));
}

std::string Helpful::methodName(const std::string& sig)
{
    std::size_t endFunc = sig.find_first_of(ArgBegin);
    if (endFunc == std::string::npos) {
        return sig;
    }

    std::size_t beginFunc = sig.find_last_of(Colon, endFunc);
    if (beginFunc == std::string::npos) {
        beginFunc = sig.find_last_of(Space, endFunc);
    }

    if (beginFunc == std::string::npos) {
        beginFunc = 0;
    } else {
        beginFunc += 1;
    }

    return sig.substr(beginFunc, (endFunc - beginFunc));
}
