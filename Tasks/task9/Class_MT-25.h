#include "Class_Tank.h"
#include "Class_57mm_ZiS-4_gun.h"
#include "Class_76mm_S-54_gun.h"

#ifndef __C_MT_25
#define __C_MT_25

class MT_25: public Tank, virtual public gun_57mm_ZiS_4, virtual public gun_76mm_S_54 {
public:
    MT_25();

    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif
