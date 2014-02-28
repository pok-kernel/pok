-- ---------------------------------------------------------------------------
-- --
-- EVENT constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
with APEX.Processes;
package APEX.Events is
   Max_Number_Of_Events : constant := System_Limit_Number_Of_Events;
   subtype Event_Name_Type is Name_Type;
   type Event_Id_Type is private;
   Null_Event_Id : constant Event_Id_Type;
   type Event_State_Type is (Down, Up);
   type Event_Status_Type is record
       Event_State       : Event_State_Type;
       Waiting_Processes : APEX.Processes.Waiting_Range_Type;
   end record;
   procedure Create_Event
      (Event_Name : in Event_Name_Type;
       Event_Id    : out Event_Id_Type;
       Return_Code : out Return_Code_Type);
   procedure Set_Event
      (Event_Id    : in Event_Id_Type;
       Return_Code : out Return_Code_Type);
   procedure Reset_Event
      (Event_Id    : in Event_Id_Type;
       Return_Code : out Return_Code_Type);
   procedure Wait_Event
      (Event_Id    : in Event_Id_Type;
       Time_Out    : in System_Time_Type;
       Return_Code : out Return_Code_Type);
   procedure Get_Event_Id
      (Event_Name : in Event_Name_Type;
       Event_Id    : out Event_Id_Type;
       Return_Code : out Return_Code_Type);
   procedure Get_Event_Status
      (Event_Id     : in Event_Id_Type;
       Event_Status : out Event_Status_Type;
       Return_Code : out Return_Code_Type);
private
   type Event_Id_Type is new APEX_Integer;
   Null_Event_Id : constant Event_Id_Type := 0;
   pragma Convention (C, Event_State_Type);
   pragma Convention (C, Event_Status_Type);

   -- POK BINDINGS
   pragma Import (C, Create_Event, "CREATE_EVENT");
   pragma Import (C, Set_Event, "SET_EVENT");
   pragma Import (C, Reset_Event, "RESET_EVENT");
   pragma Import (C, Wait_Event, "WAIT_EVENT");
   pragma Import (C, Get_Event_Id, "GET_EVENT_ID");
   pragma Import (C, Get_Event_Status, "GET_EVENT_STATUS");
    -- END OF POK BINDINGS
end APEX.Events;
