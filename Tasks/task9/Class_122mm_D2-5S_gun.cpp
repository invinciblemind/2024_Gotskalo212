#include "Class_Gun.h"
#include <cstring>
#include "Class_122mm_D2-5S_gun.h"

gun_122mm_D2_5S::gun_122mm_D2_5S() {
    strcpy(g_name, "122мм Д2-5С");
    g_damage = 400;
    g_cooldown = 13.9;
    g_break_through = 9;
}
