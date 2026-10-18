#include "Class_Gun.h"
#include <cstring>
#include "Class_85mm_D-5T_gun.h"

gun_85mm_D_5T::gun_85mm_D_5T() {
    strcpy(g_name, "85мм Д-5Т");
    g_damage = 200;
    g_cooldown = 7.35;
    g_break_through = 6;
}
