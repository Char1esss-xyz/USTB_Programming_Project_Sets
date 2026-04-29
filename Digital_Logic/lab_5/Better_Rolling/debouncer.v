module debouncer (
    input wire clk,
    input wire rst_n,
    input wire btn_in,
    output reg btn_pulse
);

    parameter CNT_MAX = 1_000_000;

    reg [31:0] cnt;
    reg btn_stable;
    reg btn_sync_0, btn_sync_1;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            btn_sync_0 <= 0;
            btn_sync_1 <= 0;
        end else begin
            btn_sync_0 <= btn_in;
            btn_sync_1 <= btn_sync_0;
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cnt <= 0;
            btn_stable <= 0;
        end else begin
            if (btn_sync_1 != btn_stable) begin
                if (cnt < CNT_MAX) cnt <= cnt + 1;
                else begin
                    btn_stable <= btn_sync_1;
                    cnt <= 0;
                end
            end else begin
                cnt <= 0;
            end
        end
    end

    reg btn_stable_d;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            btn_stable_d <= 0;
            btn_pulse <= 0;
        end else begin
            btn_stable_d <= btn_stable;
            btn_pulse <= (btn_stable && !btn_stable_d);
        end
    end
endmodule