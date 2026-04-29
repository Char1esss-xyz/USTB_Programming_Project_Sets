module shift_counter #(
    parameter MAX_IDX = 13
)(
    input  wire clk,
    input  wire rst_n,
    input  wire shift_en,
    output reg  [3:0] start_idx 
);

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            start_idx <= 0;
        end else if (shift_en) begin
            if (start_idx >= MAX_IDX - 1)
                start_idx <= 0;
            else
                start_idx <= start_idx + 1;
        end
    end
endmodule