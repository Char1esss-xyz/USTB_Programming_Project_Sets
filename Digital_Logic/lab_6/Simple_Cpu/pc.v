module pc (
    input wire clk,
    input wire rst_n,
    input wire pc_enable,
    output reg [3:0] pc_out
);

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            pc_out <= 4'd0;
        end else if (pc_enable) begin
            if (pc_out == 4'd15) begin
                pc_out <= 4'd0;
            end else begin
                pc_out <= pc_out + 1'b1;
            end
        end
    end
    
endmodule