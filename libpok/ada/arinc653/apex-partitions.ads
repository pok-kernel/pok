-- ---------------------------------------------------------------------------
-- --
-- PARTITION constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
with APEX.Processes;
package APEX.Partitions is
   Max_Number_Of_Partitions : constant := System_Limit_Number_Of_Partitions;
   type Operating_Mode_Type is (Idle, Cold_Start, Warm_Start, Normal);
   type Partition_Id_Type is private;
   Null_Partition_Id : constant Partition_Id_Type;
   type Start_Condition_Type is
      (Normal_Start,
       Partition_Restart,
       Hm_Module_Restart,
       Hm_Partition_Restart);
   type Partition_Status_Type is record
       Period          : System_Time_Type;
       Duration        : System_Time_Type;
       Identifier      : Partition_Id_Type;
       Lock_Level      : APEX.Processes.Lock_Level_Type;
       Operating_Mode : Operating_Mode_Type;
       Start_Condition : Start_Condition_Type;
   end record;
   procedure Get_Partition_Status
      (Partition_Status : out Partition_Status_Type;
       Return_Code      : out Return_Code_Type);
   procedure Set_Partition_Mode
      (Operating_Mode : in Operating_Mode_Type;
       Return_Code    : out Return_Code_Type);
private
   type Partition_ID_Type is new APEX_Integer;
   Null_Partition_Id : constant Partition_Id_Type := 0;
   pragma Convention (C, Operating_Mode_Type);
   pragma Convention (C, Start_Condition_Type);
   pragma Convention (C, Partition_Status_Type);

   -- POK BINDINGS
   pragma Import (C, Get_Partition_Status, "GET_PARTITION_STATUS");
   pragma Import (C, Set_Partition_Mode, "SET_PARTITION_MODE");
   -- END OF POK BINDINGS
end APEX.Partitions;
