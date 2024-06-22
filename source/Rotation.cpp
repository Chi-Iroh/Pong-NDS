#include "Rotation.hpp"

#include <nds.h>

std::pair<int, int> rotate(const std::pair<int, int>& move, Direction obstacleDirection) {
    if (obstacleDirection == Direction::Horizontal) {
        return { move.first, -move.second };
    }
    return { -move.first, move.second };
}