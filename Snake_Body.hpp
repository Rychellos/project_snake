//
// Created by Rychellos on 13.01.2025.
//

#ifndef SNAKE_BODY_H
#define SNAKE_BODY_H

#include "Fruits.hpp"

class Fruits;

struct Snake_Body_Node {
    int x = 0;
    int y = 0;
    Snake_Body_Node *next = nullptr;
};

enum Direction {
    NONE,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class Snake_Body {
    Direction direction;
    Direction next_direction;


    void move_in_direction(Direction direction) const;

public:
    Snake_Body();

    Snake_Body_Node *head;

    /**
     * Checks if any part of snake's body, except the
     * head, is located at provided spot.
     */
    bool is_colliding_at(int x, int y);

    /**
     * Generates head if not present at the center of
     * the screen and then generates specified number
     * of body segments to the left of the head.
     * @param n number of body segments
     */
    void generate_body(Display &display, int n);

    void set_direction(Direction direction);

    bool tick(Display &display, Fruits &fruits, int &points);

    void die(Display &display);
};


#endif //SNAKE_BODY_H
