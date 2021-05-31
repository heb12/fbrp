# FBRP
"Flexible Bible Reference Parser"  
FBRP is a tiny Bible reference parser written in C99. It parses  
references like:
- "2 Kings 1, 3-5:2-4, 3-6, 1-2"
Into structures like this:
```
Book: 2Kings
Chapter: [1]-[1]
Chapter: [3]-[5]
Verse: [2]-[4]
Verse: [3]-[6]
Verse: [1]-[2]
```


## Features
* No dependencies or even standard C headers
* One C file, one header file, no complex build system needed
* Tiny and flexible

## Cannot Parse:
By design, it cannot parse references like:  
- "John 3 16, John 3 17"
- "John 3, Hebrews 4"
- "John 3 16, KJV"

## Testing:
`cc test.c fbrp.c && ./a.out`

## How it works:
Like a lexer/parser, it parses the string into a tokens.  
Then, it uses that to run through each token and figure out  
exactly what they all mean.  

There is currently no algorithm specification, but feel free to  
ask if you need any clarification.
