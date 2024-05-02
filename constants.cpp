#include "constants.h"
#include <cmath> // For std::round

const bool EXERCISE = false;
const float MU_0 = 12.56637061e-7f;
const float EPS_0 = 8.85418782e-12f;
const long long FREQUENCY = static_cast<long long>((EXERCISE) ? 8683e5 : 60e9);
const float PI = 3.1415926535f;
const long long PULS = static_cast<long long>(2.0f * PI * FREQUENCY);
const int C = 299792458;
const float Z_0 = 376.73f;
const int R_A = 71; // R_A = R_AR because the antenna has no loss (R_AL = 0)
const float P_TX = 100.0f; // 10^(20/10), in mW
const float BETA_AIR = PULS / C;
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 800;
const int SCALE = ((EXERCISE) ? 8 : 90);
float TILE_SIZE = 0.5f;
