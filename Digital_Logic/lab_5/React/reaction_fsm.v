module reaction_timer_fsm (
    input wire clk,
    input wire rst_n,
    input wire start_pulse,
    input wire react_pulse,
    input wire tick_1ms,
    input wire [15:0] random_val,
    output reg led,
    output reg [13:0] count_val
);

    reg [1:0] state, next_state;
    reg [13:0] timer_counter;
    reg [13:0] delay_counter;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) state <= 2'b00;
        else state <= next_state;
    end

    always @(*) begin
        case (state)
            // idle state
            2'b00: begin
                if (start_pulse) next_state = 2'b01;
                else next_state = 2'b00;
            end
            // start delay
            2'b01: begin
                if (delay_counter == 0) next_state = 2'b10;
                else next_state = 2'b01;
            end
            // start timer
            2'b10: begin
                if (react_pulse) next_state = 2'b11;
                else next_state = 2'b10;
            end
            // measurement complete
            2'b11: begin
                if (start_pulse) next_state = 2'b01;
                else next_state = 2'b11;
            end
            default: next_state = 2'b00;
        endcase
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            led <= 0;
            timer_counter <= 0;
            delay_counter <= 0;
            count_val <= 14'd0;
        end else begin
            case (state)
                2'b00: begin  // idle state
                    led <= 0;
                    timer_counter <= 0;
                    if (start_pulse) begin
                        // set random delay time
                        delay_counter <= {3'b0, random_val[10:0]} + 2000;
                    end
                end

                2'b01: begin  // start delay
                    led <= 0;
                    if (tick_1ms && delay_counter > 0)
                        delay_counter <= delay_counter - 1;
                end

                2'b10: begin  // start timer
                    led <= 1;
                    if (tick_1ms && timer_counter < 9999)
                        timer_counter <= timer_counter + 1;  // �?大计�?9999ms
                end

                2'b11: begin  // measurement complete
                    led <= 0;
                end
            endcase

            count_val <= timer_counter;
        end
    end

endmodule