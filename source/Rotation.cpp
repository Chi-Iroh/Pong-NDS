#include "Rotation.hpp"

static inline int inverse_1_minus1(int x) {
    return x == 1 ? -1 : 1;
}

std::pair<int, int> rotate(const std::pair<int, int>& move, Direction obstacleDirection) {
    if (obstacleDirection == Direction::Hozirontal) {
        return { move.first, inverse_1_minus1(move.second) };
    }
    return { inverse_1_minus1(move.first), move.second };
}