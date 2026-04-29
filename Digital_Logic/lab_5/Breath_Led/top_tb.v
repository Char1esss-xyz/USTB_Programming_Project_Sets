`timescale 1ns / 1ps

module top_tb();

    reg clk;
    reg rst_n;
    wire led_out;

    breathing_led #(
        .CLK_FREQ(4000), 
        .PWM_FREQ(100),   
        .BREATH_FREQ(5)  
    ) uut (
        .clk(clk),
        .rst_n(rst_n),
        .led_out(led_out)
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        rst_n = 0;
        #50;
        rst_n = 1;
        
        #20000;
        $stop;
    end

endmodule