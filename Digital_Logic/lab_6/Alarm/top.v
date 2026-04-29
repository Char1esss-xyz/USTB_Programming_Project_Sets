`timescale 1ns / 1ps

module top(
    input wire clk, 
    input wire rst,  
    input wire B1,
    input wire B2,
    input wire K1,
    input wire K2,
    output wire L1,
    output wire [7:0] seg,
    output wire [3:0] an
);

    wire B1_pulse;
    wire B2_pulse;
    
    reg alarm_state;
    reg alarm_state_r;
    
    reg [3:0] alarm_count;
    
    reg [31:0] blink_counter;
    reg led_state;
    wire [31:0] blink_threshold;
    
    wire [1:0] frequency;
    
    wire test_mode = (K1 == 1'b1 && K2 == 1'b1);

    debounce debounce_B1 (
        .clk(clk),
        .rst(rst),
        .btn_in(B1),
        .btn_pulse(B1_pulse)
    );
    
    debounce debounce_B2 (
        .clk(clk),
        .rst(rst),
        .btn_in(B2),
        .btn_pulse(B2_pulse)
    );
    
    always @(posedge clk or negedge rst) begin
        if (!rst) begin
            alarm_state <= 1'b0;
            alarm_state_r <= 1'b0;
        end else begin
            if (B1_pulse) begin
                alarm_state_r <= alarm_state;
                alarm_state <= ~alarm_state;
            end else begin
                alarm_state_r <= alarm_state;
            end
        end
    end
    
    always @(posedge clk or negedge rst) begin
        if (!rst) begin
            alarm_count <= 4'd0;
        end else if (B2_pulse && alarm_state) begin
            alarm_count <= 4'd1;
        end else if (B1_pulse && alarm_state == 1'b0) begin
            if (alarm_count < 4'd9)
                alarm_count <= alarm_count + 1'b1;
            else
                alarm_count <= 4'd0;
        end
    end
    
    assign frequency = {K2, K1};
    
    wire [31:0] count_val = (alarm_count == 4'd0) ? 32'd1 : {28'd0, alarm_count};
    wire [31:0] denom = ((frequency == 2'b00) ? 32'd1 :
                         (frequency == 2'b01) ? 32'd2 :
                         (frequency == 2'b10) ? 32'd4 : 32'd8) * count_val;
    assign blink_threshold = 32'd25000000 / denom;
    
    always @(posedge clk or negedge rst) begin
        if (!rst) begin
            blink_counter <= 32'd0;
            led_state <= 1'b0;
        end else if (alarm_state) begin
            if (blink_counter >= blink_threshold) begin
                blink_counter <= 32'd0;
                led_state <= ~led_state;
            end else begin
                blink_counter <= blink_counter + 1'b1;
            end
        end else begin
            blink_counter <= 32'd0;
            led_state <= 1'b0;
        end
    end
    
    assign L1 = alarm_state ? led_state : 1'b0;
    
    seg_display seg_display_inst (https://open.bigmodel.cn/api/paas/v4/chat/completions
        .clk(clk),
        .rst(rst),
        .num(alarm_count),
        .test_mode(test_mode),
        .seg(seg),
        .an(an)
    );

endmodule