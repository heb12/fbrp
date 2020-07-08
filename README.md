## FBRP
"Flexible Bible Reference Parser"  
Written in C from the ground up. No regex used.  

### Can Parse:
"John 3 16"  
"John 3 16-17"  
"1 John 3 v16"  

### Cannot Parse:
"John 3 16, John 3 17"  
"John 3, Hebrews 4"  

### Testing:
`gcc test.c fbrp.c && ./a.out`