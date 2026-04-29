module lfsr_random (
    input wire clk,
    input wire rst_n,
    output wire [15:0] rand_out
);
    reg [15:0] r_lfsr;
    wire feedback = r_lfsr[15] ^ r_lfsr[13] ^ r_lfsr[12] ^ r_lfsr[10];

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) r_lfsr <= 16'hACE1;
        else r_lfsr <= {r_lfsr[14:0], feedback};
    end

    assign rand_out = r_lfsr;
endmodule