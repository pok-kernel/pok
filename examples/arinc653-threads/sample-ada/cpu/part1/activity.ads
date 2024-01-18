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
--                                      Copyright (c) 2007-2024 POK team

pragma No_Run_Time;

with APEX;
use APEX;
with APEX.Timing;
use APEX.Timing;
with Interfaces.C;

package Activity is
   procedure Thr1_Job;
end Activity;
