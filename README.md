# [Infix Maze]()
## Description
Determine whether a matrix contains at least a legal infix expression which need to be from the top-left corner to the bottom-right corner. If yes, convert it based on the postfix notation.   

There are some rules to be followed. 

- Each cell can be reached by four directions(up, down, right, left).
- Matrices are consisted of 1~9, +,-,*,/, (, )
- Each cell can be visited at most one time.
- The priority directions are: down>right>left>up.
- No negative number. EX: - 5 + 3 is illegal.
