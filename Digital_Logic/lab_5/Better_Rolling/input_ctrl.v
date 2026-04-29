module input_ctrl(
    input wire clk,
    input wire rst_n,
    input wire [3:0] sw,
    input wire confirm_btn,
    input wire mode,
    output reg [43:0] phone_digits,
    output reg [3:0] num_cnt
);
    wire confirm_btn_debounced;
    reg confirm_btn_d, confirm_btn_dd;
    wire confirm_posedge;

    debouncer u_debouncer (
        .clk(clk),
        .rst_n(rst_n),
        .btn_in(confirm_btn),
        .btn_pulse(confirm_btn_debounced)
    );

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            confirm_btn_d <= 0;
            confirm_btn_dd <= 0;
        end else begin
            confirm_btn_d <= confirm_btn_debounced;
            confirm_btn_dd <= confirm_btn_d;
        end
    end
    assign confirm_posedge = (confirm_btn_d & ~confirm_btn_dd);

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            phone_digits <= 0;
            num_cnt <= 0;
        end else if (mode == 0) begin
            if (confirm_posedge && num_cnt < 11) begin
                    if (sw >= 4'd1 && sw <= 4'd9)
                        phone_digits[num_cnt*4 +: 4] <= sw;
                    else
                        phone_digits[num_cnt*4 +: 4] <= 4'd0;
                num_cnt <= num_cnt + 1;
            end
        end
    end
endmodule