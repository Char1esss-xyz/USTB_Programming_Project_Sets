`timescale 1ns / 1ps

module tb_fast_reaction();

    reg clk;
    reg rst_n;
    reg start_pulse;
    reg react_pulse;
    reg tick_1ms;
    reg [15:0] random_val;
    
    wire led;
    wire [13:0] count_val;

    reaction_timer_fsm u_fsm (
        .clk(clk),
        .rst_n(rst_n),
        .start_pulse(start_pulse),
        .react_pulse(react_pulse),
        .tick_1ms(tick_1ms),
        .random_val(random_val),
        .led(led),
        .count_val(count_val)
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        tick_1ms = 0;
        forever begin
            @(posedge clk); 
            tick_1ms = 1;
            @(posedge clk);
            tick_1ms = 0;
        end
    end

    initial begin
        rst_n = 0;
        start_pulse = 0;
        react_pulse = 0;
        random_val = 16'd0; 
        
        #50;
        rst_n = 1;
        #50;

        @(posedge clk);
        start_pulse = 1;
        @(posedge clk);
        start_pulse = 0;

        wait(u_fsm.state == 2'b01);
        
        @(posedge clk);
        
        force u_fsm.delay_counter = 14'd5;
        @(posedge clk);
        release u_fsm.delay_counter;

        wait(led == 1);
        
        repeat(20) @(posedge tick_1ms);

        @(posedge clk);
        react_pulse = 1;
        @(posedge clk);
        react_pulse = 0;

        #100;
        $stop;
    end

endmodule