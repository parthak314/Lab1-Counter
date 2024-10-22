module counter #(
    parameter WIDTH = 8
)(
    // interface signals
    input logic          clk,    // clock
    input logic          rst,    // reset
    input logic          en,     // counter enable
    output logic [WIDTH-1:0] count  // count output
);

// Internal register to halt counting
always_ff @(posedge clk or posedge rst) begin
    if (rst) count <= {WIDTH{1'b0}};
    else     count <= count + {{WIDTH-1{1'b0}}, en};
end

endmodule
