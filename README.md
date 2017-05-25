# BATT Programming Language

BATT is a simple stack-based programming language, inspired by FORTH & Reverse Polish Notation. There are no variables, and everything is on a stack.

Usage:
`battc [path to program]`

# Examples

````
"Enter your name" println

"Good morning " getln +
println
````

Will output
````
Enter your name
<Input>
Good morning <Input>
````

Simple fibonacci sequence, using recursion
````
proc fibonacci
    dup 50 > @end if     ; calculate until 50
    dup                  ; duplicate prev
    @+ swap              ; add '+' and swap it with prev
    dip                  ; 1 2 + 2 -> 3 2
    swap                 
    dup println
    fibonacci
end

1 println 1 println
1 1 fibonacci
````

Will output
````
1
1
2
3
5
8
13
21
34
55
````

More examples can be found inside the `Examples/` folder.