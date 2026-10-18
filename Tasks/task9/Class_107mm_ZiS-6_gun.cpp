#include "Class_Gun.h"
#include <cstring>
#include "Class_107mm_ZiS-6_gun.h"

gun_107mm_ZiS_6::gun_107mm_ZiS_6() {
    strcpy(g_name, "107мм ЗиС-6");
    g_damage = 320;
    g_cooldown = 8.9;
    g_break_through = 9;
}
