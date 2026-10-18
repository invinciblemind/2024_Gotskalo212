#include "Class_Tank.h"
#include "Class_85mm_ZiS_S-53_gun.h"
#include "Class_85mm_D-5T-85BML_gun.h"
#include "Class_LTTB.h"
#include <iostream>
#include <cstring>
using namespace std;

LTTB::LTTB() {
    strcpy(name, "ЛТТБ");
    health = 1100;
    armor = 8;
    damage = gun_85mm_ZiS_S_53::g_damage;
    cooldown = gun_85mm_ZiS_S_53::g_cooldown;
    break_through = gun_85mm_ZiS_S_53::g_break_through;
}

void LTTB::install_gun(int number) {
    if (number == 1) {
        damage = gun_85mm_ZiS_S_53::g_damage;
        cooldown = gun_85mm_ZiS_S_53::g_cooldown;
        break_through = gun_85mm_ZiS_S_53::g_break_through;
    }
    else if (number == 2) {
        damage = gun_85mm_D_5T_85BML::g_damage;
        cooldown = gun_85mm_D_5T_85BML::g_cooldown;
        break_through = gun_85mm_D_5T_85BML::g_break_through;
    }
}

void LTTB::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<7<<endl;
    cout<<"Тип: "<<"легкий"<<endl;
    Tank::print();
}

void LTTB::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_85mm_ZiS_S_53::print_gun_info();
    cout<<"  2:";
    gun_85mm_D_5T_85BML::print_gun_info();
}
