#include "Class_Gun.h"
#include <cstring>
#include "Class_152mm_ML-20_gun.h"

gun_152mm_ML_20::gun_152mm_ML_20() {
    strcpy(g_name, "152мм МЛ-20");
    g_damage = 640;
    g_cooldown = 14.29;
    g_break_through = 7.3;
}
