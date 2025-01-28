//
// Created by Rychellos on 19.12.2024.
//

#include "display.hpp"

#include <array>
#include <iostream>
#include <conio.h>
#include <fstream>

Display::Display() {
    high_score_cache = 0;
    current_score_cache = 0;

    screen_width = 0;
    screen_height = 0;

    console_output = GetStdHandle(STD_OUTPUT_HANDLE);
    header_lines = {10, 40};

    update_screen_size();
}

void Display::draw_wchar(int x, int y, wchar_t ch) const {
    COORD pos = {static_cast<short>(x), static_cast<short>(y)};

    pos.X = static_cast<short>(x);
    pos.Y = static_cast<short>(y);
    SetConsoleCursorPosition(console_output, pos);

    std::wcout << ch;
}

void Display::draw_wchar(int x, int y, wchar_t *ch[]) const {
    COORD pos = {static_cast<short>(x), static_cast<short>(y)};

    pos.X = static_cast<short>(x);
    pos.Y = static_cast<short>(y);
    SetConsoleCursorPosition(console_output, pos);

    std::wcout << ch;
}

void Display::draw_wstring(int x, int y, const std::wstring &text) const {
    COORD pos = {static_cast<short>(x), static_cast<short>(y)};

    pos.X = static_cast<short>(x);
    pos.Y = static_cast<short>(y);
    SetConsoleCursorPosition(console_output, pos);

    std::wcout << text;
}

void Display::set_current_score(const int value) {
    if (current_score_cache != value) {
        current_score_cache = value;
        draw_header();
    }
}

void Display::set_high_score(const int value) {
    if (high_score_cache != value) {
        high_score_cache = value;
        draw_header();
    }
}

void Display::clear_area(short x, short y, short width, short height) const {
    COORD coordinate = {x, y};
    DWORD written;

    for (short y_index = y; y_index < height + y; ++y_index) {
        coordinate.Y = y_index;
        FillConsoleOutputCharacterA(
            console_output, ' ', width, coordinate, &written
        );

        FillConsoleOutputAttribute(
            console_output, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
            width, coordinate, &written
        );
    }
}

void Display::draw_corners() const {
    draw_wchar(0, 0, L'┏');
    draw_wchar(0, screen_height - 1, L'┗');
    draw_wchar(screen_width - 1, 0, L'┓');
    draw_wchar(screen_width - 1, screen_height - 1, L'┛');
}

void Display::draw_horizontal_lines() const {
    for (int i = 1; i < screen_width - 1; ++i) {
        draw_wchar(i, 0, L'━');
        draw_wchar(i, 3, L'━');
        draw_wchar(i, screen_height - 1, L'━');
    }

    for (short index: header_lines) {
        draw_wchar(index, 0, L'┯');
        draw_wchar(index, 3, L'┷');
    }
}

void Display::draw_vertical_lines() const {
    for (int i = 1; i < screen_height - 1; ++i) {
        draw_wchar(0, i, L'┃');
        draw_wchar(screen_width - 1, i, L'┃');
    }

    for (short index: header_lines) {
        draw_wchar(index, 1, L'│');
        draw_wchar(index, 2, L'│');
    }

    draw_wchar(0, 3, L'┣');
    draw_wchar(screen_width - 1, 3, L'┫');
}

void Display::draw_highest_score() const {
    auto offset = static_cast<short>(header_lines[1] + 2);
    draw_wstring(offset, 1, L"Najlepszy wynik: ");
    offset += 17;

    const std::wstring high_score = std::to_wstring(high_score_cache);

    draw_wstring(offset, 1, high_score);
}

void Display::draw_current_score() {
    auto offset = static_cast<short>(header_lines[0] + 2);
    draw_wstring(offset, 1, L"Obecny wynik: ");
    offset += 14;
    const std::wstring score_string = std::to_wstring(current_score_cache);

    draw_wstring(offset, 1, score_string);
    offset += static_cast<short>(score_string.length());

    header_lines[1] = offset + 1;
}

void Display::draw_header() {
    for (short index: header_lines) {
        draw_wchar(index, 0, L'━');
        draw_wchar(index, 3, L'━');
    }

    clear_area(header_lines[0], 1, static_cast<short>(screen_width - 2 - header_lines[1]), 2);

    draw_wstring(1, 1, L"Snejkuś");
    draw_wstring(1, 2, L"By s33087");
    draw_current_score();
    draw_highest_score();

    for (short index: header_lines) {
        draw_wchar(index, 1, L'│');
        draw_wchar(index, 2, L'│');
    }

    for (short index: header_lines) {
        draw_wchar(index, 0, L'┯');
        draw_wchar(index, 3, L'┷');
    }
}

void Display::draw_borders() const {
    clear_area(0, 0, screen_width - 1, screen_height - 1);
    draw_corners();
    draw_horizontal_lines();
    draw_vertical_lines();
}

int Display::update_screen_size() {
    CONSOLE_SCREEN_BUFFER_INFO buffer_info;

    GetConsoleScreenBufferInfo(console_output, &buffer_info);
    const auto columns = static_cast<short>(buffer_info.srWindow.Right - buffer_info.srWindow.Left + 1);
    const auto rows = static_cast<short>(buffer_info.srWindow.Bottom - buffer_info.srWindow.Top + 1);

    if (rows != screen_height || columns != screen_width) {
        screen_height = rows;
        screen_width = columns;

        return true;
    }

    return false;
}

void Display::draw_message_box(std::wstring message) const {
    short widest_line = 0;
    short current_line_width = 0;
    short lines = 1;

    for (auto letter: message) {
        current_line_width++;

        if (letter == '\n') {
            lines++;

            if (current_line_width > widest_line) {
                widest_line = current_line_width - 1;
            }

            current_line_width = 0;
        }
    }

    short x = static_cast<short>(screen_width / 2 - widest_line / 2);
    short y = static_cast<short>(screen_height / 2 - lines / 2);

    clear_area(
        static_cast<short>(x - 1),
        y,
        static_cast<short>(widest_line + 3),
        lines
    );

    draw_wchar(x - 2, y - 1, L'╔');
    draw_wchar(x - 2, y + lines, L'╚');
    draw_wchar(x + widest_line, y - 1, L'╗');
    draw_wchar(x + widest_line, y + lines, L'╝');

    for (short i = 0; i < lines; i++) {
        draw_wchar(x - 2, y + i, L'║');
        draw_wchar(x + widest_line, y + i, L'║');
    }

    for (short i = -1; i < widest_line; i++) {
        draw_wchar(x + i, y - 1, L'═');
        draw_wchar(x + i, y + lines, L'═');
    }

    int start = 0;
    int current_line = 0;
    for (int i = 0; i < message.length(); ++i) {
        if (message[i] == '\n') {
            draw_wstring(x, y + current_line, message.substr(start, i - start));
            current_line++;
            start = i + 1;
        } else if (i + 1 == message.length()) {
            draw_wstring(x, y + current_line, message.substr(start, i + 1 - start));
        }
    }
}
