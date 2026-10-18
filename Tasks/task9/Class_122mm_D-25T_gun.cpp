#include "Class_Gun.h"
#include <cstring>
#include "Class_122mm_D-25T_gun.h"

gun_122mm_D_25T::gun_122mm_D_25T() {
    strcpy(g_name, "122мм Д-25Т");
    g_damage = 400;
    g_cooldown = 14.38;
    g_break_through = 8.75;
}
