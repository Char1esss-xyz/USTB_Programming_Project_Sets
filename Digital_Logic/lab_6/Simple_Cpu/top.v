module top (
    input wire clk,
    input wire rst_n,
    output wire [6:0] seg,
    output wire [3:0] an
);

    reg [25:0] clk_divider;
    reg clk_cpu;
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            clk_divider <= 26'b0;
            clk_cpu <= 1'b0;
        end else begin
            clk_divider <= clk_divider + 1'b1;
            if (clk_divider == 26'd50000000) begin
                clk_cpu <= ~clk_cpu;
                clk_divider <= 26'b0;
            end
        end
    end
    
    simple_cpu u_simple_cpu (
        .clk(clk_cpu),
        .clk_display(clk),
        .rst_n(rst_n),
        .seg(seg),
        .an(an)
    );
    
endmodule

