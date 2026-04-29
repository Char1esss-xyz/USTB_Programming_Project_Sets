module regfile (
    input wire clk,
    input wire rst_n,
    input wire write_enable,
    input wire [1:0] read_addr_a,
    input wire [1:0] read_addr_b,
    input wire [1:0] write_addr,
    input wire [7:0] write_data,
    output wire [7:0] read_data_a,
    output wire [7:0] read_data_b
);

    reg [7:0] registers [0:3];
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            registers[0] <= 8'd5;
            registers[1] <= 8'd3;
            registers[2] <= 8'd0;
            registers[3] <= 8'd0;   
        end else if (write_enable) begin
            registers[write_addr] <= write_data;
        end
    end
    
    assign read_data_a = registers[read_addr_a];
    assign read_data_b = registers[read_addr_b];
    
endmodule
