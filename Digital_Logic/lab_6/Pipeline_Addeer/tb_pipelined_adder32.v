`timescale 1ns / 1ps

module tb_pipelined_adder32();

    // Signal definitions
    reg clk;
    reg rst_n;
    reg [31:0] A, B;
    reg cin;
    wire [31:0] SUM;
    wire cout;

    // 4-stage pipeline input delay queue
    reg [31:0] A_history [0:3];
    reg [31:0] B_history [0:3];
    reg        cin_history [0:3];

    // Statistic variables
    integer error_count = 0;
    integer test_count = 0;
    integer i;

    // Instantiate the adder under test
    pipelined_adder32 dut (
        .clk(clk),
        .rst_n(rst_n),
        .A(A),
        .B(B),
        .cin(cin),
        .SUM(SUM),
        .cout(cout)
    );
    always #5 clk = ~clk;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (i=0; i<4; i=i+1) begin
                A_history[i] <= 0;
                B_history[i] <= 0;
                cin_history[i] <= 0;
            end
        end else begin
            A_history[0] <= A;
            B_history[0] <= B;
            cin_history[0] <= cin;
            
            A_history[1] <= A_history[0];
            B_history[1] <= B_history[0];
            cin_history[1] <= cin_history[0];

            A_history[2] <= A_history[1];
            B_history[2] <= B_history[1];
            cin_history[2] <= cin_history[1];

            A_history[3] <= A_history[2];
            B_history[3] <= B_history[2];
            cin_history[3] <= cin_history[2];
        end
    end

    always @(posedge clk) begin
        if (rst_n) begin
            #2; 
            
            if (test_count > 4) begin
                {expected_cout, expected_sum} = A_history[3] + B_history[3] + cin_history[3];

                if (SUM !== expected_sum || cout !== expected_cout) begin
                    error_count = error_count + 1;
                end
            end
        end
    end

    reg [31:0] expected_sum;
    reg expected_cout;

    initial begin
        // Initialization
        clk = 0; rst_n = 0;
        A = 0; B = 0; cin = 0;

        // Reset
        #15 rst_n = 1;
        #5;

        // Send random data
        for (i = 0; i < 20; i = i + 1) begin
            A = $urandom();
            B = $urandom();
            cin = $urandom_range(0, 1);
            
            test_count = test_count + 1;
            #10;
        end

        // Wait for pipeline to empty
        #50;
        $finish;
    end

endmodule