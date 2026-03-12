#pragma once

#include <functional>


// Source - https://stackoverflow.com/a/4609795
// Posted by user79758, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-10, License - CC BY-SA 4.0

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
