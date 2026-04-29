`timescale 1ns / 1ps
module top_tb;

    reg clk;
    reg rst_n;
    
    reg B1;
    reg B2;
    reg K1;
    reg K2;
    
    wire L1;
    wire [7:0] seg;
    wire [3:0] an;
    
    top uut (
        .clk(clk),
        .rst(rst_n),
        .B1(B1),
        .B2(B2),
        .K1(K1),
        .K2(K2),
        .L1(L1),
        .seg(seg),
        .an(an)
    );
    
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    initial begin
        rst_n = 0;
        B1 = 0;
        B2 = 0;
        K1 = 0;
        K2 = 0;
        
        #100;
        rst_n = 1;
        #100;

        // 第一次触发报警
        B1 = 0;
        #100;
        B1 = 1;
        #100;
        
        // 解除报警
        B1 = 0;
        #100;
        B1 = 1;
        #100;

        // 第二次触发报警
        B1 = 0;
        #100;
        B1 = 1;
        #100;

        // 打开K1, 闪烁频率为2N
        K1 = 1;
        #100;

        // 打开K2, 闪烁频率为4N
        K1 = 0;
        K2 = 1;
        #100;

        // 打开K1和K2，闪烁频率为8N
        K1 = 1;
        K2 = 1;
        #100;

        // 关闭K1和K2
        K1 = 0;
        K2 = 0;
        #100;

        // 解除报警
        B1 = 0;
        #100;
        B1 = 1;
        #100;

        // 第三次触发报警
        B1 = 0;
        #100;
        B1 = 1;
        #100;
        
        // 重置报警次数
        B2 = 0;
        #100;
        B2 = 1;
        #100;
        B2 = 0;
    
        #1000;
        $finish;
    end

endmodule
