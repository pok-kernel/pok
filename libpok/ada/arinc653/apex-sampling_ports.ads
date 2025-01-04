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
-- SAMPLING PORT constant and type definitions and management services --
-- --
-- ---------------------------------------------------------------------------
package APEX.Sampling_Ports is
   Max_Number_Of_Sampling_Ports : constant :=
      System_Limit_Number_Of_Sampling_Ports;
   subtype Sampling_Port_Name_Type is Name_Type;
   type Sampling_Port_Id_Type is private;
   Null_Sampling_Port_Id : constant Sampling_Port_Id_Type;
   type Validity_Type is (Invalid, Valid);
   type Sampling_Port_Status_Type is record
       Refresh_Period    : System_Time_Type;
       Max_Message_Size : Message_Size_Type;
       Port_Direction    : Port_Direction_Type;
       Last_Msg_Validity : Validity_Type;
   end record;
   procedure Create_Sampling_Port
      (Sampling_Port_Name : in Sampling_Port_Name_Type;
       Max_Message_Size   : in Message_Size_Type;
       Port_Direction     : in Port_Direction_Type;
       Refresh_Period     : in System_Time_Type;
       Sampling_Port_Id   : out Sampling_Port_Id_Type;
       Return_Code        : out Return_Code_Type);
   procedure Write_Sampling_Message
      (Sampling_Port_Id : in Sampling_Port_Id_Type;
       Message_Addr     : in Message_Addr_Type;
       Length           : in Message_Size_Type;
       Return_Code      : out Return_Code_Type);
   procedure Read_Sampling_Message
      (Sampling_Port_Id : in Sampling_Port_Id_Type;
       Message_Addr     : in Message_Addr_Type;
   -- The message address is passed IN, although the respective message is
   -- passed OUT
       Length           : out Message_Size_Type;
       Validity         : out Validity_Type;
       Return_Code      : out Return_Code_Type);
   procedure Get_Sampling_Port_Id
      (Sampling_Port_Name : in Sampling_Port_Name_Type;
       Sampling_Port_Id   : out Sampling_Port_Id_Type;
       Return_Code        : out Return_Code_Type);
   procedure Get_Sampling_Port_Status
      (Sampling_Port_Id     : in Sampling_Port_Id_Type;
       Sampling_Port_Status : out Sampling_Port_Status_Type;
       Return_Code          : out Return_Code_Type);
private
   type Sampling_Port_Id_Type is new APEX_Integer;
   Null_Sampling_Port_Id : constant Sampling_Port_Id_Type := 0;
   pragma Convention (C, Validity_Type);
   pragma Convention (C, Sampling_Port_Status_Type);

   -- POK BINDINGS
   pragma Import (C, Create_Sampling_Port, "CREATE_SAMPLING_PORT");
   pragma Import (C, Write_Sampling_Message, "WRITE_SAMPLING_MESSAGE");
   pragma Import (C, Read_Sampling_Message, "READ_SAMPLING_MESSAGE");
   pragma Import (C, Get_Sampling_Port_Id, "GET_SAMPLING_PORT_ID");
   pragma Import (C, Get_Sampling_Port_Status, "GET_SAMPLING_PORT_STATUS");
   -- END OF POK BINDINGS
end APEX.Sampling_Ports;
