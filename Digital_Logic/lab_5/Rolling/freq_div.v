module freq_div #(
    parameter CNT_MAX = 10000,
    parameter BITS    = 16
)(
    input  wire clk,
    output reg  en_pulse
);

    reg [BITS-1:0] cnt;

    always @(posedge clk) begin
            if (cnt >= CNT_MAX - 1) begin
                cnt <= 0;
                en_pulse <= 1;
            end else begin
                cnt <= cnt + 1;
                en_pulse <= 0;
            end
        end
endmodule