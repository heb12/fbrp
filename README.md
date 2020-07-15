# FBREP
"Flexible Bible REference Parser"
## Features
* Pure C99
* No Regex
* Small and fast

### Can Parse:
"John 3 16"  
"John 3 16-17"  
"1 John 3 v16"  

### Cannot Parse:
By design, it cannot parse references like:  
"John 3 16, John 3 17"  
"John 3, Hebrews 4"  

### Testing:
`gcc test.c fbrp.c && ./a.out`