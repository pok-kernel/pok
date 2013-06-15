pragma No_Run_Time;

with Interfaces.C;
with APEX;
use APEX;
with APEX.Processes;
use APEX.Processes;
with APEX.Partitions;
use APEX.Partitions;
with Activity;

package Main is
   procedure Compute;
   procedure Main;
   pragma Export (C, Main, "main");
end Main;
