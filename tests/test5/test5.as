MAIN:	sub *r3, *r4
		mov *r3, *r4
		add *r3, *r4
macr	TEST
		prn STRING
endmacr
		TEST
		.data 1, 2, 3, 4
STRING: .string "myString"
		mov *r4, *r3
		stop
