package body Activity is
   procedure Printf (String : in Interfaces.C.char_array);
   pragma Import (C, Printf, "printf");

   procedure Thr1_Job is
      Ret : Return_Code_Type;
   begin
      loop
         Printf ("beep ");
         Timed_Wait (1000, Ret);
      end loop;
   end Thr1_Job;
end Activity;

