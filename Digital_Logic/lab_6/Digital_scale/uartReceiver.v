module uartReceiver(
    input wire clk,
    input wire rst,
    input wire rx,
    output wire [7:0] rdata,
    output reg rvalid
);
    localparam DIV_CNT = 325;

    reg [8:0] divider_1;
    reg [7:0] locker_rdata;
    reg rx_delay;
    reg [3:0] cnt;

    reg [15:0] filter_buffer;
    reg [4:0] ones_counter;
    wire rx_filtered;

    reg [3:0] read_counter;

    wire b_clk;
    wire s_clk;
    wire read_start;

    assign b_clk = (divider_1 == DIV_CNT - 1);
    assign s_clk = (read_counter == 'd8) && b_clk;
    assign rdata = locker_rdata;

    always @(posedge clk) begin
        if(rst) begin
            filter_buffer <= {16{1'b1}};
            ones_counter <= 'd16;
        end else begin
            filter_buffer <= {filter_buffer[14:0], rx};
            ones_counter <= ones_counter + rx - filter_buffer[15];
        end
    end

    assign rx_filtered = (ones_counter > 'd12) ? 1'b1 : 1'b0;  // 阈值：16中超过12个为1

    always @(posedge clk) begin
        if(rst)
            rx_delay <= 1'b1;
        else
            rx_delay <= rx_filtered;
    end

    assign read_start = (~rx_filtered & rx_delay);

    reg receiving;

    always @(posedge clk) begin
        if (rst)
            receiving <= 1'b0;
        else if (read_start && !receiving)
            receiving <= 1'b1;
        else if (cnt == 'd9 && s_clk)
            receiving <= 1'b0;
    end

    always @(posedge clk) begin
        if(rst || b_clk) begin
            divider_1 <= 'd0;
        end else begin
            if(read_start && !receiving)
                divider_1 <= 'd0;
            else
                divider_1 <= divider_1 + 'd1;
        end
    end

    always @(posedge clk) begin
        if(rst || (read_start && !receiving))
            read_counter <= 'd0;
        else if(b_clk) begin
            if(receiving)
                read_counter <= read_counter + 'd1;
            else
                read_counter <= 'd0;
        end
    end

    always @(posedge clk) begin
        if(rst) begin
            cnt <= 'd0;
            rvalid <= 1'b0;
            locker_rdata <= 'd0;
        end else if(receiving && s_clk) begin
            case (cnt)
                'd0: begin
                    if(rx_filtered == 0) cnt <= cnt + 1;
                end
                'd1, 'd2, 'd3, 'd4, 'd5, 'd6, 'd7, 'd8: begin
                    locker_rdata[cnt - 1] <= rx_filtered;
                    cnt <= cnt + 1;
                end
                'd9: begin
                    cnt <= 'd0;
                    rvalid <= 1'b1;
                end
                default: cnt <= 'd0;
            endcase
        end else begin
            rvalid <= 1'b0;
        end
    end

endmodule
