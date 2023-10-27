#ifndef EXAMPLE_LOG_H
#define EXAMPLE_LOG_H

#include "log_base.h"

// custom macros

#define MYTRACE IF_LOGLEVEL(kors::logger::Level::Debug) LOG_STREAM("MYTRACE", LOG_TAG, kors::logger::Color::Magenta)

#endif // EXAMPLE_LOG_H
