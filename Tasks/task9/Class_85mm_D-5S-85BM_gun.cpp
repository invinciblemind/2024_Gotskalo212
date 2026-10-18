#include "Class_Gun.h"
#include "Class_85mm_D-5S-85BM_gun.h"
#include <cstring>

gun_85mm_D_5S_85BM::gun_85mm_D_5S_85BM() {
    strcpy(g_name, "85 мм Д-5С-85БМ");
    g_damage = 200;
    g_cooldown = 8.05;
    g_break_through = 7.2;
}
