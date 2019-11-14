# Test file for Lab3_bitManipulation


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

# Example test:
test "PINA: 0x00, PINB: 0x00 => PORTC: 0"
# Set inputs
##setPINA 0x00
##setPINB 0x00
# Continue for several ticks
##continue 2
# Set expect values
##expectPORTC 0
# Check pass/fail
##checkResult

# Add tests below

test "PINA[7:0]: 0x00, PORTB[7:0]: 0x00, PORTC[7:0]: 0x00"
setPINA 0x00
setPINB 0x00
continue 2
expectPORTC 0x00
checkResult

test "PINA[7:0]: 0x10, PORTB[7:0]: 0x00, PORTC[7:0]: 0x01"
setPINA 0x10
setPINB 0x00
continue 2
expectPORTC 0x01
checkResult

test "PINA[7:0]: 0x01, PORTB[7:0]: 0x01, PORTC[7:0]: 0x02"
setPINA 0x01
setPINB 0x01
continue 2
expectPORTC 0x02
checkResult

test "PINA[7:0]: 0x04, PORTB[7:0]: 0x40, PORTC[7:0]: 0x03"
setPINA 0x03
setPINB 0x40
continue 2
expectPORTC 0x03
checkResult

test "PINA[7:0]: 0x03, PORTB[7:0]: 0x03, PORTC[7:0]: 0x04"
setPINA 0x03
setPINB 0x03
continue 2
expectPORTC 0x04
checkResult

test "PINA[7:0]: 0x80, PORTB[7:0]: 0xF0, PORTC[7:0]: 0x05"
setPINA 0x80
setPINB 0xF0
continue 2
expectPORTC 0x05
checkResult

test "PINA[7:0]: 0x70, PORTB[7:0]: 0x07, PORTC[7:0]: 0x06"
setPINA 0x70
setPINB 0x07
continue 2
expectPORTC 0x06
checkResult

test "PINA[7:0]: 0xC0, PORTB[7:0]: 0xB3, PORTC[7:0]: 0x07"
setPINA 0xC0
setPINB 0xB3
continue 2
expectPORTC 0x07
checkResult

test "PINA[7:0]: 0xF0, PORTB[7:0]: 0x0F, PORTC[7:0]: 0x08"
setPINA 0xF0
setPINB 0x0F
continue 2
expectPORTC 0x08
checkResult

test "PINA[7:0]: 0xCC, PORTB[7:0]: 0xF1, PORTC[7:0]: 0x09"
setPINA 0xCC
setPINB 0xF1
continue 2
expectPORTC 0x09
checkResult

test "PINA[7:0]: 0xF1, PORTB[7:0]: 0x1F, PORTC[7:0]: 0x0A"
setPINA 0xF1
setPINB 0x1F
continue 2
expectPORTC 0x0A
checkResult

test "PINA[7:0]: 0xFF, PORTB[7:0]: 0xFF, PORTC[7:0]: 0x10"
setPINA 0xFF
setPINB 0xFF
continue 2
expectPORTC 0x10
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
