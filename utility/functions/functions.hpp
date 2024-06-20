#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#endif

#include <QString>
#include <stdexcept>

namespace srilakshmikanthanp::utility {
#ifdef _WIN32
QString getFileNameFromHandle(HANDLE hFile);
#endif
}  // namespace srilakshmikanthanp::utility
