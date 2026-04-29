module clk_divider_1ms (
    input wire clk,
    input wire rst_n,
    output reg tick_1ms
);

    parameter CNT_MAX = 100_000; 

    reg [31:0] cnt;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cnt <= 0;
            tick_1ms <= 0;
        end else begin
            if (cnt >= CNT_MAX - 1) begin
                cnt <= 0;
                tick_1ms <= 1;  
            end else begin
                cnt <= cnt + 1;
                tick_1ms <= 0;
            end
        end
    end
endmodule