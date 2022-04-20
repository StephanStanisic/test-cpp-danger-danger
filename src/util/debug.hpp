#pragma once
#include <csignal>
#define breakpoint std::raise(SIGINT)
