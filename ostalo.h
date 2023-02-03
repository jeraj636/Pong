#include <fmt/core.h>
#include <fmt/color.h>
#include <iostream>

namespace io
{
    enum class type
    {
        msg,
        warning,
        error
    };
    void izpis(const char *msg, const type &tip)
    {
        static int w = 0;
        static int s = 0;
        if (tip == type::error)
        {
            fmt::print(fg(fmt::color::red), "ERROR: {}\nKONEC PROGRAMA!\n", msg);
            exit(-1);
        }
        else if (tip == type::msg)
        {
            fmt::print(fg(fmt::color::green), "{}: MSG: ", s++);
            fmt::print("{}\n", msg);
        }
        else if (tip == type::warning)
        {
            fmt::print(fg(fmt::color::green), "{}: WARNING: ", w++);
            fmt::print("{}\n", msg);
        }
    }
    template <typename t>
    void vpis(const char *msg, t &input)
    {
        fmt::print(fg(fmt::color::dark_violet), "[{}]>> ", msg);
        std::cin >> input;
    }

}
