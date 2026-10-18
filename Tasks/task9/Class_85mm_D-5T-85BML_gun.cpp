#include "Class_Gun.h"
#include <cstring>
#include "Class_85mm_D-5T-85BML_gun.h"

gun_85mm_D_5T_85BML::gun_85mm_D_5T_85BML() {
    strcpy(g_name, "85мм Д-5Т-85БМЛ");
    g_damage = 200;
    g_cooldown = 7.93;
    g_break_through = 7.2;
}
