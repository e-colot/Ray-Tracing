#include "constants.h"
#include <cmath> // For std::round

bool EXERCISE = false;
const float MU_0 = 12.56637061e-7f;
const float EPS_0 = 8.85418782e-12f;
long long FREQUENCY = static_cast<long long>((EXERCISE) ? 8683e5 : 60e9);
const float PI = 3.1415926535f;
long long PULS = static_cast<long long>(2.0f * PI * FREQUENCY);
const int C = 299792458;
const float Z_0 = 376.73f;
const int R_A = 73;
float P_TX = (EXERCISE) ? 1.0f : 100.0f; // 10^(20/10), in mW
float BETA_AIR = static_cast<float>(PULS / C);
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 1000;
int SCALE = ((EXERCISE) ? 8 : 90);

void CHANGE_TYPE(bool exercise) {
	EXERCISE = exercise;
	FREQUENCY = static_cast<long long>((EXERCISE) ? 8683e5 : 60e9);
	PULS = static_cast<long long>(2.0f * PI * FREQUENCY);
	SCALE = ((EXERCISE) ? 8 : 90);
	BETA_AIR = static_cast<float>(PULS / C);
	P_TX = (EXERCISE) ? 1.0f : 100.0f;
}
