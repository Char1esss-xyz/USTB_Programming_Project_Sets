module trail_led (
    input  wire clk,
    input  wire rst_n,
    output reg [7:0] led_out
);

    reg [15:0] cnt_pwm;
    reg [23:0] cnt_shift;
    reg [2:0] active_led_idx;

    reg [15:0] led_duty [7:0];

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cnt_pwm <= 0;
        end else begin
            if (cnt_pwm >= 49999)
                cnt_pwm <= 0;
            else
                cnt_pwm <= cnt_pwm + 1;
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cnt_shift <= 0;
            active_led_idx <= 0;
        end else begin
            if (cnt_shift >= 12499999) begin
                cnt_shift <= 0;
                active_led_idx <= active_led_idx + 1;
            end else begin
                cnt_shift <= cnt_shift + 1;
            end
        end
    end

    integer i;
    reg [2:0] current_idx;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (i = 0; i < 8; i = i + 1) begin
                led_duty[i] <= 0;
            end
        end else begin
            for (i = 0; i < 8; i = i + 1) begin
                led_duty[i] <= 0;
            end

            for (i = 0; i < 5; i = i + 1) begin
                current_idx = (active_led_idx - i) & 3'b111;
                led_duty[current_idx] <= 49999 >> i;
            end
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            led_out <= 8'b0;
        end else begin
            for (i = 0; i < 8; i = i + 1) begin
                led_out[i] <= (cnt_pwm < led_duty[i]) ? 1'b1 : 1'b0;
            end
        end
    end

endmodule