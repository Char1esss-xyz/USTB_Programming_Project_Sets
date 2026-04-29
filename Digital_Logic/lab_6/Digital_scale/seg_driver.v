module seg_driver(
    input wire clk,
    input wire rst_n,
    input wire [3:0] data_in_7,
    input wire [3:0] data_in_6,
    input wire [3:0] data_in_5,
    input wire [3:0] data_in_4,
    input wire [3:0] data_in_3,
    input wire [3:0] data_in_2,
    input wire [3:0] data_in_1,
    input wire [3:0] data_in_0,
    output reg [7:0] seg_out_left,
    output reg [7:0] seg_out_right,
    output reg [3:0] seg_sel
);

    localparam CNT_MAX = 17'd100_000;
    localparam BLANK_TIME = 17'd5_000;

    reg [16:0] cnt;
    reg [1:0] scan_sel;
    reg [3:0] current_data;

    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            cnt <= 0;
            scan_sel <= 0;
        end else begin
            if(cnt >= CNT_MAX) begin
                cnt <= 0;
                scan_sel <= scan_sel + 1;
            end else begin
                cnt <= cnt + 1;
            end
        end
    end

    always @(*) begin
        case(scan_sel)
            2'd0: current_data = data_in_3;
            2'd1: current_data = data_in_2;
            2'd2: current_data = data_in_1;
            2'd3: current_data = data_in_0;
            default: current_data = 4'd14;
        endcase
    end

    always @(*) begin
        if(cnt < BLANK_TIME) begin
            seg_sel = 4'b0000;
        end else begin
            case(scan_sel)
                2'd0: seg_sel = 4'b1000;
                2'd1: seg_sel = 4'b0100;
                2'd2: seg_sel = 4'b0010;
                2'd3: seg_sel = 4'b0001;
                default: seg_sel = 4'b0000;
            endcase
        end
    end

    always @(*) begin
        case(current_data)
            4'd0: begin
                seg_out_left = 8'b0011_1111;
                seg_out_right = 8'b0011_1111;
            end
            4'd1: begin
                seg_out_left = 8'b0000_0110;
                seg_out_right = 8'b0000_0110;
            end
            4'd2: begin
                seg_out_left = 8'b0101_1011;
                seg_out_right = 8'b0101_1011;
            end
            4'd3: begin
                seg_out_left = 8'b0100_1111;
                seg_out_right = 8'b0100_1111;
            end
            4'd4: begin
                seg_out_left = 8'b0110_0110;
                seg_out_right = 8'b0110_0110;
            end
            4'd5: begin
                seg_out_left = 8'b0110_1101;
                seg_out_right = 8'b0110_1101;
            end
            4'd6: begin
                seg_out_left = 8'b0111_1101;
                seg_out_right = 8'b0111_1101;
            end
            4'd7: begin
                seg_out_left = 8'b0000_0111;
                seg_out_right = 8'b0000_0111;
            end
            4'd8: begin
                seg_out_left = 8'b0111_1111;
                seg_out_right = 8'b0111_1111;
            end
            4'd9: begin
                seg_out_left = 8'b0110_1111;
                seg_out_right = 8'b0110_1111;
            end
            4'd10: begin
                seg_out_left = 8'b0111_0111;
                seg_out_right = 8'b0111_0111;
            end
            4'd11: begin
                seg_out_left = 8'b0011_1001;
                seg_out_right = 8'b0011_1001;
            end
            4'd12: begin
                seg_out_left = 8'b0000_0000;
                seg_out_right = 8'b0000_0000;
            end
            default: begin
                seg_out_left = 8'b0000_0000;
                seg_out_right = 8'b0000_0000;
            end
        endcase
    end

endmodule
