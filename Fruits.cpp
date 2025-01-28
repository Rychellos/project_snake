//
// Created by Rychellos on 13.01.2025.

#include "Fruits.hpp"

#include "display.hpp"
#include "Snake_Body.hpp"

Fruits::Fruits(const Display &display) {
    for (auto &fruit: list) {
        fruit = Point(-1, -1);

        while (true) {
            short x = 1 + rand() % display.screen_width - 2;
            short y = 1 + rand() % display.screen_height - 6;

            if (is_at(x, y)) {
                continue;
            }

            fruit.x = x;
            fruit.y = y;

            break;
        }
    }
}

void Fruits::update(const Display &display, bool (*snake_is_colliding_at)(int x, int y)) {
    for (int i = 0; i < NUM_FRUITS; i++) {
        if (list[i].x < 0 ||
            list[i].y < 0
            || list[i].x > display.screen_width - 2
            || list[i].y > display.screen_height - 6) {
            continue;
        }

        display.draw_wchar(
            list[i].x + 1,
            list[i].y + 4,
            L' '
        );
    }

    for (int i = 0; i < NUM_FRUITS; i++) {
        if (
            list[i].x < 0
            || list[i].y < 0
            || list[i].x > display.screen_width - 2
            || list[i].y > display.screen_height - 5
        ) {
            while (true) {
                short x = rand() % (display.screen_width - 2);
                short y = rand() % (display.screen_height - 5);

                if (is_at(x, y) || snake_is_colliding_at(x, y)) {
                    continue;
                }

                list[i].x = x;
                list[i].y = y;

                break;
            }
        }
    }

    for (int i = 0; i < NUM_FRUITS; i++) {
        if (list[i].x < 0 || list[i].y < 0) {
            continue;
        }

        display.draw_wchar(list[i].x + 1, list[i].y + 4, L'♡');
    }
}

bool Fruits::is_at(short x, short y) const {
    for (int i = 0; i < NUM_FRUITS; i++) {
        if (list[i].x == x && list[i].y == y) {
            return true;
        }
    }

    return false;
}

void Fruits::remove_at(short x, short y) {
    for (int i = 0; i < NUM_FRUITS; i++) {
        if (list[i].x == x && list[i].y == y) {
            list[i].x = -1;
            list[i].y = -1;
        }
    }
}
