# FBRP
"Flexible Bible Reference Parser"
## Features
* C99
* No Regex or any other libraries
* Tiny and fast
* One C file

### Can Parse:
"John 3 16"  
"2 John 3 16-17"  
"2 Kings 1, 3-5:2-4, 3-6, 1-2"  

### Cannot Parse:
By design, it cannot parse references like:  
"John 3 16, John 3 17"  
"John 3, Hebrews 4"  

### Testing:
`cc test.c fbrp.c && ./a.out`