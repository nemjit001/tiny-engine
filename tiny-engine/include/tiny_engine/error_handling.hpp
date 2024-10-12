#pragma once

#include <cstdio>
#include <cstdlib>

/// @brief Abort the current process, logging the abort msg.
/// @param msg Message logged to stderr on process abort.
#define TINY_ENGINE_DIE(msg)	(void)(fprintf(stderr, "%s\n", msg), abort())
