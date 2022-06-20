#pragma once
#include <iostream>

namespace Variadic_templates_Func {
    static double get_impl(std::size_t, std::size_t)
    {
        return 0;
    }

    template <class... Ts>
    static double get_impl(std::size_t n, std::size_t i, double value, Ts... rest)
    {
        return n == i ? value : get_impl(n, i + 1, rest...);
    }

    template <class... Ts>
    static double get(std::size_t n, Ts... args)
    {
        return get_impl(n, 0, args...);
    }
}