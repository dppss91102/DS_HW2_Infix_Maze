#include <iostream>
using namespace std;

char** makeMatrix(int w, int h);
bool solveMaze(char** matrix, int r, int c);
bool isOperator(char c);
bool isNumber(char c);
int width, height;

string infix_to_postfix(string in);
int getPriority (char op);

#define MAX 200
class Path{
private:
    int a[MAX][2];
    int top;
public:
    int pare;//balance of parentheses
    Path(){
        top = -1;
        for (int i = 0; i < MAX; ++i) {
            a[i][0] = -1;
            a[i][1] = -1;
        }
    }
    void init();
    bool push(int r, int c);
    bool pop();
    int top_r();
    int top_c();
    bool contains(int r, int c);
    bool isEmpty();
};
void Path::init(){
    top = -1;
    for (int i = 0; i < MAX; ++i) {
        a[i][0] = -1;
        a[i][1] = -1;
    }
}
bool Path::push(int r, int c) {
    if (top == MAX - 1){
        //Stack Overflow
        return false;
    } else{
        a[++top][0] = r;
        a[top][1]   = c;
        return true;
    }
}
bool Path::pop() {
    if(top == -1){
        //Path empty
        return false;
    } else{
        a[top][0]   = -1;
        a[top--][1] = -1;
        return true;
    }
}
int Path::top_r() {
    return a[top][0];
}
int Path::top_c() {
    return a[top][1];
}
bool Path::contains(int r, int c) {
    if (top < 0)
        return false;//Path empty
    for (int i = 0; i <= top; ++i) {
        if (r == a[i][0] && c == a[i][1])
            return true;
    }
    return false;
}
bool Path::isEmpty() {
    return top == -1;
}
Path path;

class Stack{
private:
    int top;
    char a[MAX];
public:
    Stack(){
        top = -1;
        for (int i = 0; i < MAX; ++i) {
            a[i] = '\0';
        }
    }
    bool push(char c);
    bool pop();
    char top_o();
    bool isEmpty();
};
bool Stack::push(char c){
    if (top == MAX - 1){
        //Stack Overflow
        return false;
    } else{
        a[++top] = c;
        return true;
    }
}
bool Stack::pop(){
    if(top == -1){
        //Path empty
        return false;
    } else{
        a[top--] = '\0';
        return true;
    }
}
char Stack::top_o() {
    return a[top];
}
bool Stack::isEmpty() {
    return top == -1;
}

int main(){
    int n;
    char** matrix;
    string infix_str, postfix_str;

    cin >> n;
    cout << n << endl;

    while (n-- > 0){
        cin >> width;
        cin >> height;
        cout << width << endl;
        cout << height << endl;

        matrix = makeMatrix(width, height);

        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                cout << matrix[i][j] << ' ';
            }
            cout << endl;
        }

        if (solveMaze(matrix, 0, 0)) {
            cout << "Yes" << endl;

            do {
                infix_str = matrix[path.top_r()][path.top_c()] + infix_str;
            } while (path.pop() && !path.isEmpty());

            for (int i = 0; i < infix_str.length(); i++) {
                if (isNumber(infix_str.at(i))) {
                    if (i == infix_str.length() - 1) {
                        infix_str += ' ';
                    } else {
                        if (!isNumber(infix_str.at(i + 1))) {
                            infix_str.insert(i + 1, 1, ' ');
                            i++;
                        }
                    }
                } else if (infix_str.at(i) == ' '){
                    continue;
                } else {
                    infix_str.insert(i + 1, 1, ' ');
                    i++;
                }
            }

            cout << infix_str << endl;

            postfix_str = infix_to_postfix(infix_str);

            cout << postfix_str << endl;

        } else
            cout << "No" << endl;

        //init for next input
        infix_str = postfix_str = "";
        for (int i = 0; i < height; i++){
            free(matrix[i]);
        }
        free(matrix);
        path.init();
    }
}

char** makeMatrix(int w, int h){
    auto temp = (char**)malloc(sizeof(char*) * h);
    for (int i = 0; i < h; i++){
        temp[i] = (char*)malloc(sizeof(char) * w);
    }

    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            cin >> temp[i][j];
        }
    }

    return temp;
}
bool solveMaze(char** matrix, int r, int c){

    if (c < 0 || r < 0 || c >= width || r >= height)
        return false;

    //cout << r << ' ' << c << ' ' << matrix[r][c] << endl;

    if (c != 0 || r != 0) {
        char last = matrix[path.top_r()][path.top_c()];
        switch (last) {
            case '0' ... '9':
                if (matrix[r][c] == '(')
                    return false;
                break;
            case '+':
            case '-':
            case '*':
            case '/':
                if (matrix[r][c] == ')' || isOperator(matrix[r][c]))
                    return false;
                break;
            case '(':
                if (matrix[r][c] == ')' || isOperator(matrix[r][c]))
                    return false;
                break;
            case ')':
                if (matrix[r][c] == '(' || isNumber(matrix[r][c]))
                    return false;
                break;
        }
    } else{
        if (matrix[r][c] == ')' || isOperator(matrix[r][c]))
            return false;
    }

    if (path.contains(r, c))
        return false;

    if (c == width - 1 && r == height - 1){
        if (isOperator(matrix[r][c]) || matrix[r][c] == '(')
            return false;
        if (matrix[r][c] == ')')
            path.pare--;
        if (path.pare != 0) {
            if (matrix[r][c] == ')')
                path.pare++;
            return false;
        } else{
            path.push(r, c);
            return true;
        }

    } else{

        if (matrix[r][c] == '(')
            path.pare++;
        else if (matrix[r][c] == ')')
            path.pare--;

        if (path.pare < 0) {
            path.pare++;
            return false;
        }

        if (!path.push(r, c))
            cout << "Path::push error." << endl;

        if (solveMaze(matrix, r + 1, c))
            return true;
        if (solveMaze(matrix, r, c + 1))
            return true;
        if (solveMaze(matrix, r, c - 1))
            return true;
        if (solveMaze(matrix, r - 1, c))
            return true;

        if (matrix[r][c] == '(')
            path.pare--;
        else if (matrix[r][c] == ')')
            path.pare++;

        if (!path.pop())
            cout << "Path::pop error." << endl;

        return false;
    }
}
bool isOperator(char c){
    return c == '+' || c == '-' || c == '*' || c == '/';
}
bool isNumber(char c){
    return c > '0' && c < '9';
}

string infix_to_postfix(string in){
    string out;
    Stack operators;

    for (char i : in) {
        switch (i){
            case '0' ... '9':
                out += i;
                break;
            case '(': operators.push(i);
                break;
            case ')':
                while (operators.top_o() != '('){
                    out += ' ';
                    out += operators.top_o();
                    operators.pop();
                }
                operators.pop();
                break;
            case '+':
            case '-':
            case '*':
            case '/':
                out += ' ';
                if (getPriority(i) < getPriority(operators.top_o()) || operators.isEmpty()) {
                    while (!operators.isEmpty() && operators.top_o() != '(') {
                        out += operators.top_o();
                        out += ' ';
                 operators.pop();
                    }
             operators.push(i);
                } else if (getPriority(i) == getPriority(operators.top_o())){
                    out += i;
                    out += ' ';
                } else{
             operators.push(i);
                }
                break;
            default:
                break;
        }

    }

    while (!operators.isEmpty()){
        out += ' ';
        out += operators.top_o(); operators.pop();
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