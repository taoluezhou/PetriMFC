load_package flow

#project_open xxx

#project_new e:/petrinet/test/top -revision top -overwrite

set_global_assignment -name FAMILY Cyclone

set_global_assignment -name DEVICE EP1C12Q240C8

set_global_assignment -name BDF_FILE top.bdf

set_global_assignment -name TOP_LEVEL_ENTITY top

set_location_assignment Pin_50 -to p1 

set_location_assignment Pin_11 -to p2

set_location_assignment Pin_228 -to p3

set_location_assignment Pin_193 -to p4

set_location_assignment Pin_170 -to t1 

set_location_assignment Pin_131 -to t2

set_location_assignment Pin_108 -to t3

set_location_assignment Pin_73 -to t4

set_location_assignment Pin_28 -to clk

set_location_assignment Pin_14 -to rst

execute_flow -compile

#project_close






#analysis & synthsis
#quartus_map xxx

#download
#quartus_pgm -c USB-Blaster -m jtag -o p;top.sof
#quartus_pgm -c "USB-Blaster [USB-0]" -m JTAG -o p;top.sof



