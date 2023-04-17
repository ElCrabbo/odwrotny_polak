#include <iostream>
#include <stack>
#include <future>
#include <string>

using namespace std;

int czy_operator (char znak) {
    if (znak=='+' || znak=='-')
        return 1;
    else if (znak=='*' || znak=='/')
        return 2;
    else if (znak==')')
        return 3;
    else if (znak=='(')
        return 4;
    else return 0;
}

int dzialanie (int x, int y, char znak) {
    switch (znak) {
        case '+': return x+y; break;
        case '-': return x-y; break;
        case '*': return x*y; break;
        case '/': return x/y; break;
    }
    return 0;
}

int na_liczbe (string ciag, int &i) {
    int liczba=0;
    while ( i<ciag.size() && isdigit(ciag[i]) ) {
        liczba*=10;
        liczba+=ciag[i] - '0';
        ++i;
    };
    --i;
    return liczba;
}

int liczenie (string notacja) {
    int x, y;
    stack<int> liczby;
    for (int i=0; i<notacja.size(); ++i) {
        if (isdigit(notacja[i])) {
            liczby.push(na_liczbe(notacja, i));
        }
        else if (czy_operator(notacja[i])) {
            y=liczby.top();
            liczby.pop();
            x=liczby.top();
            liczby.pop();
            liczby.push(dzialanie(x, y, notacja[i]));
        }
    }
    return liczby.top();
}
int main()
{
    string normalne, notacja;
    stack<char> operatory;
    getline(cin,normalne);
    for (int i=0; i<normalne.size(); ++i) {
        if(isdigit(normalne[i]))
            notacja+=to_string(na_liczbe(normalne, i));
        else if (czy_operator(normalne[i])) {
            if (operatory.size()>0) {
                switch (czy_operator(normalne[i])) {
                    case 1:
                        if (operatory.top()!='(') {
                            notacja+=operatory.top();
                            operatory.pop();
                        }
                        operatory.push(normalne[i]);
                        break;
                    case 2:
                        if (operatory.top()!='(' && operatory.top()!='+' && operatory.top()!='-') {
                            notacja+=operatory.top();
                            operatory.pop();
                        }
                        operatory.push(normalne[i]);
                        break;
                    case 3:
                        while (operatory.top()!='(') {
                            notacja+=operatory.top();
                            notacja+=" ";
                            operatory.pop();
                        }
                        operatory.pop();
                        break;
                    case 4:
                        operatory.push(normalne[i]);
                        break;
                }
            }
            else
                operatory.push(normalne[i]);
        }
        notacja+=" ";
    }
    while (!operatory.empty()) {
        notacja+=operatory.top();
        operatory.pop();
        notacja+=" ";
    }
    future<int> wyn = async(liczenie,notacja);
    int wynik = wyn.get();
    cout << endl << notacja << endl;
    cout << endl << wynik;
    return 0;
}
