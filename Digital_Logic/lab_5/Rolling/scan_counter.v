module scan_counter (
    input  wire clk,
    input  wire rst_n,
    input  wire scan_en,
    output reg  [1:0] scan_pos,
    output reg  [3:0] seg_sel
);

always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            scan_pos <= 2'd0;
        end else if (scan_en) begin
            scan_pos <= scan_pos + 1;
        end
    end

    always @(posedge clk) begin
        if (scan_en) begin
            scan_pos <= scan_pos + 1;
        end
    end

    always @(posedge clk) begin
        case (scan_pos)
            2'd0: seg_sel <= 4'b0001;
            2'd1: seg_sel <= 4'b0010;
            2'd2: seg_sel <= 4'b0100;
            2'd3: seg_sel <= 4'b1000;
            default: seg_sel <= 4'b0000;
        endcase
    end

endmodule