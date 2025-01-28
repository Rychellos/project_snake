//
// Created by Rychellos on 13.01.2025.
//

#include <thread>

#include "Snake_Body.hpp"

Snake_Body::Snake_Body() {
    direction = RIGHT;
    next_direction = RIGHT;
    head = nullptr;
}

void Snake_Body::move_in_direction(const Direction value) const {
    switch (value) {
        case UP:
            head->y -= 1;
            break;
        case DOWN:
            head->y += 1;
            break;
        case LEFT:
            head->x -= 1;
            break;
        case RIGHT:
            head->x += 1;
            break;
        default:
            break;
    }
}

bool Snake_Body::is_colliding_at(int x, int y) {
    if (head == nullptr || head->next == nullptr) {
        return false;
    }

    auto current = head->next;
    while (current->next != nullptr) {
        if (current->x == x && current->y == y) {
            return true;
        }

        current = current->next;
    }

    return false;
}

bool is_in_bounds_at(Display &display, int x, int y) {
    if (
        x < 0
        || y < 0
        || x > display.screen_width - 3
        || y > display.screen_height - 6
    ) {
        return false;
    }

    return true;
}

void Snake_Body::generate_body(Display &display, int n) {
    if (head == nullptr) {
        head = new Snake_Body_Node();
        head->x = (display.screen_width - 2) / 2;
        head->y = (display.screen_height - 6) / 2;
    }

    Snake_Body_Node *current = head;

    for (int i = 1; i <= n; i++) {
        auto *node = new Snake_Body_Node{current->x - 1, current->y};

        current->next = node;
        current = node;
    }
}

void Snake_Body::set_direction(Direction value) {
    switch (value) {
        case UP:
            if (direction == DOWN) {
                break;
            }

            next_direction = value;
            break;
        case RIGHT:
            if (direction == LEFT) {
                break;
            }

            next_direction = value;
            break;
        case DOWN:
            if (direction == UP) {
                break;
            }

            next_direction = value;
            break;
        case LEFT:
            if (direction == RIGHT) {
                break;
            }

            next_direction = value;
            break;
        default:
            break;
    }
}

bool Snake_Body::tick(Display &display, Fruits &fruits, int &points) {
    Snake_Body_Node *new_node = new Snake_Body_Node{
        head->x,
        head->y,
        head
    };

    head = new_node;

    // Handling directions
    if (next_direction != NONE) {
        direction = next_direction;
        next_direction = NONE;
    }

    move_in_direction(direction);

    if (is_colliding_at(head->x, head->y) || !is_in_bounds_at(display, head->x, head->y)) {
        return false;
    }

    Snake_Body_Node *tail = head;
    Snake_Body_Node *before_tail = nullptr;

    while (tail->next != nullptr) {
        before_tail = tail;
        tail = tail->next;
    }

    if (fruits.is_at(head->x, head->y)) {
        fruits.remove_at(head->x, head->y);

        points++;
    } else {
        display.draw_wchar(tail->x + 1, tail->y + 4, L' ');

        delete tail;

        if (before_tail != nullptr) {
            before_tail->next = nullptr;
        }
    }

    return true;
}

void Snake_Body::die(Display &display) {
    Snake_Body_Node *temp = head;
    head = head->next;
    delete temp;

    while (head != nullptr) {
        temp = head;
        head = head->next;

        display.draw_wchar(temp->x + 1, temp->y + 4, L' ');

        delete temp;
        std::this_thread::sleep_for(std::chrono::milliseconds(125));
    }
}


