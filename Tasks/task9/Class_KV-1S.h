#include "Class_Tank.h"
#include "Class_76mm_S-54_gun.h"
#include "Class_85mm_D-5T_gun.h"
#include "Class_122mm_D2-5T_gun.h"

#ifndef __C_KV_1S
#define __C_KV_1S

class KV_1S: public Tank, virtual public gun_76mm_S_54, virtual public gun_122mm_D2_5T, virtual public gun_85mm_D_5T {
public:
    KV_1S();

    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif
