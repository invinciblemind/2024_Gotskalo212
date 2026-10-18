#include "Class_Tank.h"
#include "Class_85mm_ZiS_S-53_gun.h"
#include "Class_85mm_D-5T-85BM_gun.h"
#include "Class_T-43.h"
#include <iostream>
#include <cstring>
using namespace std;

T_43::T_43() {
    strcpy(name, "Т-43");
    health = 1250;
    armor = 7.5;
    damage = gun_85mm_ZiS_S_53::g_damage;
    cooldown = gun_85mm_ZiS_S_53::g_cooldown;
    break_through = gun_85mm_ZiS_S_53::g_break_through;
}

void T_43::install_gun(int number) {
    if (number == 1) {
        damage = gun_85mm_ZiS_S_53::g_damage;
        cooldown = gun_85mm_ZiS_S_53::g_cooldown;
        break_through = gun_85mm_ZiS_S_53::g_break_through;
    }
    else if (number == 2) {
        damage = gun_85mm_D_5T_85BM::g_damage;
        cooldown = gun_85mm_D_5T_85BM::g_cooldown;
        break_through = gun_85mm_D_5T_85BM::g_break_through;
    }
}

void T_43::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<7<<endl;
    cout<<"Тип: "<<"средний"<<endl;
    Tank::print();
}

void T_43::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_85mm_ZiS_S_53::print_gun_info();
    cout<<"  2:";
    gun_85mm_D_5T_85BM::print_gun_info();
}
