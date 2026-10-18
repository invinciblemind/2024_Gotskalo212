#include "Class_Gun.h"
#include <cstring>
#include "Class_100mm_D-54S_gun.h"

gun_100mm_D_54S::gun_100mm_D_54S() {
    strcpy(g_name, "100мм Д-54С");
    g_damage = 310;
    g_cooldown = 7.48;
    g_break_through = 10.95;
}
