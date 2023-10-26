#ifndef EXAMPLE_LOG_H
#define EXAMPLE_LOG_H

#include "log_base.h"

// custom macros

#define MYTRACE IF_LOGLEVEL(kors::logger::Debug) LOG("MYTRACE", LOG_TAG)

#endif // EXAMPLE_LOG_H
