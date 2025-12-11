#include "globals.h"

int g_nextPipeId = 1;
int g_nextStationId = 1;

const std::map<int, double> PIPE_DIAMETERS_CAPACITY = {
    {530, 5.0},
    {720, 12.0},
    {1020, 30.0},
    {1220, 55.0},
    {1420, 95.0}
};