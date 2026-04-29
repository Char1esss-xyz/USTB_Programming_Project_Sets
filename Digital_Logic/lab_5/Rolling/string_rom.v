module string_rom (
    input  wire [3:0] addr,
    output reg  [5:0] char_code
);

    always @(*) begin
        case (addr)
            default: char_code = 6'd0;
        endcase
    end
endmodule