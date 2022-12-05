#ifndef _MOVEMENT_DIRECTION_HPP_
#define _MOVEMENT_DIRECTION_HPP_

#include <cstdint>

namespace specter {

enum class MovementDirection : uint8_t {
    None,
    Forward, Backward, Left, Right
};

}

#endif