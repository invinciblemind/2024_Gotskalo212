#include "Class_Tank.h"
#include "Class_122mm_U-11_gun.h"
#include "Class_107mm_ZiS-6_gun.h"
#include "Class_152mm_M-10_gun.h"
#include "Class_KV-2.h"
#include <cstring>
#include <iostream>
using namespace std;

KV_2::KV_2() {
    strcpy(name, "КВ-2");
    health = 950;
    armor = 7.5;
    damage = gun_122mm_U_11::g_damage;
    cooldown = gun_122mm_U_11::g_cooldown;
    break_through = gun_122mm_U_11::g_break_through;
}

void KV_2::install_gun(int number) {
    if (number == 1) {
        damage = gun_122mm_U_11::g_damage;
        cooldown = gun_122mm_U_11::g_cooldown;
        break_through = gun_122mm_U_11::g_break_through;
    }
    else if (number == 2) {
        damage = gun_107mm_ZiS_6::g_damage;
        cooldown = gun_107mm_ZiS_6::g_cooldown;
        break_through = gun_107mm_ZiS_6::g_break_through;
    }
    else if (number == 3) {
        damage = gun_152mm_M_10::g_damage;
        cooldown = gun_152mm_M_10::g_cooldown;
        break_through = gun_152mm_M_10::g_break_through;
    }
}

void KV_2::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<6<<endl;
    cout<<"Тип: "<<"тяжелый"<<endl;
    Tank::print();
}

void KV_2::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_122mm_U_11::print_gun_info();
    cout<<"  2:";
    gun_107mm_ZiS_6::print_gun_info();
    cout<<"  3:";
    gun_152mm_M_10::print_gun_info();
}
