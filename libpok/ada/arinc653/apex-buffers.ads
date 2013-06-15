-- ---------------------------------------------------------------------------
-- --
-- BUFFER constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
with APEX.Processes;
package APEX.Buffers is
   Max_Number_Of_Buffers : constant := System_Limit_Number_Of_Buffers;
   subtype Buffer_Name_Type is Name_Type;
   type Buffer_Id_Type is private;
   Null_Buffer_Id : constant Buffer_Id_Type;
   type Buffer_Status_Type is record
       Nb_Message         : Message_Range_Type;
       Max_Nb_Message     : Message_Range_Type;
       Max_Message_Size : Message_Size_Type;
       Waiting_Processes : APEX.Processes.Waiting_Range_Type;
   end record;
   procedure Create_Buffer
      (Buffer_Name         : in Buffer_Name_Type;
       Max_Message_Size    : in Message_Size_Type;
       Max_Nb_Message      : in Message_Range_Type;
       Queuing_Discipline :  in Queuing_Discipline_Type;
       Buffer_Id           : out Buffer_Id_Type;
       Return_Code         : out Return_Code_Type);
   procedure Send_Buffer
      (Buffer_Id    : in Buffer_Id_Type;
       Message_Addr : in Message_Addr_Type;
       Length       : in Message_Size_Type;
       Time_Out     : in System_Time_Type;
       Return_Code : out Return_Code_Type);
   procedure Receive_Buffer
      (Buffer_Id    : in Buffer_Id_Type;
       Time_Out     : in System_Time_Type;
       Message_Addr : in Message_Addr_Type;
   -- The message address is passed IN, although the respective message is
   -- passed OUT
       Length       : out Message_Size_Type;
       Return_Code : out Return_Code_Type);
   procedure Get_Buffer_Id
      (Buffer_Name : in Buffer_Name_Type;
       Buffer_Id   : out Buffer_Id_Type;
       Return_Code : out Return_Code_Type);
   procedure Get_Buffer_Status
      (Buffer_Id      : in Buffer_Id_Type;
       Buffer_Status : out Buffer_Status_Type;
       Return_Code    : out Return_Code_Type);
private
   type Buffer_Id_Type is new APEX_Integer;
   Null_Buffer_Id : constant Buffer_Id_Type := 0;
   pragma Convention (C, Buffer_Status_Type);

   -- POK BINDINGS
   pragma Import (C, Create_Buffer, "CREATE_BUFFER");
   pragma Import (C, Send_Buffer, "SEND_BUFFER");
   pragma Import (C, Receive_Buffer, "RECEIVE_BUFFER");
   pragma Import (C, Get_Buffer_Id, "GET_BUFFER_ID");
   pragma Import (C, Get_Buffer_Status, "GET_BUFFER_STATUS");
   -- END OF POK BINDINGS
end APEX.Buffers;
