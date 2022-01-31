#include "Watchy_Asymmetrical.h"
#include "settings.h"

WatchyAsymm watchy(settings);

void setup() {
    Serial.begin(115200);
    watchy.init();
}

void loop() {
}
