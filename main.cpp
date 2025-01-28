#include <chrono>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <fcntl.h>
#include <fstream>

#include "Fruits.hpp"
#include "Snake_Body.hpp"
#include "display.hpp"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

void show_console_cursor(bool show_flag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = show_flag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

// shared between threads
auto display = std::make_shared<Display>();
auto fruits = std::make_shared<Fruits>(*display);
auto snake_body = std::make_shared<Snake_Body>();
std::shared_ptr<bool> dead = std::make_shared<bool>(0);
std::shared_ptr<bool> running = std::make_shared<bool>(0);

// written here so no circular dependencies are made
void draw_snake(const Snake_Body_Node *head) {
    const Snake_Body_Node *current = head;

    while (current->next != nullptr) {
        display->draw_wchar(current->x + 1, current->y + 4, L'#');
        current = current->next;
    }

    display->draw_wchar(head->x + 1, head->y + 4, L'*');
}

// written here so no circular dependencies are made
void draw_fruits(Point fruits[NUM_FRUITS]) {
    for (int i = 0; i < NUM_FRUITS; i++) {
        if (fruits[i].x < 0 || fruits[i].y < 0) {
            continue;
        }

        display->draw_wchar(fruits[i].x + 1, fruits[i].y + 4, L'♡');
    }
}

void print_and_save_if_best_score(int score) {
    if (std::ifstream in("highest_score.txt"); in.is_open()) {
        std::string high_score;
        in >> high_score;

        if (high_score.empty()) {
            high_score = "0";
        }

        in.close();

        if (std::stoi(high_score) < score) {
            std::ofstream out("highest_score.txt");
            if (!out.is_open()) {
                return;
            }

            out << score;
            display->set_high_score(score);
            out.close();
        } else {
            display->set_high_score(std::stoi(high_score));
        }
    }
};

int run_snake() {
    using std::chrono::steady_clock;
    auto last_tick = steady_clock::now();
    int score = 0;
    print_and_save_if_best_score(score);

    snake_body->generate_body(*display, 5);
    display->draw_borders();
    display->draw_header();

    // hacky way so there's no need for another file to share snake_body pointer
    // #lazy
    auto snake_collision_checker = [](int x, int y) {
        return snake_body->is_colliding_at(x, y);
    };

    while (*running) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - last_tick) >
            std::chrono::milliseconds(125)) {
            last_tick += std::chrono::milliseconds(125);

            fruits->update(*display, snake_collision_checker);

            if (!snake_body->tick(*display, *fruits, score)) {
                snake_body->die(*display);

                print_and_save_if_best_score(score);

                std::wstring message = L"Przegrałeś/aś :( \nTwój wynik: ";

                std::wstring temp1 = std::to_wstring(score);
                message.append(temp1);

                display->draw_borders();
                display->draw_header();
                display->draw_message_box(message);

                *dead = true;

                while (*dead) {
                    if (display->update_screen_size()) {
                        display->draw_borders();
                        display->draw_header();
                        display->draw_message_box(message);
                    }
                }

                fruits->update(*display, snake_collision_checker);

                display->clear_area(display->screen_width / 2, display->screen_height / 2, 24, 2);

                last_tick = steady_clock::now();
                score = 0;

                display->draw_borders();
                display->draw_header();

                snake_body->set_direction(RIGHT);
                snake_body->generate_body(*display, 5);
            }

            display->set_current_score(score);
            draw_snake(snake_body->head);
        }


        if (display->update_screen_size()) {
            display->clear_area(0, 0, display->screen_width - 1, display->screen_height - 1);
            fruits->update(*display, snake_collision_checker);
            display->draw_borders();
            display->draw_header();
        }
    }

    return 0;
}

int main() {
    show_console_cursor(false);
    _setmode(_fileno(stdout), _O_U8TEXT);
    *running = true;
    srand(std::chrono::steady_clock::now().time_since_epoch().count());

    std::thread thread(run_snake);

    while (true) {
        char c = getch();

        *dead = false;

        switch (c) {
            case KEY_UP: {
                snake_body->set_direction(UP);
                break;
            }
            case KEY_DOWN: {
                snake_body->set_direction(DOWN);
                break;
            }
            case KEY_LEFT: {
                snake_body->set_direction(LEFT);
                break;
            }
            case KEY_RIGHT: {
                snake_body->set_direction(RIGHT);
                break;
            }
            case 'q':
                *running = false;
                thread.join();
                return 0;
            default: ;
        }
    }
}
