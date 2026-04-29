module seg_scan_driver (
    input wire clk,
    input wire rst_n,
    input wire [13:0] number_in,
    input wire tick_1ms,
    output reg [7:0] seg_code,
    output reg [3:0] seg_sel
);

    reg [3:0] digit_thou, digit_hund, digit_tens, digit_unit;
    always @(*) begin
        digit_thou = number_in / 1000;
        digit_hund = (number_in % 1000) / 100;
        digit_tens = (number_in % 100) / 10;
        digit_unit = number_in % 10;
    end

    reg [1:0] scan_idx;
    always @(posedge clk) begin
        if (tick_1ms) begin
            scan_idx <= scan_idx + 1'b1;
        end
    end

    reg [3:0] current_digit_val;
    reg current_dot_en;
    reg [7:0] raw_seg;
    reg [3:0] active_high_sel;

    always @(*) begin
        case (scan_idx)
            2'b00: begin current_digit_val = digit_thou; current_dot_en = 1'b1; end
            2'b01: begin current_digit_val = digit_hund; current_dot_en = 1'b0; end
            2'b10: begin current_digit_val = digit_tens; current_dot_en = 1'b0; end
            2'b11: begin current_digit_val = digit_unit; current_dot_en = 1'b0; end
            default: begin current_digit_val = 0; current_dot_en = 0; end
        endcase
    end

    always @(*) begin
        case (current_digit_val)
            4'd0: raw_seg[6:0] = 7'b0111111; 
            4'd1: raw_seg[6:0] = 7'b0000110; 
            4'd2: raw_seg[6:0] = 7'b1011011; 
            4'd3: raw_seg[6:0] = 7'b1001111; 
            4'd4: raw_seg[6:0] = 7'b1100110; 
            4'd5: raw_seg[6:0] = 7'b1101101; 
            4'd6: raw_seg[6:0] = 7'b1111101; 
            4'd7: raw_seg[6:0] = 7'b0000111; 
            4'd8: raw_seg[6:0] = 7'b1111111; 
            4'd9: raw_seg[6:0] = 7'b1101111; 
            default: raw_seg[6:0] = 7'b0000000;
        endcase
        raw_seg[7] = current_dot_en;
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            seg_sel <= 4'h0;
            seg_code <= 8'h00;
        end else begin
            case (scan_idx)
                2'd0: seg_sel <= 4'b0001;
                2'd1: seg_sel <= 4'b0010;
                2'd2: seg_sel <= 4'b0100;
                2'd3: seg_sel <= 4'b1000;
            endcase
            
            seg_code <= raw_seg;
        end
    end

endmodule