#include "Class_Gun.h"
#include <cstring>
#include "Class_100mm_D-10S_gun.h"

gun_100mm_D_10S::gun_100mm_D_10S() {
    strcpy(g_name, "100мм Д-10С");
    g_damage = 280;
    g_cooldown = 8.05;
    g_break_through = 8.75;
}
