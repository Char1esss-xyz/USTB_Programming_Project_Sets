module breathing_led(
    input wire clk,
    input wire rst_n,
    output wire led_out
);

    parameter CLK_FREQ = 100_000_000;
    parameter PWM_FREQ = 1_000;
    parameter BREATH_FREQ = 1;

    localparam PWM_PERIOD = CLK_FREQ / PWM_FREQ - 1;
    localparam BREATH_STEP = PWM_FREQ / (2 * BREATH_FREQ);

    reg [$clog2(PWM_PERIOD):0] cnt_pwm;
    reg [$clog2(BREATH_STEP):0] cnt_step;
    reg direction;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cnt_pwm <= 0;
        end else begin
            if (cnt_pwm < PWM_PERIOD) begin
                cnt_pwm <= cnt_pwm + 1;
            end else begin
                cnt_pwm <= 0;
            end
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cnt_step <= 0;
            direction <= 1'b1;
        end else if (cnt_pwm == PWM_PERIOD) begin
            if (direction) begin
                if (cnt_step < BREATH_STEP - 1) begin
                    cnt_step <= cnt_step + 1;
                end else begin
                    cnt_step <= cnt_step - 1;
                    direction <= 1'b0;
                end
            end else begin
                if (cnt_step > 1) begin
                    cnt_step <= cnt_step - 1;
                end else begin
                    cnt_step <= cnt_step + 1;
                    direction <= 1'b1;
                end
            end
        end
    end

    wire [$clog2(PWM_PERIOD):0] current_duty;
    assign current_duty = (cnt_step * PWM_PERIOD) / BREATH_STEP;
    assign led_out = (cnt_pwm < current_duty) ? 1'b1 : 1'b0;

endmodule