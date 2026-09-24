//
// Created by dlllibstdntc on 22.09.2026.
//

#include "ConsoleWriter.h"
#include <iostream>
#include <iomanip>
#include <sstream>

toolkit::tui::ProgressBar::ProgressBar(const INT max,
                                       const INT width,
                                       const INT height): max(max), current(0), width(width), height(height),
                                                               fillChar('#'), emptyChar('-'),
                                                               showPercentage(true), showCount(true) {
}

void toolkit::tui::ProgressBar::SetLabel(const STRING &text) {
    label = text;
}

void toolkit::tui::ProgressBar::SetChars(CHAR fill, CHAR empty) {
    fillChar = fill;
    emptyChar = empty;
}

void toolkit::tui::ProgressBar::SetShowPercentage(BOOL value) {
    showPercentage = value;
}

void toolkit::tui::ProgressBar::SetShowCount(BOOL value) {
    showCount = value;
}

void toolkit::tui::ProgressBar::SetProgress(INT value) {
    current = value < 0 ? 0 : (value > max ? max : value);
    Render();
}

void toolkit::tui::ProgressBar::Advance(INT step) {
    SetProgress(current + step);
}

INT toolkit::tui::ProgressBar::GetProgress() const {
    return current;
}

BOOL toolkit::tui::ProgressBar::IsFinished() const {
    return current >= max;
}

// todo: переписать реализацию, на более эффективную
void toolkit::tui::ProgressBar::Render() const {
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

void toolkit::tui::ProgressBar::Finish() {
    if (finished) return;
    SetProgress(max);
    std::cout << "\n";
    finished = true;
}
