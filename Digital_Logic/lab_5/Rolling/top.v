module top_rolling_display (
    input  wire clk,
    input  wire rst_n,
    output wire [3:0] seg_sel,
    output wire [7:0] seg_led
);

    parameter TOTAL_CHARS = 12;

    wire tick_3hz;
    wire tick_scan;
    wire [3:0] start_idx;
    wire [1:0] scan_pos;
    wire [3:0] raw_addr_sum;
    reg  [3:0] final_addr;
    wire [5:0] current_char_code;

      freq_div #(.CNT_MAX(100000000), .BITS(32)) u_div_shift (
        .clk(clk), .en_pulse(tick_3hz)
    );

    freq_div #(.CNT_MAX(100000), .BITS(17)) u_div_scan (
        .clk(clk), .en_pulse(tick_scan)
    );
    shift_counter #(.MAX_IDX(TOTAL_CHARS)) u_shift_ctrl (
        .clk(clk),
        .rst_n(rst_n),
        .shift_en(tick_3hz),
        .start_idx(start_idx)
    );

    scan_counter u_scan_ctrl (
        .clk(clk),
        .rst_n(1'b1),
        .scan_en(tick_scan),
        .scan_pos(scan_pos),
        .seg_sel(seg_sel)
    );

    assign raw_addr_sum = start_idx + scan_pos;

    always @(*) begin
        if (raw_addr_sum >= TOTAL_CHARS)
            final_addr = raw_addr_sum - TOTAL_CHARS;
        else
            final_addr = raw_addr_sum;
    end

    string_rom u_rom (
        .addr(final_addr),
        .char_code(current_char_code)
    );

    seg_driver u_driver (
        .char_idx(current_char_code),
        .seg_led(seg_led)
    );

endmodule