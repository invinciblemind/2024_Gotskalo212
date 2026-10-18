#include "Class_Tank.h"
#include "Class_76mm_S-54_gun.h"
#include "Class_85mm_D-5T_gun.h"
#include "Class_122mm_D2-5T_gun.h"
#include "Class_KV-1S.h"
#include <cstring>
#include <iostream>
using namespace std;

KV_1S::KV_1S() {
    strcpy(name, "КВ-1С");
    health = 1000;
    armor = 9.5;
    damage = gun_76mm_S_54::g_damage;
    cooldown = gun_76mm_S_54::g_cooldown;
    break_through = gun_76mm_S_54::g_break_through;
}

void KV_1S::install_gun(int number) {
    if (number == 1) {
        damage = gun_76mm_S_54::g_damage;
        cooldown = gun_76mm_S_54::g_cooldown;
        break_through = gun_76mm_S_54::g_break_through;
    }
    else if (number == 2) {
        damage = gun_85mm_D_5T::g_damage;
        cooldown = gun_85mm_D_5T::g_cooldown;
        break_through = gun_85mm_D_5T::g_break_through;
    }
    else if (number == 3) {
        damage = gun_122mm_D2_5T::g_damage;
        cooldown = gun_122mm_D2_5T::g_cooldown;
        break_through = gun_122mm_D2_5T::g_break_through;
    }
}

void KV_1S::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<6<<endl;
    cout<<"Тип: "<<"тяжелый"<<endl;
    Tank::print();
}

void KV_1S::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_76mm_S_54::print_gun_info();
    cout<<"  2:";
    gun_85mm_D_5T::print_gun_info();
    cout<<"  3:";
    gun_122mm_D2_5T::print_gun_info();
}
