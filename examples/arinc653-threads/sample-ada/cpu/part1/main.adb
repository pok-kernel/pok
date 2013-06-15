package body Main is
   procedure Printf (String : in Interfaces.C.char_array);
   pragma Import (C, Printf, "printf");

   procedure Compute is
   begin
      loop
         Printf ("beep ");
      end loop;
   end Compute;

   procedure Main is
      Ret : Return_Code_Type;
      Attr : Process_Attribute_Type;
      Id : Process_Id_Type;
   begin
      Attr.Period := 1000;
      Attr.Deadline :=  Soft;
      Attr.Time_Capacity := 1;
      Attr.Stack_Size := 4096;
      Attr.Entry_Point := Compute'Address;
      Create_Process (Attr, Id, Ret);
      Set_Partition_Mode (Normal, Ret);
   end Main;
end Main;
