#!/bin/sh

# Cleanup previous simulation artifacts
rm -rf obj_dir
rm -f counter.vcd

# Run Verilator to translate SystemVerilog into C++, including the C++ testbench
verilator -Wall --cc --trace counter.sv --exe top_tb.cpp

# Attach Vbuddy
~/Documents/iac/lab0-devtools/tools/attach_usb.sh

# Build C++ project via make automatically generated by Verilator
make -j -C obj_dir/ -f Vcounter.mk Vcounter

# Run executable simulation file
obj_dir/Vtop