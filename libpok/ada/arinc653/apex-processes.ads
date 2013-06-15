-- ---------------------------------------------------------------------------
-- --
-- PROCESS constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
package APEX.Processes is
   Max_Number_Of_Processes : constant := System_Limit_Number_Of_Processes;
   Min_Priority_Value : constant := 0;
   Max_Priority_Value : constant := 249;
   Max_Lock_Level : constant := 32;
   subtype Process_Name_Type is Name_Type;
   type Process_Id_Type is private;
   Null_Process_Id : constant Process_Id_Type;
   subtype Lock_Level_Type is APEX_Integer range 0 .. Max_Lock_Level;
   subtype Stack_Size_Type is APEX_Unsigned;
   subtype Waiting_Range_Type is APEX_Integer range
      0 .. Max_Number_Of_Processes;
   subtype Priority_Type is APEX_Integer range
      Min_Priority_Value .. Max_Priority_Value;
   type Process_State_Type is (Dormant, Ready, Running, Waiting);
   type Deadline_Type is (Soft, Hard);
   type Process_Attribute_Type is record
       Period        : System_Time_Type;
       Time_Capacity : System_Time_Type;
       Entry_Point   : System_Address_Type;
       Stack_Size    : Stack_Size_Type;
       Base_Priority : Priority_Type;
       Deadline      : Deadline_Type;
       Name          : Process_Name_Type;
   end record;
   type Process_Status_Type is record
       Deadline_Time    : System_Time_Type;
       Current_Priority : Priority_Type;
       Process_State    : Process_State_Type;
       Attributes       : Process_Attribute_Type;
   end record;
   procedure Create_Process
      (Attributes : in Process_Attribute_Type;
       Process_Id : out Process_Id_Type;
       Return_Code : out Return_Code_Type);
   procedure Set_Priority
      (Process_Id : in Process_Id_Type;
       Priority    : in Priority_Type;
       Return_Code : out Return_Code_Type);
   procedure Suspend_Self
      (Time_Out    : in System_Time_Type;
       Return_Code : out Return_Code_Type);
   procedure Suspend
      (Process_Id : in Process_Id_Type;
       Return_Code : out Return_Code_Type);
   procedure Resume
      (Process_Id : in Process_Id_Type;
       Return_Code : out Return_Code_Type);
   procedure Stop_Self;
   procedure Stop
      (Process_Id : in Process_Id_Type;
      Return_Code : out Return_Code_Type);
   procedure Start
     (Process_Id : in Process_Id_Type;
      Return_Code : out Return_Code_Type);
   procedure Delayed_Start
     (Process_Id : in Process_Id_Type;
      Delay_Time : in System_Time_Type;
      Return_Code : out Return_Code_Type);
   procedure Lock_Preemption
     (Lock_Level : out Lock_Level_Type;
      Return_Code : out Return_Code_Type);
   procedure Unlock_Preemption
     (Lock_Level : out Lock_Level_Type;
      Return_Code : out Return_Code_Type);
   procedure Get_My_Id
     (Process_Id : out Process_Id_Type;
      Return_Code : out Return_Code_Type);
   procedure Get_Process_Id
     (Process_Name : in Process_Name_Type;
      Process_Id    : out Process_Id_Type;
      Return_Code : out Return_Code_Type);
   procedure Get_Process_Status
     (Process_Id      : in Process_Id_Type;
      Process_Status : out Process_Status_Type;
      Return_Code     : out Return_Code_Type);
private
   type Process_ID_Type is new APEX_Integer;
   Null_Process_Id : constant Process_Id_Type := 0;
   pragma Convention (C, Process_State_Type);
   pragma Convention (C, Deadline_Type);
   pragma Convention (C, Process_Attribute_Type);
   pragma Convention (C, Process_Status_Type);

   -- POK BINDINGS
   pragma Import (C, Create_Process, "CREATE_PROCESS");
   pragma Import (C, Set_Priority, "SET_PRIORITY");
   pragma Import (C, Suspend_Self, "SUSPEND_SELF");
   pragma Import (C, Suspend, "SUSPEND");
   pragma Import (C, Resume, "SUSPEND");
   pragma Import (C, Stop_Self, "STOP_SELF");
   pragma Import (C, Stop, "STOP");
   pragma Import (C, Start, "START");
   pragma Import (C, Delayed_Start, "DELAYED_START");
   pragma Import (C, Lock_Preemption, "LOCK_PREEMPTION");
   pragma Import (C, Unlock_Preemption, "UNLOCK_PREEMPTION");
   pragma Import (C, Get_My_Id, "GET_MY_ID");
   pragma Import (C, Get_Process_Id, "GET_PROCESS_ID");
   pragma Import (C, Get_Process_Status, "GET_PROCESS_STATUS");
   -- END OF POK BINDINGS
end APEX.Processes;
