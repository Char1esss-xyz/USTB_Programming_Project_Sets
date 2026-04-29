module seg_driver (
    input  wire [3:0] char_idx,
    output reg  [7:0] seg_led
);

    always @(*) begin
        case (char_idx)
            4'd0: seg_led = 8'h3F;
            4'd1: seg_led = 8'h06;
            4'd2: seg_led = 8'h5B;
            4'd3: seg_led = 8'h4F;
            4'd4: seg_led = 8'h66;
            4'd5: seg_led = 8'h6D;
            4'd6: seg_led = 8'h7D;
            4'd7: seg_led = 8'h07;
            4'd8: seg_led = 8'h7F;
            4'd9: seg_led = 8'h6F;
            default: seg_led = 8'h00;
        endcase
    end
endmodule