#include "Class_Gun.h"
#include <cstring>
#include "Class_122mm_D-25S_gun.h"

gun_122mm_D_25S::gun_122mm_D_25S() {
    strcpy(g_name, "122мм Д-25С");
    g_damage = 400;
    g_cooldown = 7.7;
    g_break_through = 10.25;
}
