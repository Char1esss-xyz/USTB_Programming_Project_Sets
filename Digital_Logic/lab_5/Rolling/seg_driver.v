module seg_driver (
    input  wire [5:0] char_idx, 
    output reg  [7:0] seg_led   
);

    always @(*) begin
        case (char_idx)
            6'd0: seg_led = 8'h3F; // 0
            6'd1: seg_led = 8'h06; // 1
            6'd2: seg_led = 8'h5B; // 2
            6'd3: seg_led = 8'h4F; // 3
            6'd4: seg_led = 8'h66; // 4
            6'd5: seg_led = 8'h6D; // 5
            6'd6: seg_led = 8'h7D; // 6
            6'd7: seg_led = 8'h07; // 7
            6'd8: seg_led = 8'h7F; // 8
            6'd9: seg_led = 8'h6F; // 9

            6'd10: seg_led = 8'h38; // L
            6'd11: seg_led = 8'h39; // C
            6'd12: seg_led = 8'h3E; // U

            default: seg_led = 8'h00; 
        endcase
    end
endmodule