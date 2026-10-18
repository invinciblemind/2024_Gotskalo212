#include "Class_Tank.h"
#include "Class_152mm_ML-20_gun.h"
#include "Class_122mm_D-25S_gun.h"

#ifndef __C_SU_152
#define __C_SU_152

class SU_152: public Tank, virtual public gun_152mm_ML_20, virtual public gun_122mm_D_25S {
public:
    SU_152();

    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif
