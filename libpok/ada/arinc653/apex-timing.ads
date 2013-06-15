-- ---------------------------------------------------------------------------
-- --
-- TIME constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
package APEX.Timing is
   procedure Timed_Wait
      (Delay_Time : in System_Time_Type;
       Return_Code : out Return_Code_Type);
   procedure Periodic_Wait (Return_Code : out Return_Code_Type);
   procedure Get_Time
      (System_Time : out System_Time_Type;
       Return_Code : out Return_Code_Type);
   procedure Replenish
      (Budget_Time : in System_Time_Type;
       Return_Code : out Return_Code_Type);

   -- POK BINDINGS
   pragma Import (C, Timed_Wait, "TIMED_WAIT");
   pragma Import (C, Periodic_Wait, "PERIODIC_WAIT");
   pragma Import (C, Get_Time, "GET_TIME");
   pragma Import (C, Replenish, "REPLENISH");
   -- END OF POK BINDINGS
end Apex.Timing;
