module uart_msg_sender(
    input wire clk,
    input wire rst_n,
    input wire [7:0] current_total,
    input wire [15:0] accum_total,
    input wire accum_flag,
    output reg [7:0] tx_data,
    output reg tx_valid,
    input wire tx_ready
);

    reg [7:0] last_current_total;
    reg send_busy;
    reg [2:0] send_step;
    reg [7:0] send_type;
    reg [15:0] send_value;

    reg [19:0] delay_cnt;

    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            last_current_total <= 0;
            send_busy <= 0;
            tx_valid <= 0;
            tx_data <= 0;
            send_step <= 0;
            delay_cnt <= 0;
        end else begin
            tx_valid <= 0;

            if(!send_busy) begin
                if(accum_flag) begin
                    send_busy <= 1;
                    send_type <= 8'h20;
                    send_value <= accum_total;
                    send_step <= 0;
                    delay_cnt <= 0;
                end
                else if(current_total != last_current_total) begin
                    last_current_total <= current_total;
                    send_busy <= 1;
                    send_type <= 8'h10;
                    send_value <= {8'd0, current_total};
                    send_step <= 0;
                    delay_cnt <= 0;
                end
            end else begin
                if(delay_cnt < 100000) begin
                    delay_cnt <= delay_cnt + 1;
                end else begin
                    delay_cnt <= 0;
                    
                    case(send_step)
                        0: begin
                            tx_data <= 8'hBB;
                            tx_valid <= 1;
                            send_step <= 1;
                        end
                        1: begin
                            tx_data <= send_type;
                            tx_valid <= 1;
                            send_step <= 2;
                        end
                        2: begin
                            tx_data <= send_value[15:8];
                            tx_valid <= 1;
                            send_step <= 3;
                        end
                        3: begin
                            tx_data <= send_value[7:0];
                            tx_valid <= 1;
                            send_step <= 4;
                        end
                        4: begin
                            tx_data <= 8'hEE;
                            tx_valid <= 1;
                            send_step <= 5;
                        end
                        5: begin
                            send_busy <= 0;
                        end
                        default: send_busy <= 0;
                    endcase
                end
            end
        end
    end

endmodule