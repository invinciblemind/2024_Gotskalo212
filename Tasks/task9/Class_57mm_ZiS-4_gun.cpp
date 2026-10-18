#include "Class_Gun.h"
#include <cstring>
#include "Class_57mm_ZiS-4_gun.h"

gun_57mm_ZiS_4::gun_57mm_ZiS_4() {
    strcpy(g_name, "57мм ЗиС-4");
    g_damage = 90;
    g_cooldown = 6.71;
    g_break_through = 5.6;
}
