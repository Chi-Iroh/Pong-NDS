#pragma once
#include <utility>

enum class Direction {
    Vertical,
    Horizontal
};

std::pair<int, int> rotate(const std::pair<int, int>& move, Direction obstacleDirection);