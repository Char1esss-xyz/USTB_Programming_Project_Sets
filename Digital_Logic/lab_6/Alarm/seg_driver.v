module seg_display(
    input wire clk,
    input wire rst,
    input wire [3:0] num,
    input wire test_mode,
    output reg [7:0] seg,
    output reg [3:0] an
);

    reg [7:0] seg_raw;
    
    always @(*) begin
        case (num)
            4'b0000: seg_raw = 8'b0011_1111;
            4'b0001: seg_raw = 8'b0000_0110;
            4'b0010: seg_raw = 8'b0101_1011;
            4'b0011: seg_raw = 8'b0100_1111;
            4'b0100: seg_raw = 8'b0110_0110;
            4'b0101: seg_raw = 8'b0110_1101;
            4'b0110: seg_raw = 8'b0111_1101;
            4'b0111: seg_raw = 8'b0000_0111;
            4'b1000: seg_raw = 8'b0111_1111;
            4'b1001: seg_raw = 8'b0110_1111;
            default: seg_raw = 8'b0000_1000;
        endcase
    end
    
    always @(posedge clk or negedge rst) begin
        if (!rst) begin
            seg <= 8'b0000_0000;
            an <= 4'b1111;
        end else begin
            seg <= seg_raw;
            an <= 4'b0001;
        end
    end
    
endmodule