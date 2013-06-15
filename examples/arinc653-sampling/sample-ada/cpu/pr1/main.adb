package body Main is
   procedure Printf (String : in Interfaces.C.char_array);
   pragma Import (C, Printf, "printf");

   Pr1_Pdataout_Id : Sampling_Port_Id_Type;

   procedure Send is
      T : Standard.Integer := 0;
      Data : Standard.Integer;
      Ret : Return_Code_Type;
   begin
      T := 3;
      loop
         Data := T;
         Printf ("Send...");
         T := T + 1;
         Write_Sampling_Message (Pr1_Pdataout_Id, Data'Address, 4, Ret);
         Periodic_Wait (Ret);
      end loop;
   end Send;

   procedure Main is
      Ret : Return_Code_Type;
      Attr : Process_Attribute_Type;
      Id : Process_Id_Type;
      Port_Name : Sampling_Port_Name_Type := "pr1_pdataout                  ";
   begin
      Port_Name (13) := ASCII.NUL;
      Create_Sampling_Port (Port_Name, 4, Source, 15,
                            Pr1_Pdataout_Id, Ret);
      Attr.Period := 1000;
      Attr.Deadline :=  Soft;
      Attr.Time_Capacity := 1;
      Attr.Stack_Size := 4096;
      Attr.Entry_Point := Send'Address;
      Create_Process (Attr, Id, Ret);
      Set_Partition_Mode (Normal, Ret);
   end Main;
end Main;
