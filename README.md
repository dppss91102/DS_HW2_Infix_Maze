# [Infix Maze](https://acm.cs.nthu.edu.tw/problem/11859/)
## Description
Determine whether a matrix contains at least a legal infix expression which need to be from the top-left corner to the bottom-right corner. If yes, convert it based on the postfix notation.   

There are some rules to be followed. 

- Each cell can be reached by four directions(up, down, right, left).
- Matrices are consisted of 1~9, +,-,*,/, (, )
- Each cell can be visited at most one time.
- The priority directions are: down>right>left>up.
- No negative number. EX: - 5 + 3 is illegal.       

![img1.png](image/img1.png)

## Input
- First line is total number of matrices.
- Second and third line are matrix width nad height.
- Width and height <100.
- Following lines are matrix.

## Output
- Repeat all inputs.
- For matrix, there is a white space after every elements.
- If there is a legal infix expression from the top-left corner to the bottom-right corner
    - output Yes
    - infix expression ( there is a white space after every elements.)
    - postfix expression ( there is a white space after every elements.)

- If No, output No

### [Sample Input](sampleIn.txt)
```
1
3
3
1 5 4 
+ - * 
+ 6 8 
```

### [Sample Output](sampleOut.txt)
```1
   3
   3
   1 5 4 
   + - * 
   + 6 8 
   Yes
   15 - 68 
   15 68 - 
```
