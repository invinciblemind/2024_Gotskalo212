#include "Class_Tank.h"
#include "Class_85mm_D-5S_gun.h"
#include "Class_85mm_D-5S-85BM_gun.h"

#ifndef __C_SU_85
#define __C_SU_85

class SU_85: public Tank, virtual public gun_85mm_D_5S_85BM, virtual public gun_85mm_D_5S {
public:
    SU_85();

    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif
