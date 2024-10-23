#include <iostream>
#include <chrono>
#include <thread>
#include "Vtop.h"        // Change this to Vtop.h instead of Vcounter.h
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    // init top verilog instance
    Vtop* top = new Vtop;   // Instantiate Vtop instead of Vcounter

    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // init vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;
    top->v = vbdValue();  // Input value to preload
    vbdSetMode(1);

    // run simulation for many clock cycles
    for (i = 0; i < 300; i++) {
        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++) {
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // +++++++ Send count value to Vbuddy
        vbdHex(4, (int(top->bcd) >> 16) & 0xF);  // Updated to 'bcd' from 'count'
        vbdHex(3, (int(top->bcd) >> 8) & 0xF);
        vbdHex(2, (int(top->bcd) >> 4) & 0xF);
        vbdHex(1, int(top->bcd) & 0xF);          // Updated from 'count' to 'bcd'
        // vbdPlot(int(top->bcd), 0, 255);        // Updated for BCD output
        vbdCycle(i+1);

        top->rst = (i < 2) | (i == 15);
        top->en = vbdFlag();

        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
