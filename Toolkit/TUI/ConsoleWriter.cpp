//
// Created by dlllibstdntc on 22.09.2026.
//

#include "ConsoleWriter.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Tui::ProgressBar::ProgressBar(INT max, INT width, INT height): max(max), current(0), width(width), height(height),
                                                               fillChar('#'), emptyChar('-'),
                                                               showPercentage(true), showCount(true) {
}

void Tui::ProgressBar::SetLabel(const STRING &text) {
    label = text;
}

void Tui::ProgressBar::SetChars(CHAR fill, CHAR empty) {
    fillChar = fill;
    emptyChar = empty;
}

void Tui::ProgressBar::SetShowPercentage(BOOL value) {
    showPercentage = value;
}

void Tui::ProgressBar::SetShowCount(BOOL value) {
    showCount = value;
}

void Tui::ProgressBar::SetProgress(INT value) {
    current = value < 0 ? 0 : (value > max ? max : value);
    Render();
}

void Tui::ProgressBar::Advance(INT step) {
    SetProgress(current + step);
}

INT Tui::ProgressBar::GetProgress() const {
    return current;
}

BOOL Tui::ProgressBar::IsFinished() const {
    return current >= max;
}

void Tui::ProgressBar::Render() const {
    const FLOAT ratio = max > 0 ? static_cast<FLOAT>(current) / static_cast<FLOAT>(max) : 1.0f;
    const INT filled = static_cast<INT>(ratio * static_cast<FLOAT>(width));

    std::ostringstream line;
    line << "\r";

    if (!label.empty()) {
        line << label << " ";
    }

    line << "[";
    for (INT i = 0; i < width; i++) {
        line << (i < filled ? fillChar : emptyChar);
    }
    line << "]";

    if (showPercentage) {
        const INT percent = static_cast<INT>(ratio * 100.0f);
        line << " " << std::setw(3) << percent << "%";
    }

    if (showCount) {
        line << " (" << current << "/" << max << ")";
    }

    line << "    ";

    std::cout << line.str() << std::flush;
}

void Tui::ProgressBar::Finish() {
    if (finished) return;
    SetProgress(max);
    std::cout << "\n";
    finished = true;
}
