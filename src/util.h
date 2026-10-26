// util.h
// Small shared helpers: safe line input and C-string allocation (C++23).
#pragma once

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

namespace acs
{

    // Read one line from stdin into a buffer and strip the newline.
    // Returns the buffer for chaining.
    inline char *read_line(char *buffer, int size)
    {
        if (std::fgets(buffer, size, stdin) == nullptr)
            buffer[0] = 0;
        buffer[std::strcspn(buffer, "\n")] = 0;
        return buffer;
    }

    // Allocate a modifiable copy of a string (for the char* fields).
    inline char *dup_cstr(const char *src)
    {
        const std::size_t n = (src ? std::strlen(src) : 0) + 1;
        char *dst = static_cast<char *>(std::malloc(n));
        if (dst)
        {
            if (src)
                std::memcpy(dst, src, n);
            else
                dst[0] = 0;
        }
        return dst;
    }

    // Flush whatever is left on the input line before a scanf/fgets pair.
    inline void drain_stdin()
    {
        int c;
        while ((c = std::fgetc(stdin)) != '\n' && c != EOF)
        {
        }
    }

} // namespace acs
