module multiplier (
    input wire clk,
    input wire rst_n,
    input wire mul_enable,
    input wire [7:0] operand_a,
    input wire [7:0] operand_b,
    output wire [15:0] product
);

    reg [15:0] temp_product;
    integer i;
    always @(*) begin
        temp_product = 16'b0;
        for (i = 0; i < 8; i = i + 1) begin
            if (operand_b[i]) begin
                temp_product = temp_product + (operand_a << i);
            end
        end
    end
    
    assign product = temp_product;
    
endmodule