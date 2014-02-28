-- This is a compilable Ada 95 specification for the APEX interface,
-- derived from section 3 of ARINC 653.
-- The declarations of the services given below are taken from the
-- standard, as are the enumerated types and the names of the others types.
-- However, the definitions given for these others types, and the
-- names and values given below for constants, are all implementation
-- specific.
-- All types have defining representation pragmas or clauses to ensure
-- representation compatibility with the C and Ada 83 bindings.
-- ---------------------------------------------------------------------------
-- --
-- Root package providing constant and type definitions                     --
-- --
-- ---------------------------------------------------------------------------
with System;
                    -- This is the Ada 95 predefined C interface package
with Interfaces.C;
package APEX is
   pragma Pure;
   -- ----------------------------
   -- Domain limits              --
   -- ----------------------------
   --  Domain dependent
   --  These values define the domain limits and are implementation-dependent.
   System_Limit_Number_Of_Partitions     : constant := 32;
   -- module scope
   System_Limit_Number_Of_Messages       : constant := 512;
   -- module scope
   System_Limit_Message_Size             : constant := 16#10_0000#;
   -- module scope
   System_Limit_Number_Of_Processes      : constant := 1024;
   -- partition scope
   System_Limit_Number_Of_Sampling_Ports : constant := 1024;
   -- partition scope
   System_Limit_Number_Of_Queuing_Ports  : constant := 1024;
   -- partition scope
   System_Limit_Number_Of_Buffers        : constant := 512;
   -- partition scope
   System_Limit_Number_Of_Blackboards    : constant := 512;
   -- partition scope
   System_Limit_Number_Of_Semaphores     : constant := 512;
   -- partition scope
   System_Limit_Number_Of_Events         : constant := 512;
   -- partition scope
   -- ----------------------------
   -- Base APEX types            --
   -- ----------------------------
   --  The actual sizes of these base types are system-specific and must
   --  match those of the underlying Operating System.
   type APEX_Byte is new Interfaces.C.unsigned_char;
   type APEX_Integer is new Interfaces.C.long;
   type APEX_Unsigned is new Interfaces.C.unsigned_long;
   type APEX_Long_Integer is new Interfaces.Integer_64;
   -- If Integer_64 is not provided in package Interfaces, any implementation-
   -- defined alternative 64-bit signed integer type may be used.
   -- ----------------------------
   -- General APEX types        --
   -- ----------------------------
   type Return_Code_Type is (
      No_Error,        -- request valid and operation performed
      No_Action,       -- status of system unaffected by request
      Not_Available,   -- resource required by request unavailable
      Invalid_Param,   -- invalid parameter specified in request
      Invalid_Config, -- parameter incompatible with configuration
      Invalid_Mode,    -- request incompatible with current mode
      Timed_Out);      -- time-out tied up with request has expired
   pragma Convention (C, Return_Code_Type);
   Max_Name_Length : constant := 30;
   subtype Name_Type is String (1 .. Max_Name_Length);
   subtype System_Address_Type is System.Address;
   subtype Message_Addr_Type is System.Address;
   subtype Message_Size_Type is APEX_Integer range
      1 .. System_Limit_Message_Size;
   subtype Message_Range_Type is APEX_Integer range
      0 .. System_Limit_Number_Of_Messages;
   type Port_Direction_Type is (Source, Destination);
   pragma Convention (C, Port_Direction_Type);
   type Queuing_Discipline_Type is (Fifo, Priority);
   pragma Convention (C, Queuing_Discipline_Type);
   subtype System_Time_Type is APEX_Long_Integer;
   -- 64-bit signed integer with 1 nanosecond LSB
   Infinite_Time_Value : constant System_Time_Type;
   Aperiodic           : constant System_Time_Type;
   Zero_Time_Value     : constant System_Time_Type;
private
   Infinite_Time_Value : constant System_Time_Type := -1;
   Aperiodic           : constant System_Time_Type := 0;
   Zero_Time_Value     : constant System_Time_Type := 0;
end APEX;
