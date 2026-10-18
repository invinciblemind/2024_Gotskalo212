#include "Class_Tank.h"
#include "Class_57mm_ZiS-4_gun.h"
#include "Class_76mm_S-54_gun.h"

#ifndef __C_T_34
#define __C_T_34

class T_34: public Tank, virtual public gun_57mm_ZiS_4, virtual public gun_76mm_S_54 {
public:
    T_34();

    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif
