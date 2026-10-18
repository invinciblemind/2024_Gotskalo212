#include "Class_Tank.h"
#include "Class_76mm_S-54_gun.h"
#include "Class_85mm_ZiS_S-53_gun.h"
#include "Class_85mm_D-5T-85BML_gun.h"
#include "Class_T-34-85.h"
#include <iostream>
#include <cstring>
using namespace std;

T_34_85::T_34_85() {
    strcpy(name, "T-34-85");
    health = 870;
    armor = 4.5;
    damage = gun_76mm_S_54::g_damage;
    cooldown = gun_76mm_S_54::g_cooldown;
    break_through = gun_76mm_S_54::g_break_through;
}

void T_34_85::install_gun(int number) {
    if (number == 1) {
        damage = gun_76mm_S_54::g_damage;
        cooldown = gun_76mm_S_54::g_cooldown;
        break_through = gun_76mm_S_54::g_break_through;
    }
    else if (number == 2) {
        damage = gun_85mm_ZiS_S_53::g_damage;
        cooldown = gun_85mm_ZiS_S_53::g_cooldown;
        break_through = gun_85mm_ZiS_S_53::g_break_through;
    }
    else if (number == 3) {
        damage = gun_85mm_D_5T_85BML::g_damage;
        cooldown = gun_85mm_D_5T_85BML::g_cooldown;
        break_through = gun_85mm_D_5T_85BML::g_break_through;
    }
}

void T_34_85::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<6<<endl;
    cout<<"Тип: "<<"средний"<<endl;
    Tank::print();
}

void T_34_85::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_76mm_S_54::print_gun_info();
    cout<<"  2:";
    gun_85mm_ZiS_S_53::print_gun_info();
    cout<<"  3:";
    gun_85mm_D_5T_85BML::print_gun_info();
}
