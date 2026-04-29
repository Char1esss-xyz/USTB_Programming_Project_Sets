module alu (
    input wire [7:0] operand_a,
    input wire [7:0] operand_b,
    input wire [2:0] alu_op,
    output reg [7:0] alu_result
);

    always @(*) begin
        case (alu_op)
            3'b001: alu_result = operand_a + operand_b;
            3'b010: alu_result = operand_a - operand_b;
            default: alu_result = 8'b0;
        endcase
    end
    
endmodule



