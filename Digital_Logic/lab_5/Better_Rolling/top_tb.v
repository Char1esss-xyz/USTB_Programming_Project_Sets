`timescale 1ns / 1ps

module tb_phone_rolling();

    reg clk;
    reg rst_n;
    reg [3:0] sw;
    reg confirm_btn;
    reg mode;

    wire [3:0] seg_sel_right;
    wire [3:0] seg_sel_left;
    wire [7:0] seg_led_right;
    wire [7:0] seg_led_left;

    // 实例化顶层模块
    top_rolling_display uut (
        .clk(clk),
        .rst_n(rst_n),
        .sw(sw),
        .confirm_btn(confirm_btn),
        .mode(mode),
        .seg_sel_right(seg_sel_right),
        .seg_sel_left(seg_sel_left),
        .seg_led_right(seg_led_right),
        .seg_led_left(seg_led_left)
    );

    // 【关键】加速仿真：覆盖原始代码中的大参数
    // 将 33,333,333 改为 20，让滚动飞快
    defparam uut.u_div_shift.CNT_MAX = 20;    
    // 将 12,500 改为 4，让扫描飞快
    defparam uut.u_div_scan.CNT_MAX = 4;      
    // 将 1,000,000 改为 5，让按键消抖瞬间完成
    defparam uut.u_input_ctrl.u_debouncer.CNT_MAX = 5; 

    initial begin
        clk = 0;
        forever #5 clk = ~clk; // 100MHz 时钟
    end

    initial begin
        rst_n = 0;
        sw = 0;
        confirm_btn = 0;
        mode = 0; // 初始为输入模式

        #100;
        rst_n = 1;
        #50;

        // --- 阶段 1：模拟输入电话号码 "123" ---
        // 输入 '1'
        sw = 4'd1;
        press_btn(); 

        // 输入 '2'
        sw = 4'd2;
        press_btn();

        // 输入 '3'
        sw = 4'd3;
        press_btn();
        
        // 观察一段时间：此时数码管应静止显示 "...123"
        #200;

        // --- 阶段 2：切换到滚动模式 ---
        mode = 1;

        // 观察一段时间：数字应该开始向左循环滚动
        #1000;

        $stop;
    end

    // 模拟按键按下的任务
    task press_btn;
    begin
        @(posedge clk);
        confirm_btn = 1;
        #100; // 按住时间需超过消抖阈值(5个周期)
        confirm_btn = 0;
        #100; // 松开等待
    end
    endtask

endmodule