module debounce (
    input wire clk,       
    input wire rst, 
    input wire btn_in,          
    output reg btn_pulse 
);
    reg [19:0] cnt;
    reg btn_in_r;
    
    localparam CNT_MAX = 20'd1000000;
    
    always @(posedge clk or negedge rst) begin
        if (!rst) begin
            cnt <= 20'd0;
            btn_in_r <= 1'b0;
            btn_pulse <= 1'b0;
        end else begin
            btn_pulse <= 1'b0;

            if (btn_in == 1'b0 && btn_in_r == 1'b1) begin
                cnt <= 20'd1;
                btn_in_r <= btn_in;
            end else if (btn_in == 1'b0 && cnt > 20'd0) begin
                if (cnt >= CNT_MAX) begin
                    btn_pulse <= 1'b1;
                    cnt <= 20'd0;
                end else begin
                    cnt <= cnt + 1'b1;
                end
            end else if (btn_in == 1'b1) begin
                btn_in_r <= 1'b1;
                cnt <= 20'd0;
            end
        end
    end

endmodule
