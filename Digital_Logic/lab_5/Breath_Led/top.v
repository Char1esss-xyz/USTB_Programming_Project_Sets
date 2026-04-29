
module top (
    input wire FPGA_CLK_100,
    input wire RESET_N,
    input wire led_mode,
    output wire [7:0] LEDS
);


    reg [7:0] led_out;
    wire [7:0] led_1;
    wire [7:0] led_2;
    wire breathing_led;


    trail_led u_trail_led (
        .clk (FPGA_CLK_100),
        .rst_n (RESET_N),
        .led_out(led_1)
    );
    breathing_led u_breathing_led (
        .clk (FPGA_CLK_100),
        .rst_n (RESET_N),
        .led_out(breathing_led)
    );



    always @(posedge FPGA_CLK_100 or negedge RESET_N) begin
        if (!RESET_N) begin
            led_out <= 8'b0;
        end else begin
            case (led_mode)
                1'b0: led_out <= {7'b0, breathing_led};
                1'b1: led_out <= led_1;
                default: led_out <= 8'b0;
            endcase
        end
    end


    assign LEDS = led_out;

endmodule