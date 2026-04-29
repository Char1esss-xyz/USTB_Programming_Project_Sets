module control (
    input wire [7:0] instruction,     
    output reg [3:0] opcode,
    output reg [1:0] reg_rx,
    output reg [1:0] reg_ry,
    output reg [2:0] alu_op,
    output reg reg_write_enable,
    output reg alu_enable,
    output reg mul_enable,
    output reg show_enable,
    output reg pc_enable
);

    always @(*) begin
        opcode = instruction[7:4];
        reg_rx = instruction[3:2];
        reg_ry = instruction[1:0];
        alu_op = 3'b000;
        reg_write_enable = 1'b0;
        alu_enable = 1'b0;
        mul_enable = 1'b0;
        show_enable = 1'b0;
        pc_enable = 1'b1;
        
        case (instruction[7:4])
            4'b0000: begin // LOAD
                reg_write_enable = 1'b1;
            end
            
            4'b0001: begin // MOVE
                reg_write_enable = 1'b1;
            end
            
            4'b0010: begin // ADD
                alu_op = 3'b001;
                alu_enable = 1'b1;
                reg_write_enable = 1'b1;
            end
            
            4'b0011: begin // SUB
                alu_op = 3'b010;
                alu_enable = 1'b1;
                reg_write_enable = 1'b1;
            end
            
            4'b0100: begin // MUL
                mul_enable = 1'b1;
                reg_write_enable = 1'b1;
            end
            
            4'b1111: begin // SHOW
                show_enable = 1'b1;
                reg_write_enable = 1'b0;
            end
            
            default: begin
                pc_enable = 1'b0;
            end
        endcase
    end
    
endmodule