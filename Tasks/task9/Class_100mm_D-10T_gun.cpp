#include "Class_Gun.h"
#include <cstring>
#include "Class_100mm_D-10T_gun.h"

gun_100mm_D_10T::gun_100mm_D_10T() {
    strcpy(g_name, "100мм Д-10Т");
    g_damage = 280;
    g_cooldown = 8.63;
    g_break_through = 8.75;
}
