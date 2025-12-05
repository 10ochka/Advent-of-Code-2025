
There will be some notes about the problems and my solutions.


# Day 1
None yet

# Day 2
### Part 1
- There's no need to check each number even in subranges.
You can easily generate sequential IDs, given one of them.

### Part 2
- There's also a way to skip some numbers. You need to iterate over divisors of number length, and for each divisor generate sequence for invalid IDs. For examples, if number length is 6:

| Divisor |   1    |   2    |   3    |
|---------|:------:|:------:|:------:|
| Number  | AAAAAA | ABABAB | ABCABC |

where A, B, C are digits, of cource.

# Day 3
### Part 1
Nothing special. The first and most naive solution actually used nested `for` loop :)
### Part 2
Well, recursive solution wasn't successful... So after some time i created the current solution. In `part1.c` file XD.
And since the second part is just a generalization of the 1st, it wasn't a big deal to fix my fault.

P.S. Maybe it is better to factor out `vnls` module out of this repo.

# Day 4
Nothing special. Just convolution with kernel

![](assets/Day4-Kernel.svg)

and immediate deletion in part 4. The hardest part is to remember to `free` the matrix. :)

# Day 5
### Part 1
Wow, first application o dynamic arrays (not including use of string buffer)!
### Part 2
It was a pretty hard task to correctly merge ranges.
