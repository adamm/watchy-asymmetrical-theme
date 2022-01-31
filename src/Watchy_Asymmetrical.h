#ifndef WATCHY_ASYMM
#define WATCHY_ASYMM_H

#include <Watchy.h>

class WatchyAsymm : public Watchy {
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawBattery();
};

#endif
