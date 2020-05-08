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
--                                      Copyright (c) 2007-2020 POK team

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

