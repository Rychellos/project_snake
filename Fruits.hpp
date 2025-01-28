//
// Created by Rychellos on 13.01.2025.
//

#ifndef FRUITS_H
#define FRUITS_H

#include "display.hpp"

#define NUM_FRUITS 10

struct Point {
    short x;
    short y;
};

class Fruits {
public:
    Point list[NUM_FRUITS]{};

    explicit Fruits(const Display &display);

    void update(const Display &display, bool (*snake_is_colliding_at)(int x, int y));

    bool is_at(short x, short y) const;

    void remove_at(short x, short y);
};


#endif //FRUITS_H
