module memory (
    input wire [3:0] read_addr,
    output reg [7:0] instruction_out
);
    
    reg [7:0] mem [0:15];
    // R0=5, R1=3, R2=0, R3=0
    initial begin
            // LOAD
            mem[0] = 8'b0000_10_10;  // LOAD R2, 10 (R2 = 10)
            mem[1] = 8'b1111_10_00;  // SHOW R2 (R2 = 10)
            
            // MOVE
            mem[2] = 8'b0001_11_01;  // MOVE R3, R1 (R3 = R1 = 3)
            mem[3] = 8'b1111_11_00;  // SHOW R3 (R3 = 3)
            
            // ADD
            mem[4] = 8'b0010_00_01;  // ADD R0, R1 (R0 = 5 + 3 = 8)
            mem[5] = 8'b1111_00_00;  // SHOW R0 (R0 = 8)
            
            // SUB
            mem[6] = 8'b0011_00_01;  // SUB R0, R1 (R0 = 8 - 3 = 5)
            mem[7] = 8'b1111_00_00;  // SHOW R0 (R0 = 5)
            
            // MUL
            mem[8] = 8'b0100_00_01;  // MUL R0, R1 (R0 = 5 * 3 = 15)
            mem[9] = 8'b1111_00_00;  // SHOW R0 (R0 = 15)
            
            // LOAD
            mem[10] = 8'b0000_00_11; // LOAD R0, 3 (R0 = 3)
            mem[11] = 8'b1111_00_00; // SHOW R0 (R0 = 3)
            
            // MOVE
            mem[12] = 8'b0001_00_10; // MOVE R0, R2 (R0 = R2 = 10)
            mem[13] = 8'b1111_00_00; // SHOW R0 (R0 = 10)
            
            // ADD
            mem[14] = 8'b0010_10_11; // ADD R2, R3 (R2 = 10 + 3 = 13)
            mem[15] = 8'b1111_10_00; // SHOW R2 (R2 = 13)
    end

    always @(*) begin
        instruction_out = mem[read_addr];
    end
    
endmodule
