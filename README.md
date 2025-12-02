
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
