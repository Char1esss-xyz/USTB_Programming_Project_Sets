module uart_pll(
    input  wire clk_in,
    output wire clk_out
);
    assign clk_out = clk_in; 
endmodule

module uartAdapter(
    input wire clk,
    input wire resetn,
    input wire rx,
    output wire tx,
    input wire [7:0] wdata,
    input wire wvalid,
    output wire [7:0] rdata,
    output wire rvalid
);

    wire sys_clk;
    wire rst;

    assign rst = ~resetn;

    uart_pll u_pll(
        .clk_in(clk),
        .clk_out(sys_clk)
    );

    uartReceiver u_rx(
        .clk(sys_clk),
        .rst(rst),
        .rx(rx),
        .rdata(rdata),
        .rvalid(rvalid)
    );

    uartTransmitter u_tx(
        .clk(sys_clk),
        .rst(rst),
        .tx(tx),
        .wdata(wdata),
        .wvalid(wvalid)
    );

endmodule