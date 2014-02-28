-- ---------------------------------------------------------------------------
-- --
-- BLACKBOARD constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
with APEX.Processes;
package APEX.Blackboards is
   Max_Number_Of_Blackboards : constant := System_Limit_Number_Of_Blackboards;
   subtype Blackboard_Name_Type is Name_Type;
   type Blackboard_Id_Type is private;
   Null_Blackboard_Id : constant Blackboard_Id_Type;
   type Empty_Indicator_Type is (Empty, Occupied);
   type Blackboard_Status_Type is record
       Empty_Indicator   : Empty_Indicator_Type;
       Max_Message_Size : Message_Size_Type;
       Waiting_Processes : APEX.Processes.Waiting_Range_Type;
   end record;
   procedure Create_Blackboard
      (Blackboard_Name : in Blackboard_Name_Type;
       Max_Message_Size : in Message_Size_Type;
       Blackboard_Id    : out Blackboard_Id_Type;
       Return_Code      : out Return_Code_Type);
   procedure Display_Blackboard
      (Blackboard_Id : in Blackboard_Id_Type;
       Message_Addr : in Message_Addr_Type;
       Length        : in Message_Size_Type;
       Return_Code   : out Return_Code_Type);
   procedure Read_Blackboard
      (Blackboard_Id : in Blackboard_Id_Type;
       Time_Out      : in System_Time_Type;
       Message_Addr : in Message_Addr_Type;
   -- The message address is passed IN, although the respective message is
   -- passed OUT
       Length        : out Message_Size_Type;
       Return_Code   : out Return_Code_Type);
   procedure Clear_Blackboard
      (Blackboard_Id : in Blackboard_Id_Type;
       Return_Code   : out Return_Code_Type);
   procedure Get_Blackboard_Id
      (Blackboard_Name : in Blackboard_Name_Type;
       Blackboard_Id   : out Blackboard_Id_Type;
       Return_Code     : out Return_Code_Type);
   procedure Get_Blackboard_Status
      (Blackboard_Id     : in Blackboard_Id_Type;
       Blackboard_Status : out Blackboard_Status_Type;
       Return_Code       : out Return_Code_Type);
private
   type Blackboard_Id_Type is new APEX_Integer;
   Null_Blackboard_Id : constant Blackboard_Id_Type := 0;
   pragma Convention (C, Empty_Indicator_Type);
   pragma Convention (C, Blackboard_Status_Type);

   -- POK BINDINGS
   pragma Import (C, Create_Blackboard, "CREATE_BLACKBOARD");
   pragma Import (C, Display_Blackboard, "DISPLAY_BLACKBOARD");
   pragma Import (C, Read_Blackboard, "READ_BLACKBOARD");
   pragma Import (C, Clear_Blackboard, "CLEAR_BLACKBOARD");
   pragma Import (C, Get_Blackboard_Id, "GET_BLACKBOARD_ID");
   pragma Import (C, Get_Blackboard_Status, "GET_BLACKBOARD_STATUS");
   -- END OF POK BINDINGS
end APEX.Blackboards;
