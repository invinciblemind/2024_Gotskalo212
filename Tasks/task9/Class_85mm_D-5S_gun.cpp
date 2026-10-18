#include "Class_Gun.h"
#include "Class_85mm_D-5S_gun.h"
#include <cstring>

gun_85mm_D_5S::gun_85mm_D_5S() {
    strcpy(g_name, "85 мм Д-5С");
    g_damage = 170;
    g_cooldown = 8.63;
    g_break_through = 6.5;
}
