module debounce(
    input wire clk,
    input wire rst_n,
    input wire key_in,
    output reg key_out
);
    localparam CNT_MAX = 2000000;

    reg [20:0] cnt;
    reg key_cnt_start;
    reg key_in_d0, key_in_d1;

    always @(posedge clk) begin
        key_in_d0 <= key_in;
        key_in_d1 <= key_in_d0;
    end
    
    wire pos_edge = key_in_d0 & ~key_in_d1;

    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            cnt <= 0;
            key_cnt_start <= 0;
            key_out <= 0;
        end else begin
            key_out <= 0;

            if(pos_edge && !key_cnt_start) begin
                key_cnt_start <= 1;
                cnt <= 0;
            end

            if(key_cnt_start) begin
                cnt <= cnt + 1;
                if(cnt >= CNT_MAX) begin
                    key_cnt_start <= 0;
                    if(key_in == 1) key_out <= 1;
                end
            end
        end
    end
endmodule