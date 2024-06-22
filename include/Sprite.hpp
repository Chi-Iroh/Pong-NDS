#pragma once

struct Sprite {
protected:
    static int globalID;

public:
    const int ID;

    Sprite();

    // Sprite generates unique IDs from 0 to 127, it will be cumbersome to track which ones are used and which ones aren't.
    Sprite(const Sprite&) = delete;
    // Sprite generates unique IDs from 0 to 127, it will be cumbersome to track which ones are used and which ones aren't.
    Sprite& operator=(const Sprite&) = delete;
};