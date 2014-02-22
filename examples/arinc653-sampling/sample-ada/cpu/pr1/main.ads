pragma No_Run_Time;

with Interfaces.C;
with APEX;
use APEX;
with APEX.Processes;
use APEX.Processes;
with APEX.Partitions;
use APEX.Partitions;
with APEX.Timing;
use APEX.Timing;
with APEX.Sampling_Ports;
use APEX.Sampling_Ports;

package Main is
   procedure Send;
   procedure Main;
   pragma Export (C, Main, "main");
end Main;
