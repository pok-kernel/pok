-- ---------------------------------------------------------------------------
-- --
-- QUEUING PORT constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
with APEX.Processes;
package APEX.Queuing_Ports is
   Max_Number_Of_Queuing_Ports : constant :=
      System_Limit_Number_Of_Queuing_Ports;
   subtype Queuing_Port_Name_Type is Name_Type;
   type Queuing_Port_Id_Type is private;
   Null_Queuing_Port_Id : constant Queuing_Port_Id_Type;
   type Queuing_Port_Status_Type is record
       Nb_Message         : Message_Range_Type;
       Max_Nb_Message     : Message_Range_Type;
       Max_Message_Size : Message_Size_Type;
       Port_Direction     : Port_Direction_Type;
       Waiting_Processes : APEX.Processes.Waiting_Range_Type;
   end record;
   procedure Create_Queuing_Port
      (Queuing_Port_Name : in Queuing_Port_Name_Type;
       Max_Message_Size    : in Message_Size_Type;
       Max_Nb_Message      : in Message_Range_Type;
       Port_Direction      : in Port_Direction_Type;
       Queuing_Discipline : in Queuing_Discipline_Type;
       Queuing_Port_Id     : out Queuing_Port_Id_Type;
       Return_Code         : out Return_Code_Type);
   procedure Send_Queuing_Message
      (Queuing_Port_Id : in Queuing_Port_Id_Type;
       Message_Addr     : in Message_Addr_Type;
       Length           : in Message_Size_Type;
       Time_Out         : in System_Time_Type;
       Return_Code      : out Return_Code_Type);
   procedure Receive_Queuing_Message
      (Queuing_Port_Id : in Queuing_Port_Id_Type;
       Time_Out         : in System_Time_Type;
       Message_Addr     : in Message_Addr_Type;
   -- The message address is passed IN, although the respective message is
   -- passed OUT
       Length           : out Message_Size_Type;
       Return_Code      : out Return_Code_Type);
   procedure Get_Queuing_Port_Id
      (Queuing_Port_Name : in Queuing_Port_Name_Type;
       Queuing_Port_Id    : out Queuing_Port_Id_Type;
       Return_Code        : out Return_Code_Type);
   procedure Get_Queuing_Port_Status
      (Queuing_Port_Id      : in Queuing_Port_Id_Type;
       Queuing_Port_Status : out Queuing_Port_Status_Type;
       Return_Code          : out Return_Code_Type);
private
   type Queuing_Port_Id_Type is new APEX_Integer;
   Null_Queuing_Port_Id : constant Queuing_Port_Id_Type := 0;
   pragma Convention (C, Queuing_Port_Status_Type);

   -- POK BINDINGS
   pragma Import (C, Create_Queuing_Port, "CREATE_QUEUING_PORT");
   pragma Import (C, Send_Queuing_Message, "SEND_QUEUING_PORT_MESSAGE");
   pragma Import (C, Receive_Queuing_Message, "RECEIVE_QUEUING_MESSAGE");
   pragma Import (C, Get_Queuing_Port_Id, "GET_QUEUING_PORT_ID");
   pragma Import (C, Get_Queuing_Port_Status, "GET_QUEUING_PORT_STATUS");
   -- END OF POK BINDINGS
end APEX.Queuing_Ports;
