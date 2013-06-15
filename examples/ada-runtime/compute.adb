package body Compute is
   procedure Printf (String : in Interfaces.C.char_array);
   pragma Import (C, Printf, "printf");

   procedure Compute is
   begin
      Printf ("beep ");
   end Compute;
end Compute;
