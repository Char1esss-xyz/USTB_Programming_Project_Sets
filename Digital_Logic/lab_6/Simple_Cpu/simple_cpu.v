module simple_cpu (
    input wire clk,
    input wire clk_display,
    input wire rst_n,
    output wire [6:0] seg,
    output wire [3:0] an
);

    wire [3:0] pc_out;
    wire [7:0] instruction;
    wire [3:0] opcode;
    wire [1:0] reg_rx, reg_ry;
    wire [2:0] alu_op;
    wire reg_write_enable, alu_enable, mul_enable, show_enable, pc_enable;
    wire [7:0] reg_data_a, reg_data_b;
    wire [7:0] alu_result;
    wire [15:0] mul_result;
    wire [7:0] write_data;
    reg [7:0] display_data;
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            display_data <= 8'b0;
        end else if (show_enable) begin
            display_data <= reg_data_a;
        end
    end
    
    pc u_pc (
        .clk(clk),
        .rst_n(rst_n),
        .pc_enable(pc_enable),
        .pc_out(pc_out)
    );
    
    memory u_memory (
        .read_addr(pc_out),
        .instruction_out(instruction)
    );
    
    control u_control (
        .instruction(instruction),
        .opcode(opcode),
        .reg_rx(reg_rx),
        .reg_ry(reg_ry),
        .alu_op(alu_op),
        .reg_write_enable(reg_write_enable),
        .alu_enable(alu_enable),
        .mul_enable(mul_enable),
        .show_enable(show_enable),
        .pc_enable(pc_enable)
    );
    
    regfile u_regfile (
        .clk(clk),
        .rst_n(rst_n),
        .write_enable(reg_write_enable),
        .read_addr_a(reg_rx),
        .read_addr_b(reg_ry),
        .write_addr(reg_rx),
        .write_data(write_data),
        .read_data_a(reg_data_a),
        .read_data_b(reg_data_b)
    );
    
    alu u_alu (
        .operand_a(reg_data_a),
        .operand_b(reg_data_b),
        .alu_op(alu_op),
        .alu_result(alu_result)
    );
    
    multiplier u_multiplier (
        .clk(clk),
        .rst_n(rst_n),
        .mul_enable(mul_enable),
        .operand_a(reg_data_a),
        .operand_b(reg_data_b),
        .product(mul_result)
    );
    
    wire [7:0] load_immediate;
    assign load_immediate = {4'b0, reg_rx, reg_ry};
    
    assign write_data = (opcode == 4'b0000) ? load_immediate : // LOAD
                        (opcode == 4'b0001) ? reg_data_b : // MOVE
                        (opcode == 4'b0010) ? alu_result : // ADD
                        (opcode == 4'b0011) ? alu_result : // SUB
                        (opcode == 4'b0100) ? mul_result[7:0] : // MUL
                        (opcode == 4'b1111) ? 8'b0 : // SHOW
                        8'b0;
    
    seg_driver u_seg_driver (
        .clk(clk_display),
        .rst_n(rst_n),
        .display_data(display_data),
        .seg(seg),
        .an(an)
    );
    
endmodule
