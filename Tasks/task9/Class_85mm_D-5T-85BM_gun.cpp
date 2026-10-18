#include "Class_Gun.h"
#include <cstring>
#include "Class_85mm_D-5T-85BM_gun.h"

gun_85mm_D_5T_85BM::gun_85mm_D_5T_85BM() {
    strcpy(g_name, "85мм Д-5Т-85БМ");
    g_damage = 200;
    g_cooldown = 4.99;
    g_break_through = 7.6;
}
