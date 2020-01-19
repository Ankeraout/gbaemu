#include <cstdint>

#include <gbaemu/gba/io.hpp>
#include <gbaemu/gba/timer.hpp>

namespace gbaemu::gba::timer {
    typedef struct {
        uint16_t reload;
        uint16_t counter;
        unsigned int prescaler;
        bool countUp;
        bool irq;
        bool operate;
    } timer_t;

    timer_t timer[4];
    uint_fast16_t prescaler;
    static inline const unsigned int prescalerValues[] = {1, 64, 256, 1024};

    void init() {
        for(int i = 0; i < 4; i++) {
            timer[i].operate = false;
        }
    }
    void cycle() {
        prescaler++;

        bool countUp = false;

        for(int i = 0; i < 4; i++) {
            if(timer[i].operate && (((i == 0 || !timer[i].countUp) && ((prescaler % timer[i].prescaler) == 0))) || (timer[i].countUp && countUp)) {
                timer[i].counter++;

                if(timer[i].counter == 0) {
                    countUp = true;
                    timer[i].counter = timer[i].reload;

                    if(timer[i].irq) {
                        io::set(io::IF, io::get(io::IF) | (0x0008 << i));
                    }
                } else {
                    countUp = false;
                }

                io::set(io::TM0CNT_L + 4 * i, timer[i].counter);
            }
        }
    }

    static inline void writeCallback_tmcnt_l(unsigned int n, uint16_t value) {
        timer[n].reload = value;
        io::set(io::TM0CNT_L + n * 4, timer[n].counter);
    }

    static inline void writeCallback_tmcnt_h(unsigned int n, uint16_t value) {
        tmcnt_h_t tmcnt_h;
        tmcnt_h.value = value;

        if(tmcnt_h.fields.operate && !timer[n].operate) {
            timer[n].counter = timer[n].reload;
            io::set(io::TM0CNT_L + 4 * n, timer[n].counter);
        }

        timer[n].prescaler = prescalerValues[tmcnt_h.fields.prescaler];
        timer[n].countUp = tmcnt_h.fields.countUp;
        timer[n].irq = tmcnt_h.fields.irq;
        timer[n].operate = tmcnt_h.fields.operate;
    }

    void writeCallback_tm0cnt_l(uint16_t value) {
        writeCallback_tmcnt_l(0, value);
    }

    void writeCallback_tm0cnt_h(uint16_t value) {
        writeCallback_tmcnt_h(0, value);
    }

    void writeCallback_tm1cnt_l(uint16_t value) {
        writeCallback_tmcnt_l(1, value);
    }

    void writeCallback_tm1cnt_h(uint16_t value) {
        writeCallback_tmcnt_h(1, value);
    }

    void writeCallback_tm2cnt_l(uint16_t value) {
        writeCallback_tmcnt_l(2, value);
    }

    void writeCallback_tm2cnt_h(uint16_t value) {
        writeCallback_tmcnt_h(2, value);
    }

    void writeCallback_tm3cnt_l(uint16_t value) {
        writeCallback_tmcnt_l(3, value);
    }

    void writeCallback_tm3cnt_h(uint16_t value) {
        writeCallback_tmcnt_h(3, value);
    }
}
