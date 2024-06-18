#include "Rotation.hpp"
#include <nds.h>

static inline int inverse_sign(int x) {
    return -x;
}

std::pair<int, int> rotate(const std::pair<int, int>& move, Direction obstacleDirection) {
    if (obstacleDirection == Direction::Horizontal) {
        return { move.first, inverse_sign(move.second) };
    }
    return { inverse_sign(move.first), move.second };
}