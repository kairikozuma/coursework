#Copyright 1991-2013 Mentor Graphics Corporation
#
#All Rights Reserved.
#
#THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
#MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.

if {![file exists work]} {
     vlib work
}
vcom -novopt ../vhdl/traffic.vhd ../vhdl/queue.vhd ../vhdl/tb_traffic.vhd
source intersection.tcl 
draw_intersection
set_light_state green .traffic.i.ns_light
set_light_state green .traffic.i.ew_light
vmap work work
vsim  -novopt tb_traffic
source lights.tcl 
connect_lights
source queues.tcl 
draw_queues
source controls.tcl 
draw_controls

