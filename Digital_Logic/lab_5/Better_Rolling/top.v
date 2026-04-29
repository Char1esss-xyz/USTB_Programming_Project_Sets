module top_rolling_display (
    input  wire clk,
    input  wire rst_n,
    input  wire [3:0] sw,
    input  wire confirm_btn,
    input  wire mode,
    output reg [3:0] seg_sel_right,
    output reg [3:0] seg_sel_left,
    output wire [7:0] seg_led_right,
    output wire [7:0] seg_led_left
);

    parameter PHONE_LEN = 11;

    wire tick_3hz;
    reg  [3:0] scroll_idx;
    wire [43:0] phone_digits;
    wire [3:0] num_cnt;

    reg  [3:0] display_digits [7:0];
    reg  [7:0] valid_mask;

    freq_div #(.CNT_MAX(33333333), .BITS(32)) u_div_shift (
        .clk(clk), .en_pulse(tick_3hz)
    );

    input_ctrl u_input_ctrl (
        .clk(clk),
        .rst_n(rst_n),
        .sw(sw),
        .confirm_btn(confirm_btn),
        .mode(mode),
        .phone_digits(phone_digits),
        .num_cnt(num_cnt)
    );

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            scroll_idx <= 0;
        end else if (mode == 1 && tick_3hz && num_cnt > 0) begin
            if (scroll_idx >= num_cnt + 7)
                scroll_idx <= 0;
            else
                scroll_idx <= scroll_idx + 1;
        end else if (mode == 0) begin
            scroll_idx <= 0;
        end
    end

    integer i;
    always @(*) begin
        for (i=0; i<8; i=i+1) display_digits[i] = 4'd0;
        valid_mask = 8'b00000000;

        if (!rst_n) begin
            valid_mask = 8'b00000000;
        end else begin
            for (i=0; i<8; i=i+1) begin
                if (mode == 0) begin
                    if (i < num_cnt) begin
                        valid_mask[i] = 1'b1;
                        if (num_cnt - 1 - i < 11)
                            display_digits[i] = phone_digits[(num_cnt - 1 - i)*4 +: 4];
                        else
                            display_digits[i] = 4'd0;
                    end else begin
                        valid_mask[i] = 1'b0;
                        display_digits[i] = 4'd0;
                    end
                end else if (mode == 1 && num_cnt > 0) begin
                    valid_mask[i] = 1'b1;
                    display_digits[i] = phone_digits[((i - scroll_idx + num_cnt) % num_cnt)*4 +: 4];
                end else begin
                    valid_mask[i] = 1'b0;
                end
            end
        end
    end

    reg [2:0] scan_counter;
    reg [3:0] current_disp_data;
    wire scan_en;

    freq_div #(.CNT_MAX(12500), .BITS(14)) u_div_scan (
        .clk(clk), .en_pulse(scan_en)
    );

    reg [3:0] next_data;
    reg       next_valid;

    always @(*) begin
        case (scan_counter)
            3'd0: begin next_data = display_digits[0]; next_valid = valid_mask[0]; end
            3'd1: begin next_data = display_digits[1]; next_valid = valid_mask[1]; end
            3'd2: begin next_data = display_digits[2]; next_valid = valid_mask[2]; end
            3'd3: begin next_data = display_digits[3]; next_valid = valid_mask[3]; end
            3'd4: begin next_data = display_digits[4]; next_valid = valid_mask[4]; end
            3'd5: begin next_data = display_digits[5]; next_valid = valid_mask[5]; end
            3'd6: begin next_data = display_digits[6]; next_valid = valid_mask[6]; end
            3'd7: begin next_data = display_digits[7]; next_valid = valid_mask[7]; end
            default: begin next_data = 4'd0; next_valid = 1'b0; end
        endcase
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            scan_counter   <= 0;
            seg_sel_right  <= 4'b0000;
            seg_sel_left   <= 4'b0000;
            current_disp_data <= 4'd0;
        end else begin
            if (scan_en) begin
                scan_counter <= scan_counter + 1;
                seg_sel_right <= 4'b0000;
                seg_sel_left  <= 4'b0000;
            end else begin
                current_disp_data <= next_data;

                if (next_valid) begin
                    case (scan_counter)
                        3'd0: begin seg_sel_right <= 4'b0001; seg_sel_left <= 4'b0000; end
                        3'd1: begin seg_sel_right <= 4'b0010; seg_sel_left <= 4'b0000; end
                        3'd2: begin seg_sel_right <= 4'b0100; seg_sel_left <= 4'b0000; end
                        3'd3: begin seg_sel_right <= 4'b1000; seg_sel_left <= 4'b0000; end
                        3'd4: begin seg_sel_right <= 4'b0000; seg_sel_left <= 4'b0001; end
                        3'd5: begin seg_sel_right <= 4'b0000; seg_sel_left <= 4'b0010; end
                        3'd6: begin seg_sel_right <= 4'b0000; seg_sel_left <= 4'b0100; end
                        3'd7: begin seg_sel_right <= 4'b0000; seg_sel_left <= 4'b1000; end
                        default: begin seg_sel_right <= 4'b0000; seg_sel_left <= 4'b0000; end
                    endcase
                end else begin
                    seg_sel_right <= 4'b0000;
                    seg_sel_left  <= 4'b0000;
                end
            end
        end
    end

    seg_driver e (
        .char_idx(current_disp_data),
        .seg_led(seg_led_right)
    );

    seg_driver u_driver_left (
        .char_idx(current_disp_data),
        .seg_led(seg_led_left)
    );
endmodule