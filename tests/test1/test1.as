macr ADD
mov RESULT, A
add RESULT, B
endmacr
macr TEST
stop
endmacr
.entry RESULT

	
     			
.extern A
.extern B
;this is a comment line that should be ignored
RESULT: .data 0, -1, +1, -2, 2
ADD
prn RESULT
stop
