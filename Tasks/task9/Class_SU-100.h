#include "Class_Tank.h"
#include "Class_85mm_D-5S_gun.h"
#include "Class_85mm_D-5S-85BM_gun.h"
#include "Class_100mm_D-10S_gun.h"
#include "Class_122mm_D2-5S_gun.h"

#ifndef __C_SU_100
#define __C_SU_100

class SU_100: public Tank, virtual public gun_85mm_D_5S, virtual public gun_85mm_D_5S_85BM, virtual public gun_100mm_D_10S, virtual public gun_122mm_D2_5S {
public:
    SU_100();

    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif
