module seg_driver (
    input wire clk,
    input wire rst_n,
    input wire [7:0] display_data,
    output reg [6:0] seg,
    output reg [3:0] an
);

    reg [3:0] bcd_hundreds;
    reg [3:0] bcd_tens;
    reg [3:0] bcd_ones;
    always @(*) begin
        bcd_hundreds = display_data / 8'd100;
        bcd_tens = (display_data % 8'd100) / 8'd10;
        bcd_ones = display_data % 8'd10;
    end
    
    reg [17:0] scan_counter;
    wire [1:0] digit_select;
    reg [3:0] current_digit;
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            scan_counter <= 18'b0;
        end else begin
            scan_counter <= scan_counter + 1'b1;
        end
    end
    
    assign digit_select = scan_counter[17:16];
    
    always @(*) begin
        case (digit_select)
            2'd0: current_digit = bcd_ones;
            2'd1: current_digit = bcd_tens;
            2'd2: current_digit = bcd_hundreds;
            2'd3: current_digit = 4'h0;
            default: current_digit = 4'h0;
        endcase
    end
    
    always @(*) begin
        case (digit_select)
            2'd0: an = 4'b0001;
            2'd1: an = 4'b0010;
            2'd2: an = 4'b0100;
            2'd3: an = 4'b1000;
            default: an = 4'b0000;
        endcase
    end
    
    always @(*) begin
        case (current_digit)
            4'h0: seg = 7'b0111111;
            4'h1: seg = 7'b0000110;
            4'h2: seg = 7'b1011011;
            4'h3: seg = 7'b1001111;
            4'h4: seg = 7'b1100110;
            4'h5: seg = 7'b1101101;
            4'h6: seg = 7'b1111101;
            4'h7: seg = 7'b0000111;
            4'h8: seg = 7'b1111111;
            4'h9: seg = 7'b1101111;
            default: seg = 7'b0000000;
        endcase
    end
    
endmodule
