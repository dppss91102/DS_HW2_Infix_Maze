#include <iostream>
using namespace std;

char** makeMatrix(int r, int c);
bool solveMaze(int r, int c, char **maze, int lastD, int prior);
bool WhichWay (char type, int r, int c, char** maze, int lastD, int prior);
bool IsFineToGo(char type, int r, int c, char **maze);
bool isOper(char c);
bool isNum(char c);

string InfixToPostfix(string in);
int getPriority (char op);

#define MAX 200
static int maxRow, maxColumn;

class Path {
public:
    int top;
    int pare;
    int a[MAX][2];
    Path() {
        a[0][0] = 0;
        a[0][1] = 0;
        top = 0;
        pare = 0;
    };
    void init();
    bool push(int row, int column);
    int row_top();
    int column_top();
    bool pop();
    bool contains(int row, int column);
};

bool Path::push(int row, int column) {
    if (Path::top == MAX) {
        //Stack overflow.
        return false;
    } else{
        a[++top][0] = row;
        a[top][1] = column;
        return true;
    }
}
int Path::row_top() {
    if (top == -1){
        //Stack is empty.
        return -1;
    } else{
        return a[top][0];
    }
}
int Path::column_top() {
    if (top == -1){
        //Stack is empty.
        return -1;
    } else{
        return a[top][1];
    }
}
bool Path::pop() {
    if (top == -1){
        //Stack is empty.
        return false;
    } else{
        a[top][0] = 0;
        a[top--][1] = 0;
        return true;
    }
}
bool Path::contains(int row, int column) {
    for (int i = 0; i <= top; ++i) {
        if (a[i][0] == row && a[i][1] == column)
            return true;
    }

    return false;
}
void Path::init() {
    for (int i = 0; i < MAX; i ++){
        a[i][0] = 0;
        a[i][1] = 0;
    }
    top = 0;
}

Path path;

class Stack{
public:
    int top;
    char a[MAX];
    Stack() {
        for (int i = 0; i < MAX ; i++)
            a[i] = '\0';
        top = -1;
    };
    bool push(char value);
    char pop();
    char at(int i);
    char at_top();
};
bool Stack::push(char value){
    if (Stack::top == MAX){
        //Stack overflow.
        return false;
    } else{
        a[++top] = value;
        return true;
    }
}
char Stack::pop(){
    if (Stack::top == -1){
        //Stack is Empty
        return 'e';
    } else{
        char temp = a[top];
        a[top--] = '\0';
        return temp;
    }
}
char Stack::at(int i){
    if (i >= 0 && i <= top)
        return a[i];
    else
        return 'e';
}
char Stack::at_top(){
    if (top != -1)
        return a[top];
    else
        return 'e';
}

int main() {
    int n;
    char **matrix;

    cin >> n;
    cout << n << endl;

    while (n>0){
        cin >> maxColumn;
        cin >> maxRow;
        matrix = makeMatrix(maxRow, maxColumn);

        cout << maxRow << endl;
        cout << maxColumn << endl;
        for (int i = 0; i < maxRow; ++i){
            for (int j = 0; j < maxColumn; ++j) {
                cout << matrix[i][j] << ' ';
            }
            cout << endl;
        }

        if (solveMaze(0, 0, matrix, 0, 4)) {
            cout << "Yes" << endl;

            std::string infix_str;
            int num = path.top;
            for (int i = 0; i <= num; i++) {
                infix_str.insert(0, 1, matrix[path.row_top()][path.column_top()]);
                path.pop();
            }

            for (int i = 0; i < infix_str.length(); i++) {
                switch (infix_str.at(i)) {
                    case '0' ... '9':
                        if (i == infix_str.length() - 1) {
                            infix_str += ' ';
                            break;
                        } else {
                            if (infix_str.at(i + 1) > '9' || infix_str.at(i + 1) < '0')
                                infix_str.insert(i + 1, 1, ' ');
                            break;
                        }
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '(':
                    case ')':
                        infix_str.insert(i + 1, 1, ' ');
                        break;
                }
            }

            cout << infix_str << endl;

            string postfix = InfixToPostfix(infix_str);
            cout << postfix << endl;
        } else
            cout << "No" << endl;

        path.init();

        for (int i = 0; i < maxRow; i++){
            free(matrix[i]);
        }
        free(matrix);
        maxRow = maxColumn = 0;
        n--;
    }
    return 0;
}

char** makeMatrix(int r, int c){

    auto ** temp = (char **)malloc(sizeof(char *) * r);
    for (int k = 0; k < r; ++k)
        temp[k] = (char *)malloc(sizeof(char) * c);

    for (int i = 0; i < r; ++i){
        for (int j = 0; j < c; ++j) {
            cin >> temp[i][j];
        }
    }

    return temp;
}

bool solveMaze(int r, int c, char **maze, int lastD, int prior){

    cout << r << ' ' << c << ' ' << maze[r][c] << ' ' << path.pare << endl;

    if (r == maxRow - 1 && c == maxColumn - 1){
        if (isOper(maze[r][c]) || maze[r][c] == '(')
            return false;
        if (path.pare != 0) {
            path.pop();
            r = path.row_top();
            c = path.column_top();
            return solveMaze(r, c, maze, 0, lastD - 1);
        }

        return true;
    } else {
        switch (maze[r][c]) {
            case '0' ... '9':
                return WhichWay ('n', r, c, maze, lastD, prior);
            case '+':
            case '-':
            case '*':
            case '/':
                return WhichWay ('c', r, c, maze, lastD, prior);
            case '(':
                if (r == 0 && c == 0)
                    path.pare++;
                return WhichWay ('l', r, c, maze, lastD, prior);
            case ')':
                return WhichWay ('r', r, c, maze, lastD, prior);

            default:
                cout << "Unacceptable symbols detected." << endl;
                break;
        }
    }
}

bool WhichWay (char type, int r, int c, char** maze, int lastD, int prior){
    if ( IsFineToGo(type, r + 1, c, maze) && prior >= 4) {
        path.push(r + 1, c);
        return solveMaze(r + 1, c, maze, 4, 4);

    } else if ( IsFineToGo(type, r, c + 1, maze) && prior >= 3) {
        path.push(r, c + 1);
        return solveMaze(r, c + 1, maze, 3, 4);

    } else if ( IsFineToGo(type, r, c - 1, maze) && prior >= 2) {
        path.push(r, c - 1);
        return solveMaze(r, c - 1, maze, 2, 4);

    } else if ( IsFineToGo(type, r - 1, c, maze) && prior >= 1) {
        path.push(r - 1, c);
        return solveMaze(r - 1, c, maze, 1, 4);

    } else {
        if (maze[r][c] == '(')
            path.pare--;
        else if (maze[r][c] == ')')
            path.pare++;
        path.pop();
        r = path.row_top();
        c = path.column_top();
        if (r == -1 || c == -1)
            return false;
        return solveMaze(r, c, maze, 0, lastD - 1);
    }
}

bool  IsFineToGo(char type, int r, int c, char **maze) {

    if (r < 0 || r >= maxRow || c < 0 || c >= maxColumn || path.contains(r, c)){
        return false;
    }

    if (maze[r][c] == '(')
        path.pare++;
    else if (maze[r][c] == ')')
        path.pare--;

    if (path.pare < 0) {
        path.pare++;
        return false;
    }
    char next = maze[r][c];

    //'c' for computation, 'r' for right parenthesis, 'l' for left parenthesis
    switch (type) {
        case 'c':
        case 'l':
            return !isOper(next) && next != ')';
        case 'r':
            return !isNum(next) && next != '(';
        case 'n':
            return next != '(';
        default:
            return false;
    }
}

string InfixToPostfix(string in){
    string out;
    Stack operands;
    int p = 0;

    for (char i : in) {
        switch (i){
            case '0' ... '9':
                out += i;
                break;
            case '(':
                operands.push(i);
                break;
            case ')':
                p = operands.top;
                while (operands.at(p) != '('){
                    out += ' ';
                    out += operands.at(p);
                    operands.pop();
                    p--;
                }
                operands.pop();
                break;
            case '+':
            case '-':
            case '*':
            case '/':
                out += ' ';
                if (getPriority (i) < getPriority(operands.at_top()) && operands.top != -1) {
                    while (operands.top != -1 && operands.at_top() != '(') {
                        out += operands.at_top();
                        out += ' ';
                        operands.pop();
                    }
                    operands.push(i);
                } else if (getPriority (i) == getPriority(operands.at_top())){
                    out += i;
                    out += ' ';
                } else{
                    operands.push(i);
                }
                break;
            default:
                break;
        }

    }

    while (operands.top != -1){
        out += ' ';
        out += operands.at_top();
        operands.pop();
    }

    out += ' ';
    return out;

}

int getPriority (char op){
    int opp;
    switch (op){
        case '+':
        case '-':
            opp = 1;
            break;
        case '*':
        case '/':
            opp = 2;
            break;
        case '(':
        case ')':
            opp = 3;
            break;
        default:
            opp = 0;
            break;
    }
    return opp;
}

bool isOper(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}
bool isNum(char c) {
    return (c >= '0' && c <= '9');
}