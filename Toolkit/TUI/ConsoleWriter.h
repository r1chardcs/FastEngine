//
// Created by dlllibstdntc on 22.09.2026.
//

#ifndef FASTENGINE_CONSOLEWRITER_H
#define FASTENGINE_CONSOLEWRITER_H

#include <Platform.h>

namespace toolkit::tui {
    class ProgressBar {
        INT max;
        INT current;
        INT width;
        INT height;

        CHAR fillChar;
        CHAR emptyChar;
        BOOL showPercentage;
        BOOL showCount;
        STRING label;

        BOOL finished = false;

    public:
        ProgressBar(INT max, INT width = 40, INT height = 1);

        void SetLabel(const STRING& text);
        void SetChars(CHAR fill, CHAR empty);
        void SetShowPercentage(BOOL value);
        void SetShowCount(BOOL value);
        void SetProgress(INT value);

        void Advance(INT step = 1);
        INT GetProgress() const;
        BOOL IsFinished() const;
        void Render() const;
        void Finish();
    };
}


#endif //FASTENGINE_CONSOLEWRITER_H
