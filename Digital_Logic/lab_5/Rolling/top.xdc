# --- 1. 时钟信号 (Clock) ---
# 将 PIN_W5 替换为你板子上 50MHz 时钟的管脚号
set_property PACKAGE_PIN P17 [get_ports clk]
set_property IOSTANDARD LVCMOS33 [get_ports clk]

# --- 2. 复位信号 (Reset) ---
# 将 PIN_U18 替换为复位按键的管脚号
set_property PACKAGE_PIN P5 [get_ports rst_n]
set_property IOSTANDARD LVCMOS33 [get_ports rst_n]

# --- 3. 数码管位选 (Selection) ---
# 对应 seg_sel[0] 到 seg_sel[3]
set_property PACKAGE_PIN G1  [get_ports {seg_sel[0]}]
set_property PACKAGE_PIN F1  [get_ports {seg_sel[1]}]
set_property PACKAGE_PIN E1  [get_ports {seg_sel[2]}]
set_property PACKAGE_PIN G6  [get_ports {seg_sel[3]}]

set_property IOSTANDARD LVCMOS33 [get_ports {seg_sel[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {seg_sel[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {seg_sel[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {seg_sel[3]}]

# --- 4. 数码管段选 (Segment LED) ---
# 对应 seg_led[0] (a段) 到 seg_led[7] (小数点)
set_property PACKAGE_PIN D4  [get_ports {seg_led[0]}]
set_property PACKAGE_PIN E3  [get_ports {seg_led[1]}]
set_property PACKAGE_PIN D3  [get_ports {seg_led[2]}]
set_property PACKAGE_PIN F4  [get_ports {seg_led[3]}]
set_property PACKAGE_PIN F3  [get_ports {seg_led[4]}]
set_property PACKAGE_PIN E2  [get_ports {seg_led[5]}]
set_property PACKAGE_PIN D2  [get_ports {seg_led[6]}]
set_property PACKAGE_PIN H2  [get_ports {seg_led[7]}]

set_property IOSTANDARD LVCMOS33 [get_ports {seg_led[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {seg_led[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {seg_led[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {seg_led[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {seg_led[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {seg_led[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {seg_led[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {seg_led[7]}]