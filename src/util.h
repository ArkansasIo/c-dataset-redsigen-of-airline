// util.h
// Small shared helpers: safe line input and C-string allocation (C++23).
#pragma once

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

namespace acs
{

    // Flush the rest of the current input line.
    // Does nothing on EOF so the stream stays in its end state.
    inline void drain_stdin()
    {
        int c = std::fgetc(stdin);
        if (c == EOF || c == '\n')
            return;
        while ((c = std::fgetc(stdin)) != '\n' && c != EOF)
        {
        }
    }

    // Read one line from stdin.
    // A leftover blank line (e.g. the newline left by a previous scanf) is
    // skipped automatically, so callers never need a manual drain first.
    // Returns the buffer for chaining.
    inline char *read_line(char *buffer, int size)
    {
        while (std::fgets(buffer, size, stdin) != nullptr)
        {
            std::size_t len = std::strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n')
            {
                buffer[--len] = 0; // strip the newline
                if (len == 0)
                    continue; // blank line: skip and read again
                break; // normal data line
            }
            if (len < static_cast<std::size_t>(size) - 1)
                break; // EOF without trailing newline: keep what we got
            drain_stdin(); // line longer than buffer: discard rest, retry
        }
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

    // Save the current state to database.dat and exit (used on EOF or
    // when input is closed so the program never spins on a dead stream).
    void save_and_exit();

    // Read a menu choice. Returns 0 on invalid input; on EOF (input closed)
    // saves and exits so the program cannot loop forever.
    inline int read_menu_choice()
    {
        int choice;
        if (std::scanf("%d", &choice) != 1)
        {
            if (std::feof(stdin))
                save_and_exit();
            drain_stdin();
            choice = 0;
        }
        else
        {
            // scanf leaves the trailing newline; consume it so the next
            // read_line() starts on a fresh data line.
            drain_stdin();
        }
        return choice;
    }

} // namespace acs
