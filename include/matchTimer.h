#include <Arduino.h>


constexpr unsigned long pamiStartTime = 85; //seconds
constexpr unsigned long matchTotalDuration = 100; //seconds
constexpr unsigned long preparationStart = 80; // seconds
constexpr unsigned long preparationEnd = 85; // seconds

int getMatchTime();
void startMatchTimer(int timeOffset = 0);