#pragma once

static constexpr int SCOREBOARD_PLAYER_X_OFFSET{ 23 };
static constexpr int SCOREBOARD_PLAYER_Y_OFFSET{ 40 };

static constexpr int SCOREBOARD_AI_X_OFFSET{ 145 };
static constexpr int SCOREBOARD_AI_Y_OFFSET{ 40 };

void initDigits();
void freeDigits();

void displayPlayerScore(unsigned score);
void displayEnemyScore(unsigned score);