#pragma once

static constexpr unsigned WIDTH{ 256 };
static constexpr unsigned HEIGHT{ 196 };

static constexpr unsigned TILE_SIZE{ 8 };
static constexpr unsigned WIDTH_N_TILES{ WIDTH / TILE_SIZE };
static constexpr unsigned HEIGHT_N_TILES{ HEIGHT / TILE_SIZE };

static constexpr unsigned MAX_X{ WIDTH_N_TILES - 1 };
static constexpr unsigned MAX_Y{ HEIGHT_N_TILES - 1 };