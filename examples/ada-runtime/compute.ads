pragma No_Run_Time;
with Interfaces.C;

package Compute is
   procedure Compute;
   pragma Export (C, Compute, "compute");
end Compute;
