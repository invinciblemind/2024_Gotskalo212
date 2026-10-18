#include "Class_Tank.h"
#include "Class_57mm_ZiS-4_gun.h"
#include "Class_76mm_S-54_gun.h"
#include "Class_T-34.h"
#include <cstring>
#include <iostream>
using namespace std;

T_34::T_34() {
    strcpy(name, "T-34");
    health = 680;
    armor = 5;
    damage = gun_57mm_ZiS_4::g_damage;
    cooldown = gun_57mm_ZiS_4::g_cooldown;
    break_through = gun_57mm_ZiS_4::g_break_through;
}

void T_34::install_gun(int number) {
    if (number == 1) {
        damage = gun_57mm_ZiS_4::g_damage;
        cooldown = gun_57mm_ZiS_4::g_cooldown;
        break_through = gun_57mm_ZiS_4::g_break_through;
    }
    else if (number == 2) {
        damage = gun_76mm_S_54::g_damage;
        cooldown = gun_76mm_S_54::g_cooldown;
        break_through = gun_76mm_S_54::g_break_through;
    }
}

void T_34::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<5<<endl;
    cout<<"Тип: "<<"средний"<<endl;
    Tank::print();
}

void T_34::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_57mm_ZiS_4::print_gun_info();
    cout<<"  2:";
    gun_76mm_S_54::print_gun_info();
}
