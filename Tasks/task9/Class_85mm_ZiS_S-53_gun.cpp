#include "Class_Gun.h"
#include <cstring>
#include "Class_85mm_ZiS_S-53_gun.h"

gun_85mm_ZiS_S_53::gun_85mm_ZiS_S_53() {
    strcpy(g_name, "85мм ЗиС С-53");
    g_damage = 200;
    g_cooldown = 7.82;
    g_break_through = 6.4;
}
