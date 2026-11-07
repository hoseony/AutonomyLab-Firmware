file build/can_hello.elf
set architecture arm
target remote localhost:3333

define flash
  mon reset halt
  load
  mon reset run
  detach
  quit
end

define debug
  mon reset halt
  load
  break main
  continue
  tui enable
end
