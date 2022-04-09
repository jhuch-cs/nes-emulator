#include "./core/CPU.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    MainBus bus = MainBus();
    CPU cpu = CPU(bus);
    while (true) {
        cpu.step();
    }
}
