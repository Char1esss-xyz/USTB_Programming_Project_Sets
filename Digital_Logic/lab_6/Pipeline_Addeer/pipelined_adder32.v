module pipelined_adder32 (
    input wire clk,
    input wire rst_n,
    input wire [31:0] A,
    input wire [31:0] B,
    input wire cin,
    output wire [31:0] SUM,
    output wire cout
);

    reg [31:8] A_stg1;
    reg [31:8] B_stg1;
    reg [8:0] res0;

    reg [31:16] A_stg2;
    reg [31:16] B_stg2;
    reg [8:0] res1;
    reg [7:0] sum0_d1;

    reg [31:24] A_stg3;
    reg [31:24] B_stg3;
    reg [8:0] res2;
    reg [7:0] sum1_d1;
    reg [7:0] sum0_d2;

    reg [8:0] res3;
    reg [7:0] sum2_d1;
    reg [7:0] sum1_d2;
    reg [7:0] sum0_d3;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            res0 <= 9'b0;
            A_stg1 <= 24'b0;
            B_stg1 <= 24'b0;
        end else begin
            res0 <= A[7:0] + B[7:0] + cin;
            A_stg1 <= A[31:8];
            B_stg1 <= B[31:8];
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            res1 <= 9'b0;
            sum0_d1 <= 8'b0;
            A_stg2 <= 16'b0;
            B_stg2 <= 16'b0;
        end else begin
            res1 <= A_stg1[15:8] + B_stg1[15:8] + res0[8];
            sum0_d1 <= res0[7:0];
            A_stg2 <= A_stg1[31:16];
            B_stg2 <= B_stg1[31:16];
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            res2 <= 9'b0;
            sum1_d1 <= 8'b0;
            sum0_d2 <= 8'b0;
            A_stg3 <= 8'b0;
            B_stg3 <= 8'b0;
        end else begin
            res2 <= A_stg2[23:16] + B_stg2[23:16] + res1[8];
            sum1_d1 <= res1[7:0];
            sum0_d2 <= sum0_d1;
            A_stg3 <= A_stg2[31:24];
            B_stg3 <= B_stg2[31:24];
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            res3 <= 9'b0;
            sum2_d1 <= 8'b0;
            sum1_d2 <= 8'b0;
            sum0_d3 <= 8'b0;
        end else begin
            res3 <= A_stg3[31:24] + B_stg3[31:24] + res2[8];
            sum2_d1 <= res2[7:0];
            sum1_d2 <= sum1_d1;
            sum0_d3 <= sum0_d2;
        end
    end

    assign SUM = {res3[7:0], sum2_d1, sum1_d2, sum0_d3};
    assign cout = res3[8];

endmodule