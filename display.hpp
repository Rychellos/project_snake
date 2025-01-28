//
// Created by Rychellos on 19.12.2024.
//

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <array>
#include <string>
#include <windows.h>

class Display {
    int high_score_cache;
    int current_score_cache;

    void draw_corners() const;

    void draw_horizontal_lines() const;

    void draw_vertical_lines() const;

    void draw_highest_score() const;

    void draw_current_score();

public:
    Display();

    HANDLE console_output;
    std::array<short, 2> header_lines{};
    short screen_width;
    short screen_height;

    void set_current_score(int value);

    void set_high_score(int value);

    void draw_wchar(int x, int y, wchar_t ch) const;

    void draw_wchar(int x, int y, wchar_t *ch[]) const;

    void draw_wstring(int x, int y, const std::wstring &text) const;

    void draw_header();

    void draw_borders() const;

    void clear_area(short x, short y, short width, short height) const;

    /**
     *
     * @return 0 if no changes, 1 if size changed
     */
    int update_screen_size();

    void draw_message_box(std::wstring message) const;
};

#endif //DISPLAY_HPP
