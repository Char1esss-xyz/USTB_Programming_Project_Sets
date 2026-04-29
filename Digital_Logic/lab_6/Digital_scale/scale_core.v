module scale_core(
    input  wire clk,
    input  wire rst_n,
    input  wire pulse_accum,
    input  wire pulse_clear,
    input  wire pulse_view,
    input  wire pulse_price_set,
    input  wire [3:0] weight,
    input  wire [3:0] price,

    output reg [3:0] d7, d6, d5, d4, d3, d2, d1, d0,
    output wire [15:0] monitor_accum_total,
    output wire [1:0] current_state,
    output wire [3:0] monitor_accum_count_ten,
    output wire [3:0] monitor_accum_count_one
);

    localparam S_CURRENT = 2'd0;
    localparam S_ACCUM = 2'd1;
    localparam S_CLEAR = 2'd2;

    reg [1:0] state;

    reg [7:0] current_total;
    reg [15:0] accum_total;
    reg [7:0] accum_count;
    
    localparam CHAR_A = 4'd10;
    localparam CHAR_C = 4'd11;
    localparam CHAR_L = 4'd12;
    localparam CHAR_R = 4'd13;
    localparam CHAR_BLANK = 4'd14;

    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            accum_total <= 0;
            accum_count <= 0;
            state <= S_CURRENT;
        end else begin
            current_total <= weight * price;

            if (pulse_clear) begin
                accum_total <= 0;
                accum_count <= 0;
                state <= S_CLEAR;
            end
            else if (pulse_price_set) begin
                state <= S_CURRENT;
            end
            else if (pulse_accum) begin
                accum_total <= accum_total + current_total;
                accum_count <= accum_count + 1;
                state <= S_ACCUM;
            end
            else if (pulse_view) begin
                if (state == S_CLEAR) state <= S_CURRENT;
                else if (state == S_CURRENT) state <= S_ACCUM;
                else state <= S_CURRENT;
            end
        end
    end

    wire [3:0] cur_w_ten = (weight >= 10) ? 1 : 0;
    wire [3:0] cur_w_one = weight % 10;
    wire [3:0] cur_p_ten = (price >= 10) ? 1 : 0;
    wire [3:0] cur_p_one = price % 10;
    
    wire [3:0] cur_t_hun = current_total / 100;
    wire [3:0] cur_t_ten = (current_total % 100) / 10;
    wire [3:0] cur_t_one = current_total % 10;

    wire [3:0] acc_c_ten = (accum_count % 100) / 10;
    wire [3:0] acc_c_one = accum_count % 10;
    
    wire [3:0] acc_t_thou = accum_total / 1000;
    wire [3:0] acc_t_hun = (accum_total % 1000) / 100;
    wire [3:0] acc_t_ten = (accum_total % 100) / 10;
    wire [3:0] acc_t_one = accum_total % 10;

    always @(*) begin
        case(state)
            S_CURRENT: begin
                d7 = cur_p_ten;
                d6 = cur_p_one;
                d5 = cur_w_ten;
                d4 = cur_w_one;
                d3 = 4'd0;
                d2 = cur_t_hun;
                d1 = cur_t_ten;
                d0 = cur_t_one;
            end
            S_ACCUM: begin
                d7 = CHAR_A;
                d6 = CHAR_C;
                d5 = acc_c_ten;
                d4 = acc_c_one;
                d3 = acc_t_thou;
                d2 = acc_t_hun;
                d1 = acc_t_ten;
                d0 = acc_t_one;
            end
            S_CLEAR: begin
                d7 = CHAR_BLANK;
                d6 = CHAR_BLANK;
                d5 = CHAR_C;
                d4 = CHAR_L;
                d3 = CHAR_R;
                d2 = CHAR_BLANK;
                d1 = CHAR_BLANK;
                d0 = CHAR_BLANK;
            end
            default: begin
                d7 = CHAR_BLANK;
                d6 = CHAR_BLANK;
                d5 = CHAR_BLANK;
                d4 = CHAR_BLANK;
                d3 = CHAR_BLANK;
                d2 = CHAR_BLANK;
                d1 = CHAR_BLANK;
                d0 = CHAR_BLANK;
            end
        endcase
    end
    assign monitor_accum_total = accum_total;
    assign current_state = state;
    assign monitor_accum_count_ten = acc_c_ten;
    assign monitor_accum_count_one = acc_c_one;
endmodule