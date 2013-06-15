-- ---------------------------------------------------------------------------
-- --
-- MODULE_SCHEDULES constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
package APEX.Module_Schedules is
   type Schedule_Id_Type is private;
   Null_Schedule_Id : constant Schedule_Id_Type;
   subtype Schedule_Name_Type is Name_Type;
   type Schedule_Status_Type     is record
       Time_Of_Last_Schedule_Switch        : System_Time_Type;
       Current_Schedule                    : Schedule_Id_Type;
       Next_Schedule                : Schedule_Id_Type;
   end record;
   procedure Set_Module_Schedule
      (Schedule_Id      : in Schedule_Id_Type;
       Return_Code      : out Return_Code_Type);
   procedure Get_Module_Schedule_Status
      (Schedule_Status      : out Schedule_Status_Type;
       Return_Code          : out Return_Code_Type);
   procedure Get_Module_Schedule_Id
      (Schedule_Name     : in Schedule_Name_Type;
       Schedule_Id       : out Schedule_Id_Type;
       Return_Code       : out Return_Code_Type);
private
   Type Schedule_Id_Type is new APEX_Integer;
   Null_Schedule_Id : constant Schedule_Id_Type := 0;
   pragma Convention (C, Schedule_Status_Type);
end APEX.Module_Schedules;
