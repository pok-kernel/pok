-- ---------------------------------------------------------------------------
-- --
-- SEMAPHORE constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
with APEX.Processes;
package APEX.Semaphores is
   Max_Number_Of_Semaphores : constant := System_Limit_Number_Of_Semaphores;
   Max_Semaphore_Value : constant := 32_767;
   subtype Semaphore_Name_Type is Name_Type;
   type Semaphore_Id_Type is private;
   Null_Semaphore_Id : constant Semaphore_Id_Type;
   type Semaphore_Value_Type is new APEX_Integer range
       0 .. Max_Semaphore_Value;
   type Semaphore_Status_Type is record
       Current_Value      : Semaphore_Value_Type;
       Maximum_Value      : Semaphore_Value_Type;
       Waiting_Processes : APEX.Processes.Waiting_Range_Type;
   end record;
   procedure Create_Semaphore
      (Semaphore_Name      : in Semaphore_Name_Type;
       Current_Value       : in Semaphore_Value_Type;
       Maximum_Value       : in Semaphore_Value_Type;
       Queuing_Discipline : in Queuing_Discipline_Type;
       Semaphore_Id        : out Semaphore_Id_Type;
       Return_Code         : out Return_Code_Type);
   procedure Wait_Semaphore
      (Semaphore_Id : in Semaphore_Id_Type;
       Time_Out      : in System_Time_Type;
       Return_Code : out Return_Code_Type);
   procedure Signal_Semaphore
      (Semaphore_Id : in Semaphore_Id_Type;
       Return_Code : out Return_Code_Type);
   procedure Get_Semaphore_Id
      (Semaphore_Name : in Semaphore_Name_Type;
       Semaphore_Id    : out Semaphore_Id_Type;
       Return_Code     : out Return_Code_Type);
   procedure Get_Semaphore_Status
      (Semaphore_Id      : in Semaphore_Id_Type;
       Semaphore_Status : out Semaphore_Status_Type;
       Return_Code       : out Return_Code_Type);
private
   type Semaphore_Id_Type is new APEX_Integer;
   Null_Semaphore_Id : constant Semaphore_Id_Type := 0;
   pragma Convention (C, Semaphore_Status_Type);

   -- POK BINDINGS
   pragma Import (C, Create_Semaphore, "CREATE_SEMAPHORE");
   pragma Import (C, Wait_Semaphore, "WAIT_SEMAPHORE");
   pragma Import (C, Signal_Semaphore, "SIGNAL_SEMAPHORE");
   pragma Import (C, Get_Semaphore_Id, "GET_SEMAPHORE_ID");
   pragma Import (C, Get_Semaphore_Status, "GET_SEMAPHORE_STATUS");
   -- END OF POK BINDINGS
end APEX.Semaphores;
