#include "Class_Tank.h"
#include "Class_57mm_ZiS-4_gun.h"
#include "Class_76mm_S-54_gun.h"
#include "Class_MT-25.h"
#include <cstring>
#include <iostream>
using namespace std;

MT_25::MT_25() {
    strcpy(name, "MT-25");
    health = 820;
    armor = 4.5;
    damage = gun_57mm_ZiS_4::g_damage;
    cooldown = gun_57mm_ZiS_4::g_cooldown;
    break_through = gun_57mm_ZiS_4::g_break_through;
}

void MT_25::install_gun(int number) {
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

void MT_25::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<6<<endl;
    cout<<"Тип: "<<"легкий"<<endl;
    Tank::print();
}

void MT_25::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_57mm_ZiS_4::print_gun_info();
    cout<<"  2:";
    gun_76mm_S_54::print_gun_info();
}
