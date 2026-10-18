#include "Class_Gun.h"
#include <cstring>
#include "Class_122mm_D2-5T_gun.h"

gun_122mm_D2_5T::gun_122mm_D2_5T() {
    strcpy(g_name, "122мм Д2-5Т");
    g_damage = 400;
    g_cooldown = 17.45;
    g_break_through = 8;
}
