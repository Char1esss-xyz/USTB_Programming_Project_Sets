module uart_cmd_parser(
    input  wire clk,
    input  wire rst_n,
    input  wire [7:0] rx_data,
    input  wire rx_valid,

    output reg [3:0] price_out,
    output reg [3:0] weight_out,
    output reg pulse_accum,
    output reg pulse_clear,
    output reg pulse_price_set
);

    localparam S_IDLE = 0;
    localparam S_CMD = 1;
    localparam S_DATA = 2;
    localparam S_TAIL = 3;

    reg [2:0] state;
    reg [7:0] cmd_reg;
    reg [7:0] data_reg;

    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            state <= S_IDLE;
            price_out <= 0;
            weight_out <= 0;
            pulse_accum <= 0;
            pulse_clear <= 0;
            pulse_price_set <= 0;
            cmd_reg <= 0;
            data_reg <= 0;
        end else begin
            pulse_accum <= 0;
            pulse_clear <= 0;
            pulse_price_set <= 0;

            if(rx_valid) begin
                case(state)
                    S_IDLE: begin
                        if(rx_data == 8'hAA) state <= S_CMD;
                    end
                    S_CMD: begin
                        cmd_reg <= rx_data;
                        state <= S_DATA;
                    end
                    S_DATA: begin
                        data_reg <= rx_data;
                        state <= S_TAIL;
                    end
                    S_TAIL: begin
                        if(rx_data == 8'h55) begin
                            case(cmd_reg)
                                8'h01: begin
                                    price_out <= data_reg[3:0];
                                    pulse_price_set <= 1'b1;
                                end
                                8'h02: weight_out <= data_reg[3:0];
                                8'h03: pulse_accum <= 1'b1;
                                8'h04: pulse_clear <= 1'b1;
                                default: ;
                            endcase
                        end
                        state <= S_IDLE;
                    end
                    default: state <= S_IDLE;
                endcase
            end
        end
    end
endmodule