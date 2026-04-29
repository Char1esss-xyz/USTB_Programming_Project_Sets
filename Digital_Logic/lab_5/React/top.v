module reaction_timer_top (
    input wire clk,
    input wire rst_n_btn,
    input wire start_btn,
    input wire react_btn,

    output wire led_out,

    output wire [7:0] seg_code,
    output wire [3:0] seg_sel
);

    wire tick_1ms;
    wire start_pulse, react_pulse;
    wire [15:0] rand_val;
    wire [13:0] time_val_ms;

    clk_divider_1ms u_clk_div (
        .clk(clk),
        .rst_n(rst_n_btn),
        .tick_1ms(tick_1ms)
    );

    lfsr_random u_lfsr (
        .clk(clk),
        .rst_n(rst_n_btn),
        .rand_out(rand_val)
    );

    debouncer u_db_start (
        .clk(clk), 
        .rst_n(rst_n_btn), 
        .btn_in(start_btn), 
        .btn_pulse(start_pulse)
    );
    debouncer u_db_react (
        .clk(clk), 
        .rst_n(rst_n_btn), 
        .btn_in(react_btn), 
        .btn_pulse(react_pulse)
    );

    reaction_timer_fsm u_fsm (
        .clk(clk),
        .rst_n(rst_n_btn),
        .start_pulse(start_pulse),
        .react_pulse(react_pulse),
        .tick_1ms(tick_1ms),
        .random_val(rand_val),
        .led(led_out),
        .count_val(time_val_ms)
    );

    seg_scan_driver u_scan_disp (
        .clk(clk),
        .rst_n(rst_n_btn),
        .tick_1ms(tick_1ms),
        .number_in(time_val_ms),
        .seg_code(seg_code),
        .seg_sel(seg_sel)
    );

endmodule