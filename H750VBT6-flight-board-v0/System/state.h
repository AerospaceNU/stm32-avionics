#ifndef STATE_H_
#define STATE_H_

#include <cstdint>

#include "data.h"
#include "serialPrint.h"

class State{

    public:
        typedef uint32_t state_t;

        Data *data;

        State(Data *data);

        state_t run(void);
        state_t state_specific(void);

    private:
        void general(void);

};

#endif
