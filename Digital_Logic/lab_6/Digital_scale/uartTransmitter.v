module uartTransmitter(
    input wire clk,
    input wire rst,
    output reg tx,
    input wire [7:0] wdata,
    input wire wvalid
);

    // CLK:100MHz, BAUDRATE:19200, OVERSAMPLING:16
    // Target Frequency = 19200 * 16 = 307200 Hz
    // Divider Coefficient = 100,000,000 / 307200 = 325
    localparam DIV_CNT = 325;

    reg [8:0] divider_1;
    reg [7:0] locker_wdata;
    reg locker_wvalid;
    reg wvalid_delay;
    reg [4:0] write_counter;
    reg [3:0] cnt;
    reg wdone;

    wire b_clk;
    wire s_clk;

    assign b_clk = (divider_1 == DIV_CNT - 1);
    assign s_clk = (write_counter == 'd15) && b_clk;

    always @(posedge clk) begin
        wvalid_delay <= wvalid;
    end

    always @(posedge clk)begin
        if(rst) 
            locker_wdata <= 'd0;
        else if(wvalid & ~wvalid_delay)
            locker_wdata <= wdata;
    end

    always @(posedge clk)begin
        if(rst) 
            locker_wvalid <= 'd0;
        else if(wvalid & ~wvalid_delay)
            locker_wvalid <= 'd1;
        else if(wdone)
            locker_wvalid <= 'd0;
    end

    always @(posedge clk) begin
        if(rst)
            write_counter <= 'd0;
        else if(s_clk)
            write_counter <= 'd0;
        else if(b_clk)
            write_counter <= write_counter + 'd1;
    end

    always @(posedge clk) begin
        if(rst || b_clk)
            divider_1 <= 'd0;
        else
            divider_1 <= divider_1 + 'd1;
    end

    always @(posedge clk) begin
        if(rst || (wdone && s_clk))
            cnt <= 'd0;
        else if(locker_wvalid && s_clk) begin
            if(cnt == 'd9)
                cnt <= 'd0;
            else
                cnt <= cnt + 'd1;
        end
    end

    always @(posedge clk) begin
        if(rst) begin
            tx <= 'd1;
        end else if(locker_wvalid) begin
            if(s_clk) begin
                case (cnt)
                    'd0: tx <= 'd0;
                    'd1: tx <= locker_wdata[0];
                    'd2: tx <= locker_wdata[1];
                    'd3: tx <= locker_wdata[2];
                    'd4: tx <= locker_wdata[3];
                    'd5: tx <= locker_wdata[4];
                    'd6: tx <= locker_wdata[5];
                    'd7: tx <= locker_wdata[6];
                    'd8: tx <= locker_wdata[7];
                    'd9: tx <= 'd1;
                    default: tx <= 'd1;
                endcase
            end
        end else begin
            tx <= 'd1;
        end
    end

    always @(posedge clk) begin
        if(rst)
            wdone <= 'd0;
        else if((cnt == 'd9) && s_clk)
            wdone <= 'd1;
        else
            wdone <= 'd0;
    end

endmodule