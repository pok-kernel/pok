-- ---------------------------------------------------------------------------
-- --
-- ERROR constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
with APEX.Processes;
package APEX.Health_Monitoring is
   Max_Error_Message_Size : constant := 64;
   subtype Error_Message_Size_Type is APEX_Integer range
      1 .. Max_Error_Message_Size;
   type Error_Message_Type is
      array (Error_Message_Size_Type) of APEX_Byte;
   type Error_Code_Type is (
       Deadline_Missed,
       Application_Error,
       Numeric_Error,
       Illegal_Request,
       Stack_Overflow,
       Memory_Violation,
       Hardware_Fault,
       Power_Fail);
   type Error_Status_Type   is record
       Error_Code         : Error_Code_Type;
       Length             : Error_Message_Size_Type;
       Failed_Process_Id :  APEX.Processes.Process_Id_Type;
       Failed_Address     : System_Address_Type;
       Message            : Error_Message_Type;
   end record;
   procedure Report_Application_Message
      (Message_Addr : in Message_Addr_Type;
       Length       : in Message_Size_Type;
       Return_Code : out Return_Code_Type);
   procedure Create_Error_Handler
      (Entry_Point : in System_Address_Type;
       Stack_Size : in APEX.Processes.Stack_Size_Type;
       Return_Code : out Return_Code_Type);
   procedure Get_Error_Status
      (Error_Status : out Error_Status_Type;
       Return_Code : out Return_Code_Type);
   procedure Raise_Application_Error
      (Error_Code   : in Error_Code_Type;
       Message_Addr : in Message_Addr_Type;
       Length       : in Error_Message_Size_Type;
       Return_Code : out Return_Code_Type);
private
   pragma Convention (C, Error_Code_Type);
   pragma Convention (C, Error_Status_Type);
end APEX.Health_Monitoring;
