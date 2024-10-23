## Task 1 - Simulate an 8-bit binary counter
Partha Khanna
#### Verilog Script
`always_ff @ (posedge clk) begin
- Triggered on rising edge of clock signal
- `always_ff` signifies hat it is intended for sequential logic (flip flops)
`if (rst) count <= {WIDTH{1'b0}};
- If reset is high, count = 0
`else count <= count + {{WIDTH-1{1'b0}}, en};
- If reset is low, count is incremented
- `{{WIDTH-1{1'b0}}, en}` creates a vector where the uppermost bit is 0 and the LSB is equal to enable. 
- This is equivalent to doing count + 1 only is enable is high and is dynamic for the width of count.

![counter interal](images/counter_inners.jpg)

`verilator -Wall --cc --trace counter.sv --exe counter_tb.cpp`
- This calls verilator
- `-Wall` enables all warnings to help identify issues
- `--cc` generates a cycle-accurate C++ simulation of the SystemVerilog code
- `--trace` enables tracing, to generate a .vcd file
- `--exe` is to provide the testbench
This generates several files in obj_dir, critically one of these is Vcounter.mk 

`make -j -C obj_dir/ -f Vcounter.mk Vcounter`
- Build automation tool that reads from make
- `-j` is for multiple jobs in parallel
- `-C obj_dir/` is to change the directory to obj_dir prior to executing other commands
- `-f Vcounter.mk`  specifies the .mk file to use
- Final parameter is the target to be created

Running `obj_dir/Vcounter` generates the .vcd file.
In this case it can be viewed through WaveTrace. But for more advanced signals, we can open this file with GTKWave.

All the commands above can be put into a single bash script. (`doit.sh`)

## Task 2 - Linking Verilator simulation with Vbuddy
`USB/IP daemon` is used to connect the Vbuddy. It allows usb devices to be connected across virtual environments or operating systems.

Other aspects are editing the program to be used with VBuddy.
vbdHex() correlates to the 7 segment display
vbdPlot() allows for plotting functionality

## Task 3 - Vbuddy parameter & flag in one-shot mode
vbdValue() correlates to  the value of the rotary encoder.
Using a new coutner.sv with a load signal instead of enable. (`ld`)
Such that when `ld` is asserted, `v` is loaded into counter as a pre-set value.

VBuddy's flag has TOGGLE for default mode, every time encoder is pressed, flag will toggle at bottom of tft screen.

Using `vbdSetMode(1)`, you can set ONE-SHOT behaviour ->  flag set to 1 when pressed and is ARMED, once read it reverts to 0.
## Task 4 - Displaying BCD

See the sv files to understand how this works.
No major update to testbench.
```
module top #(
  parameter WIDTH = 8
)(
  // interface signals
  input  logic             clk,      // clock
  input  logic             rst,      // reset
  input  logic             en,       // enable
  input  logic [WIDTH-1:0] v,        // value to preload
  output logic [11:0]      bcd       // count output
);

  logic  [WIDTH-1:0]       count;    // interconnect wire
 
counter myCounter (
  .clk (clk),
  .rst (rst),
  .ld  (en),    // Correctly map 'en' to 'ld' of the counter
  .v   (v),     // Load value
  .count (count)
);

bin2bcd myDecoder (
  .x (count),
  .BCD (bcd)
);
endmodule
```
This links the two .sv files together, by including the parameters from counter and also send this to bin2bcd to convert the binary to binary coded decimal using the algorithm above. 