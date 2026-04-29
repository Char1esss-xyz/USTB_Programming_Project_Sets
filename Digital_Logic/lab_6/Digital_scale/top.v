module uart_scale_top(
    input  wire sys_clk,
    input  wire sys_rst_n,
    input  wire uart_rx,
    output wire uart_tx,
    output wire [7:0] seg_out_left,
    output wire [3:0] seg_sel_left,
    output wire [7:0] seg_out_right,
    output wire [3:0] seg_sel_right
);

    wire [7:0] rx_data, tx_data;
    wire rx_valid, tx_valid;

    // wire loopback_en = 1'b0;
    // reg [7:0] loop_tx_data;
    // reg loop_tx_valid;

    // always @(posedge sys_clk or negedge sys_rst_n) begin
    //     if(!sys_rst_n) begin
    //         loop_tx_data <= 8'd0;
    //         loop_tx_valid <= 1'b0;
    //     end else if(loopback_en && rx_valid) begin
    //         loop_tx_data <= rx_data;
    //         loop_tx_valid <= 1'b1;
    //     end else begin
    //         loop_tx_valid <= 1'b0;
    //     end
    // end

    // wire [7:0] tx_data_mux = loopback_en ? loop_tx_data : tx_data;
    // wire tx_valid_mux = loopback_en ? loop_tx_valid : tx_valid;
    
    wire [7:0] tx_data_mux = tx_data;
    wire tx_valid_mux = tx_valid;
    
    wire [3:0] u_price, u_weight;
    wire u_pulse_accum, u_pulse_clear, u_pulse_price_set;

    wire [7:0] core_current_total;
    wire [7:0] calc_total = u_price * u_weight;
    wire [7:0] seg_out_left_int, seg_out_right_int;
    wire [3:0] seg_sel_left_int, seg_sel_right_int;

    wire is_accum_state = (current_state == 2'd1);

    wire [3:0] acc_t_thou = monitor_acc / 1000;
    wire [3:0] acc_t_hun = (monitor_acc % 1000) / 100;
    wire [3:0] acc_t_ten = (monitor_acc % 100) / 10;
    wire [3:0] acc_t_one = monitor_acc % 10;

    wire [3:0] l_d7 = is_accum_state ? 4'hA : d7;
    wire [3:0] l_d6 = is_accum_state ? 4'hC : d6;
    wire [3:0] l_d5 = is_accum_state ? accum_count_ten : d5;
    wire [3:0] l_d4 = is_accum_state ? accum_count_one : d4;
    wire [3:0] l_d3 = is_accum_state ? 4'd14 : d3;
    wire [3:0] l_d2 = is_accum_state ? 4'd14 : d2;
    wire [3:0] l_d1 = is_accum_state ? 4'd14 : d1;
    wire [3:0] l_d0 = is_accum_state ? 4'd14 : d0;

    wire [3:0] r_d7 = is_accum_state ? 4'd14 : d7;
    wire [3:0] r_d6 = is_accum_state ? 4'd14 : d6;
    wire [3:0] r_d5 = is_accum_state ? 4'd14 : d5;
    wire [3:0] r_d4 = is_accum_state ? 4'd14 : d4;
    wire [3:0] r_d3 = is_accum_state ? acc_t_thou : d3;
    wire [3:0] r_d2 = is_accum_state ? acc_t_hun : d2;
    wire [3:0] r_d1 = is_accum_state ? acc_t_ten : d1;
    wire [3:0] r_d0 = is_accum_state ? acc_t_one : d0;
    
    uartAdapter u_uart_phy(
        .clk(sys_clk),
        .resetn(sys_rst_n),
        .rx(uart_rx),
        .tx(uart_tx),
        .wdata(tx_data_mux),
        .wvalid(tx_valid_mux),
        .rdata(rx_data),
        .rvalid(rx_valid)
    );

    uart_cmd_parser u_parser(
        .clk(sys_clk),
        .rst_n(sys_rst_n),
        .rx_data(rx_data),
        .rx_valid(rx_valid),
        .price_out(u_price),
        .weight_out(u_weight),
        .pulse_accum(u_pulse_accum),
        .pulse_clear(u_pulse_clear),
        .pulse_price_set(u_pulse_price_set)
    );

    wire [3:0] d7, d6, d5, d4, d3, d2, d1, d0;
    wire [1:0] current_state;
    wire [3:0] accum_count_ten, accum_count_one;

    scale_core u_core(
        .clk(sys_clk),
        .rst_n(sys_rst_n),
        .pulse_accum(u_pulse_accum),
        .pulse_clear(u_pulse_clear),
        .pulse_view(1'b0),
        .pulse_price_set(u_pulse_price_set),
        .weight(u_weight),
        .price(u_price),
        .d7(d7), .d6(d6), .d5(d5), .d4(d4),
        .d3(d3), .d2(d2), .d1(d1), .d0(d0),
        .monitor_accum_total(monitor_acc),
        .current_state(current_state),
        .monitor_accum_count_ten(accum_count_ten),
        .monitor_accum_count_one(accum_count_one)
    );

    wire [15:0] monitor_acc;

    uart_msg_sender u_sender(
        .clk(sys_clk),
        .rst_n(sys_rst_n),
        .current_total(calc_total),
        .accum_total(monitor_acc),
        .accum_flag(u_pulse_accum),
        .tx_data(tx_data),
        .tx_valid(tx_valid),
        .tx_ready(1'b1)
    );

    seg_driver u_seg_left(
        .clk(sys_clk),
        .rst_n(sys_rst_n),
        .data_in_7(4'd0), .data_in_6(4'd0), .data_in_5(4'd0), .data_in_4(4'd0),
        .data_in_3(l_d7),
        .data_in_2(l_d6),
        .data_in_1(l_d5),
        .data_in_0(l_d4),
        .seg_out_left(seg_out_left_int),
        .seg_out_right(),
        .seg_sel(seg_sel_left_int)
    );

    seg_driver u_seg_right(
        .clk(sys_clk),
        .rst_n(sys_rst_n),
        .data_in_7(4'd0), .data_in_6(4'd0), .data_in_5(4'd0), .data_in_4(4'd0),
        .data_in_3(r_d3),
        .data_in_2(r_d2),
        .data_in_1(r_d1),
        .data_in_0(r_d0),
        .seg_out_left(),
        .seg_out_right(seg_out_right_int),
        .seg_sel(seg_sel_right_int)
    );

    assign seg_out_left = seg_out_left_int;
    assign seg_sel_left = seg_sel_left_int;
    assign seg_out_right = seg_out_right_int;
    assign seg_sel_right = seg_sel_right_int;

endmodule