#include "Class_Gun.h"
#include <cstring>
#include "Class_76mm_S-54_gun.h"

gun_76mm_S_54::gun_76mm_S_54() {
    strcpy(g_name, "76мм С-54");
    g_damage = 160;
    g_cooldown = 7.57;
    g_break_through = 6.25;
}
