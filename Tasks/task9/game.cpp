#include "Class_Tank.h"

#include "Class_T-34.h"
#include "Class_SU-85.h"
#include "Class_KV-1.h"

#include "Class_MT-25.h"
#include "Class_T-34-85.h"
#include "Class_SU-100.h"
#include "Class_KV-1S.h"
#include "Class_KV-2.h"

#include "Class_LTTB.h"
#include "Class_T-43.h"
#include "Class_IS.h"
#include "Class_KV-3.h"
#include "Class_SU-152.h"
#include "Class_SU-100M1.h"

#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
    char command[50];
    
    T_34 tank_t_34;
    SU_85 tank_su_85;
    KV_1 tank_kv_1;
    
    MT_25 tank_mt_25;
    T_34_85 tank_t_34_85;
    KV_1S tank_kv_1s;
    KV_2 tank_kv_2;
    SU_100 tank_su_100;
    
    LTTB tank_lttb;
    T_43 tank_t_43;
    IS tank_is;
    KV_3 tank_kv_3;
    SU_152 tank_su_152;
    SU_100M1 tank_su_100m1;
    
    Tank team_red_5[] = {tank_t_34, tank_kv_1, tank_kv_1s, tank_t_34, tank_su_85, tank_su_85, tank_su_85};
    Tank team_blue_5[] = {tank_kv_1, tank_kv_1, tank_kv_1s, tank_kv_1s, tank_t_34, tank_t_34, tank_su_85};
    
    Tank team_red_6[] = {tank_t_34_85, tank_t_34_85, tank_mt_25, tank_mt_25, tank_su_100, tank_kv_1s, tank_kv_2};
    Tank team_blue_6[] = {tank_kv_2, tank_kv_2, tank_kv_1s, tank_kv_1s, tank_su_100, tank_su_100, tank_t_34_85};
    
    Tank team_red_7[] = {tank_kv_3, tank_kv_3, tank_is, tank_is, tank_t_43, tank_t_43, tank_su_152};
    Tank team_blue_7[] = {tank_su_100m1, tank_su_100m1, tank_su_100m1, tank_lttb, tank_t_43, tank_t_43, tank_kv_3};
    
    Tank team_red_m[] = {tank_t_34, tank_kv_1, tank_kv_1s, tank_su_100, tank_lttb, tank_kv_3, tank_su_152};
    Tank team_blue_m[] = {tank_su_85, tank_mt_25, tank_t_34_85, tank_kv_2, tank_t_43, tank_is, tank_su_100m1};
    
    cout<<"Введите команду вида <танк>.<функция>(...) или вида <функция>(...)"<<endl;
    cout<<"Допустимые функции танков - print(), print_about_guns(), install_gun(number)"<<endl;
    cout<<"Допустимые функции над танками - PvP(number), Battle(number,strategy_red,strategy_blue)"<<endl;
    cout<<"Допустимые номера PvP:\n   1: t_34 - t_34\n   2: t_34 - t_34_85\n   3: kv_1 - su_85\n   4: t_34 - su_100\n   5: kv_1s - mt_25\n   6: t_34_85 - su_100m1\n";
    cout<<"   7: t_34 - kv_3\n   8: su_100m1 - kv_3\n   9: su_152 - kv_2"<<endl;
    cout<<"Допустимые номера Battle:\n   5: бой пятерок\n   6: бой шестерок\n   7: бой семерок\n   m: бой всех уровней\n";
    cout<<"Допустимые стратегии Battle: random, focus, damage\n";
    cin>>command;
    while (command[0] != '0') {
        switch (command[0]) {
        case 't':
            switch (command[5]) {
                case 't':
                    switch (command[7]) {
                        case '3':
                            switch (command[9]) {
                                case '.': // tank_t_34
                                    switch (command[10]) {
                                        case 'p':
                                            switch (command[15]) {
                                                case '(': // print
                                                    tank_t_34.print();
                                                    break;
                                        
                                                case '_': // print_about_guns
                                                    tank_t_34.print_about_guns();
                                                    break;
                                            }
                                            break;
                                
                                        case 'i': // install_gun
                                            tank_t_34.install_gun((int)(command[22]-'0'));
                                            break;
                                    }
                                    break;
                                
                                case '_': // tank_t_34_85
                                    switch (command[13]) {
                                        case 'p':
                                            switch (command[18]) {
                                                case '(': // print
                                                    tank_t_34_85.print();
                                                    break;
                                        
                                                case '_': // print_about_guns
                                                    tank_t_34_85.print_about_guns();
                                                    break;
                                            }
                                            break;
                                
                                        case 'i': // install_gun
                                            tank_t_34_85.install_gun((int)(command[25]-'0'));
                                            break;
                                    }
                                    break;
                            }
                            break;
                        
                        case '4': // tank_t_43
                            switch (command[10]) {
                                case 'p':
                                    switch (command[15]) {
                                        case '(': // print
                                            tank_t_43.print();
                                            break;
                                
                                        case '_': // print_about_guns
                                            tank_t_43.print_about_guns();
                                            break;
                                    }
                                    break;
                        
                                case 'i': // install_gun
                                    tank_t_43.install_gun((int)(command[22]-'0'));
                                    break;
                            }
                            break;
                    }
                    break;
                
                case 's':
                    switch (command[8]) {
                        case '8': // tank_su_85
                            switch (command[11]) {
                                case 'p':
                                    switch (command[16]) {
                                        case '(': // print
                                            tank_su_85.print();
                                            break;
                                
                                        case '_': // print_about_guns
                                            tank_su_85.print_about_guns();
                                            break;
                                    }
                                    break;
                        
                                case 'i': // install_gun
                                    tank_su_85.install_gun((int)(command[23]-'0'));
                                    break;
                            }
                            break;
                        
                        case '1':
                            switch (command[9]) {
                                case '0':
                                    switch (command[11]) {
                                        case '.': // tank_su_100
                                            switch (command[12]) {
                                                case 'p':
                                                    switch (command[17]) {
                                                        case '(': // print
                                                            tank_su_100.print();
                                                            break;
                                                
                                                        case '_': // print_about_guns
                                                            tank_su_100.print_about_guns();
                                                            break;
                                                    }
                                                    break;
                                        
                                                case 'i': // install_gun
                                                    tank_su_100.install_gun((int)(command[24]-'0'));
                                                    break;
                                            }
                                            break;
                                        
                                        case 'm': // tank_su_100m1
                                            switch (command[14]) {
                                                case 'p':
                                                    switch (command[19]) {
                                                        case '(': // print
                                                            tank_su_100m1.print();
                                                            break;
                                                
                                                        case '_': // print_about_guns
                                                            tank_su_100m1.print_about_guns();
                                                            break;
                                                    }
                                                    break;
                                        
                                                case 'i': // install_gun
                                                    tank_su_100m1.install_gun((int)(command[26]-'0'));
                                                    break;
                                            }
                                            break;
                                    }
                                    break;
                                
                                case '5': // tank_su_152
                                    switch (command[12]) {
                                        case 'p':
                                            switch (command[17]) {
                                                case '(': // print
                                                    tank_su_152.print();
                                                    break;
                                        
                                                case '_': // print_about_guns
                                                    tank_su_152.print_about_guns();
                                                    break;
                                            }
                                            break;
                                
                                        case 'i': // install_gun
                                            tank_su_152.install_gun((int)(command[24]-'0'));
                                            break;
                                    }
                                    break;
                            }
                            break;
                    }
                    break;
                
                case 'k':
                    switch (command[8]) {
                        case '1':
                            switch (command[9]) {
                                case '.': // tank_kv_1
                                    switch (command[10]) {
                                        case 'p':
                                            switch (command[15]) {
                                                case '(': // print
                                                    tank_kv_1.print();
                                                    break;
                                        
                                                case '_': // print_about_guns
                                                    tank_kv_1.print_about_guns();
                                                    break;
                                            }
                                            break;
                                
                                        case 'i': // install_gun
                                            tank_kv_1.install_gun((int)(command[22]-'0'));
                                            break;
                                    }
                                    break;
                                
                                case 's': // tank_kv_1s
                                    switch (command[11]) {
                                        case 'p':
                                            switch (command[16]) {
                                                case '(': // print
                                                    tank_kv_1s.print();
                                                    break;
                                        
                                                case '_': // print_about_guns
                                                    tank_kv_1s.print_about_guns();
                                                    break;
                                            }
                                            break;
                                
                                        case 'i': // install_gun
                                            tank_kv_1s.install_gun((int)(command[23]-'0'));
                                            break;
                                    }
                                    break;
                            }
                            break;
                        
                        case '2': // tank_kv_2
                            switch (command[10]) {
                                case 'p':
                                    switch (command[15]) {
                                        case '(': // print
                                            tank_kv_2.print();
                                            break;
                                
                                        case '_': // print_about_guns
                                            tank_kv_2.print_about_guns();
                                            break;
                                    }
                                    break;
                        
                                case 'i': // install_gun
                                    tank_kv_2.install_gun((int)(command[22]-'0'));
                                    break;
                            }
                            break;
                        
                        case '3': // tank_kv_3
                            switch (command[10]) {
                                case 'p':
                                    switch (command[15]) {
                                        case '(': // print
                                            tank_kv_3.print();
                                            break;
                                
                                        case '_': // print_about_guns
                                            tank_kv_3.print_about_guns();
                                            break;
                                    }
                                    break;
                        
                                case 'i': // install_gun
                                    tank_kv_3.install_gun((int)(command[22]-'0'));
                                    break;
                            }
                            break;
                    }
                    break;
                
                case 'm': // tank_mt_25
                    switch (command[11]) {
                        case 'p':
                            switch (command[16]) {
                                case '(': // print
                                    tank_mt_25.print();
                                    break;
                        
                                case '_': // print_about_guns
                                    tank_mt_25.print_about_guns();
                                    break;
                            }
                            break;
                
                        case 'i': // install_gun
                            tank_mt_25.install_gun((int)(command[23]-'0'));
                            break;
                    }
                    break;
                
                case 'l': // tank_lttb
                    switch (command[10]) {
                        case 'p':
                            switch (command[15]) {
                                case '(': // print
                                    tank_lttb.print();
                                    break;
                        
                                case '_': // print_about_guns
                                    tank_lttb.print_about_guns();
                                    break;
                            }
                            break;
                
                        case 'i': // install_gun
                            tank_lttb.install_gun((int)(command[22]-'0'));
                            break;
                    }
                    break;
                
                case 'i': // tank_is
                    switch (command[8]) {
                        case 'p':
                            switch (command[13]) {
                                case '(': // print
                                    tank_lttb.print();
                                    break;
                        
                                case '_': // print_about_guns
                                    tank_lttb.print_about_guns();
                                    break;
                            }
                            break;
                
                        case 'i': // install_gun
                            tank_lttb.install_gun((int)(command[20]-'0'));
                            break;
                    }
                    break;
            }
            break;
        
        case 'P':
            switch (command[4]) {
                case '1':
                    PvP(tank_t_34, tank_t_34);
                    break;
                
                case '2':
                    PvP(tank_t_34, tank_t_34_85);
                    break;
                
                case '3':
                    PvP(tank_kv_1, tank_su_85);
                    break;
                
                case '4':
                    PvP(tank_t_34, tank_su_100);
                    break;
                
                case '5':
                    PvP(tank_kv_1s, tank_mt_25);
                    break;
                
                case '6':
                    PvP(tank_t_34_85, tank_su_100m1);
                    break;
                
                case '7':
                    PvP(tank_t_34, tank_kv_3);
                    break;
                
                case '8':
                    PvP(tank_su_100m1, tank_kv_3);
                    break;
                
                case '9':
                    PvP(tank_su_152, tank_kv_2);
                    break;
            }
            break;
        
        case 'B':
            switch (command[7]) {
                case '5':
                    switch (command[9]) {
                        case 'r':
                            switch (command[16]) {
                                case 'r':
                                    Battle(team_red_5, "random", team_blue_5, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_5, "random", team_blue_5, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_5, "random", team_blue_5, "damage");
                                    break;
                            }
                            break;
                        
                        case 'f':
                            switch (command[15]) {
                                case 'r':
                                    Battle(team_red_5, "focus", team_blue_5, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_5, "focus", team_blue_5, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_5, "focus", team_blue_5, "damage");
                                    break;
                            }
                            break;
                        
                        case 'd':
                            switch (command[16]) {
                                case 'r':
                                    Battle(team_red_5, "damage", team_blue_5, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_5, "damage", team_blue_5, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_5, "damage", team_blue_5, "damage");
                                    break;
                            }
                            break;
                    }
                    break;
                
                case '6':
                    switch (command[9]) {
                        case 'r':
                            switch (command[16]) {
                                case 'r':
                                    Battle(team_red_6, "random", team_blue_6, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_6, "random", team_blue_6, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_6, "random", team_blue_6, "damage");
                                    break;
                            }
                            break;
                        
                        case 'f':
                            switch (command[15]) {
                                case 'r':
                                    Battle(team_red_6, "focus", team_blue_6, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_6, "focus", team_blue_6, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_6, "focus", team_blue_6, "damage");
                                    break;
                            }
                            break;
                        
                        case 'd':
                            switch (command[16]) {
                                case 'r':
                                    Battle(team_red_6, "damage", team_blue_6, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_6, "damage", team_blue_6, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_6, "damage", team_blue_6, "damage");
                                    break;
                            }
                            break;
                    }
                    break;
                
                case '7':
                    switch (command[9]) {
                        case 'r':
                            switch (command[16]) {
                                case 'r':
                                    Battle(team_red_7, "random", team_blue_7, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_7, "random", team_blue_7, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_7, "random", team_blue_7, "damage");
                                    break;
                            }
                            break;
                        
                        case 'f':
                            switch (command[15]) {
                                case 'r':
                                    Battle(team_red_7, "focus", team_blue_7, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_7, "focus", team_blue_7, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_7, "focus", team_blue_7, "damage");
                                    break;
                            }
                            break;
                        
                        case 'd':
                            switch (command[16]) {
                                case 'r':
                                    Battle(team_red_7, "damage", team_blue_7, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_7, "damage", team_blue_7, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_7, "damage", team_blue_7, "damage");
                                    break;
                            }
                            break;
                    }
                    break;
                
                case 'm':
                    switch (command[9]) {
                        case 'r':
                            switch (command[16]) {
                                case 'r':
                                    Battle(team_red_m, "random", team_blue_m, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_m, "random", team_blue_m, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_m, "random", team_blue_m, "damage");
                                    break;
                            }
                            break;
                        
                        case 'f':
                            switch (command[15]) {
                                case 'r':
                                    Battle(team_red_m, "focus", team_blue_m, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_m, "focus", team_blue_m, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_m, "focus", team_blue_m, "damage");
                                    break;
                            }
                            break;
                        
                        case 'd':
                            switch (command[16]) {
                                case 'r':
                                    Battle(team_red_m, "damage", team_blue_m, "random");
                                    break;
                                
                                case 'f':
                                    Battle(team_red_m, "damage", team_blue_m, "focus");
                                    break;
                                
                                case 'd':
                                    Battle(team_red_m, "damage", team_blue_m, "damage");
                                    break;
                            }
                            break;
                    }
                    break;
            }
            break;
        }
        sleep(3);
        cout<<"Введите команду вида <танк>.<функция>(...) или вида <функция>(...)"<<endl;
        cout<<"Допустимые функции танков - print(), print_about_guns(), install_gun(number)"<<endl;
        cout<<"Допустимые функции над танками - PvP(number), Battle(number,strategy_red,strategy_blue)"<<endl;
        cout<<"Допустимые номера PvP:\n   1: t_34 - t_34\n   2: t_34 - t_34_85\n   3: kv_1 - su_85\n   4: t_34 - su_100\n   5: kv_1s - mt_25\n   6: t_34_85 - su_100m1\n";
        cout<<"   7: t_34 - kv_3\n   8: su_100m1 - kv_3\n   9: su_152 - kv_2"<<endl;
        cout<<"Допустимые номера Battle:\n   5: бой пятерок\n   6: бой шестерок\n   7: бой семерок\n   m: бой всех уровней\n";
        cout<<"Допустимые стратегии Battle: random, focus, damage\n";
        cin>>command;
    }
    
    return 0;
}

/*

tank_t_34.print()
tank_su_85.print()
tank_kv_1.print()

tank_mt_25.print()
tank_t_34_85.print()
tank_kv_1s.print()
tank_kv_2.print()
tank_su_100.print()

tank_lttb.print()
tank_t_43.print()
tank_is.print()
tank_kv_3.print()
tank_su_152.print()
tank_su_100m1.print()


tank_t_34.print_about_guns()
tank_su_85.print_about_guns()
tank_kv_1.print_about_guns()

tank_mt_25.print_about_guns()
tank_t_34_85.print_about_guns()
tank_kv_1s.print_about_guns()
tank_kv_2.print_about_guns()
tank_su_100.print_about_guns()

tank_lttb.print_about_guns()
tank_t_43.print_about_guns()
tank_is.print_about_guns()
tank_kv_3.print_about_guns()
tank_su_152.print_about_guns()
tank_su_100m1.print_about_guns()


tank_t_34.install_gun(2)
tank_su_85.install_gun(2)
tank_kv_1.install_gun(2)

tank_mt_25.install_gun(2)
tank_t_34_85.install_gun(2)
tank_kv_1s.install_gun(2)
tank_kv_2.install_gun(2)
tank_su_100.install_gun(2)

tank_lttb.install_gun(2)
tank_t_43.install_gun(2)
tank_is.install_gun(2)
tank_kv_3.install_gun(2)
tank_su_152.install_gun(2)
tank_su_100m1.install_gun(2)


PvP(1)

Battle(5,random,random)
Battle(5,random,focus)
Battle(5,random,damage)
Battle(5,focus,random)
Battle(5,focus,focus)
Battle(5,focus,damage)
Battle(5,damage,random)
Battle(5,damage,focus)
Battle(5,damage,damage)

Battle(6,random,random)
Battle(6,random,focus)
Battle(6,random,damage)
Battle(6,focus,random)
Battle(6,focus,focus)
Battle(6,focus,damage)
Battle(6,damage,random)
Battle(6,damage,focus)
Battle(6,damage,damage)

Battle(7,random,random)
Battle(7,random,focus)
Battle(7,random,damage)
Battle(7,focus,random)
Battle(7,focus,focus)
Battle(7,focus,damage)
Battle(7,damage,random)
Battle(7,damage,focus)
Battle(7,damage,damage)

Battle(m,random,random)
Battle(m,random,focus)
Battle(m,random,damage)
Battle(m,focus,random)
Battle(m,focus,focus)
Battle(m,focus,damage)
Battle(m,damage,random)
Battle(m,damage,focus)
Battle(m,damage,damage)

*/
