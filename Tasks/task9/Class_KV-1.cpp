#include "Class_Tank.h"
#include "Class_85mm_F-30_gun.h"
#include "Class_122mm_U-11_gun.h"
#include "Class_KV-1.h"
#include <cstring>
#include <iostream>
using namespace std;

KV_1::KV_1() {
    strcpy(name, "КВ-1");
    health = 780;
    armor = 6.1;
    damage = gun_122mm_U_11::g_damage;
    cooldown = gun_122mm_U_11::g_cooldown;
    break_through = gun_122mm_U_11::g_break_through;
}

void KV_1::install_gun(int number) {
    if (number == 1) {
        damage = gun_122mm_U_11::g_damage;
        cooldown = gun_122mm_U_11::g_cooldown;
        break_through = gun_122mm_U_11::g_break_through;
    }
    else if (number == 2) {
        damage = gun_85mm_F_30::g_damage;
        cooldown = gun_85mm_F_30::g_cooldown;
        break_through = gun_85mm_F_30::g_break_through;
    }
}

void KV_1::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<5<<endl;
    cout<<"Тип: "<<"тяжелый"<<endl;
    Tank::print();
}

void KV_1::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_122mm_U_11::print_gun_info();
    cout<<"  2:";
    gun_85mm_F_30::print_gun_info();
}
