`timescale 1ns / 1ps

module tb_rolling_name();

    reg clk;
    reg rst_n;
    reg shift_en;
    reg scan_en;

    wire [3:0] start_idx;
    wire [1:0] scan_pos;
    wire [3:0] seg_sel;
    wire [3:0] rom_addr;
    wire [5:0] char_code;

    shift_counter #(.MAX_IDX(13)) u_shift (
        .clk(clk),
        .rst_n(rst_n),
        .shift_en(shift_en),
        .start_idx(start_idx)
    );

    scan_counter u_scan (
        .clk(clk),
        .rst_n(rst_n),
        .scan_en(scan_en),
        .scan_pos(scan_pos),
        .seg_sel(seg_sel)
    );

    assign rom_addr = start_idx + scan_pos;

    string_rom u_rom (
        .addr(rom_addr),
        .char_code(char_code)
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        scan_en = 0;
        forever begin
            repeat(1) @(posedge clk);
            scan_en = 1;
            @(posedge clk);
            scan_en = 0;
        end
    end

    initial begin
        shift_en = 0;
        forever begin
            repeat(19) @(posedge clk);
            shift_en = 1;
            @(posedge clk);
            shift_en = 0;
        end
    end

    initial begin
        rst_n = 0;
        #20;
        rst_n = 1;

        #2000;
        $stop;
    end

endmodule