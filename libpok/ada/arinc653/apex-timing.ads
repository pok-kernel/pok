--                               POK header
--
-- The following file is a part of the POK project. Any modification should
-- be made according to the POK licence. You CANNOT use this file or a part
-- of a file for your own project.
--
-- For more information on the POK licence, please see our LICENCE FILE
--
-- Please follow the coding guidelines described in doc/CODING_GUIDELINES
--
--                                      Copyright (c) 2007-2025 POK team

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
