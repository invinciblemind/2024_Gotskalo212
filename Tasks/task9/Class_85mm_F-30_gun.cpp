#include "Class_Gun.h"
#include <cstring>
#include "Class_85mm_F-30_gun.h"

gun_85mm_F_30::gun_85mm_F_30() {
    strcpy(g_name, "85мм Ф-30");
    g_damage = 200;
    g_cooldown = 10.55;
    g_break_through = 6;
}
