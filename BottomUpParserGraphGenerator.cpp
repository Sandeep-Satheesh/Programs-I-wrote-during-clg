#include <bits/stdc++.h>
using namespace std;

#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math,O3")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

void __print(int x)
{
    cerr << x;
}
void __print(long x)
{
    cerr << x;
}
void __print(long long x)
{
    cerr << x;
}
void __print(unsigned x)
{
    cerr << x;
}
void __print(unsigned long x)
{
    cerr << x;
}
void __print(unsigned long long x)
{
    cerr << x;
}
void __print(float x)
{
    cerr << x;
}
void __print(double x)
{
    cerr << x;
}
void __print(long double x)
{
    cerr << x;
}
void __print(char x)
{
    cerr << '\'' << x << '\'';
}
void __print(const char *x)
{
    cerr << '\"' << x << '\"';
}
void __print(const string &x)
{
    cerr << '\"' << x << '\"';
}
void __print(bool x)
{
    cerr << (x ? "true" : "false");
}

template <typename T, typename V>
void __print(const pair<T, V> &x)
{
    cerr << '{';
    __print(x.first);
    cerr << ',';
    __print(x.second);
    cerr << '}';
}
template <typename T>
void __print(const T &x)
{
    int f = 0;
    cerr << '{';
    for (auto i : x)
        cerr << (f++ ? "," : ""), __print(i);
    cerr << "}";
}
void _print()
{
    cerr << "]\n";
}
template <typename T, typename... V>
void _print(T t, V... v)
{
    __print(t);
    if (sizeof...(v))
        cerr << ", ";
    _print(v...);
}

#ifndef ONLINE_JUDGE
#define dbg(x...)                 \
    cerr << "[" << #x << "] = ["; \
    _print(x)
#else
#define dbg(x...)
#endif
#define endl "\n"

#define FOR(i, a, n) for (ll i = a; i < n; i++)
#define all(x) (x).begin(), (x).end()
#define ins(A, x) A.insert(x)
#define f first
#define s second
#define pb(A,x) A.push_back(x)
#define dline cout << "///REACHED///\n";

typedef long long ll;

/*OBJECT DEFINITIONS TO SIMPLIFY CODING */
struct prodRule {
    char LHS;
    string RHS;

    bool operator <(const prodRule& p) const {
        return (LHS < p.LHS) || (p.LHS == LHS && RHS < p.RHS);
    }
    bool operator ==(const prodRule& p) const {
        return (LHS == p.LHS) && RHS == p.RHS;
    }
};

struct CanonicalState {
    set<prodRule> rules;
    map<char, CanonicalState> gotoStates;
    int shouldGoToExistingState;

    bool operator <(const CanonicalState& c) const {
        return (rules < c.rules);
    }

    bool operator == (const CanonicalState& c) const {
        return (rules == c.rules);
    }

    CanonicalState() {
        shouldGoToExistingState = -1;
    }
};

/* GLOBAL VARIABLES */
set<prodRule> prodRules;
vector<CanonicalState> states;
set<char> nonTerminals, terminals;
queue<CanonicalState> unexpandedStatesQueue;

/* UTILITY FUNCTIONS*/
void pause() {
    system("pause");
}
void clearScreen() {
    system("cls");
}

bool isNonTerminal(char c) {
    return nonTerminals.find(c) != nonTerminals.end();
}

void displayStates() {
    clearScreen();

    map<CanonicalState, int> m;
    FOR (i, 0, states.size()) {
        m[states[i]] = i;
    }
    int idx = 0;
    cout<<'+';
    FOR (i, 0, 53) cout<<'-'; cout<<'+'<<endl;
    cout<<"|"<<setw(8)<<"STATE"<<setw(4)<<"|"<<setw(20)<<"PRODUCTION RULES"<<setw(4)<<"| "<<setw(8)<<"GOTO"<<setw(11)<<"|\n";
    cout<<'+';
    FOR (i, 0, 53) cout<<'-';
    cout<<'+';

    for (CanonicalState st : states) {
        cout<<'\n';
        int n = st.rules.size(), i = 1;
        vector<prodRule> v (all(st.rules));
        cout<<"|"<<setw(5)<<"I"<<idx<<setw(6 - (idx/10 > 0))<<"|"<<setw(14-v[0].RHS.length())<<v[0].LHS<<" -> "<<v[0].RHS<<setw(5)<<"|";

        auto it = st.gotoStates.begin();

        if (st.gotoStates.size() > 0) {
            cout<<setw(7)<<" GOTO ('"<<it->f<<"') = I"<<m[it->s]<<setw(4 - (m[it->s]/10 > 0))<<"|\n";
            it++;
        }
        else cout<<setw(15)<<"REDUCED STATE!"<<setw(5)<<"|\n";
        bool spaceFlg = (idx/10 > 0);

        while (i < n) {
            spaceFlg = (idx/10 > 0);
            cout<<"|"<<setw(6 + spaceFlg)<<"  "<<setw(6 - (idx/10 > 0))<<"|"<<setw(14-v[i].RHS.length())<<v[i].LHS<<" -> "<<v[i].RHS<<setw(5)<<"|";
            if (it != st.gotoStates.end()) {
                cout<<setw(7)<<" GOTO ('"<<it->f<<"') = I"<<m[it->s]<<setw(4 - (m[it->s]/10 > 0))<<"|\n";
                it++;
            }
            else cout<<setw(20)<<"|\n";
            i++;
        }
        while (it != st.gotoStates.end()) {
            cout<<"|"<<setw(12 + spaceFlg)<<"|"<<setw(12)<<"|"<<setw(12)<<" GOTO ('"<<it->f<<"') = I"<<m[it->s]<<setw(4 - (m[it->s]/10 > 0))<<"|\n";
            it++;
        }
        cout<<'+';
        FOR (i, 0, 53) cout<<'-';
        cout<<'+';
        idx++;
    }
    cout<<'\n';
}

/*HANDLER FUNCTIONS FOR EACH MENU OPTION*/

void showIntro() {
    FOR (i, 0, 81) cout<<'-';
    cout<<"\n|\t\t\tCANONICAL STATE CALCULATOR FOR BOTTOM UP PARSERS\t|\n";
    FOR (i, 0, 81) cout<<'-';
    cout<<"\n\nWelcome! This is a small project developed by S Sandeep to save himself during Compiler Design CATs! "<<char(2);
    cout<<"\n\nIMPORTANT NOTE: If you are new to the program interface, enter '5' in the options menu to know the expected input format, to have a positive experience using the program."<<"\nHope you like it!\n";
    cout<<"\nChoose from the menu options below:";
}

void showMenu() {
    cout<<"\n\n\tMenu\n----------------------\n1 - Enter production rule."
        <<"\n2 - Show entered production rules."
        <<"\n3 - Delete production rule."
        <<"\n4 - Generate canonical items graph."
        <<"\n5 - Show constraints of the program (IMPORTANT: READ THIS TO KNOW THE FORMAT OF INPUT EXPECTED!)"
        <<"\n6 - Exit.";

        cout<<"\n\nMake your choice: ";
}

void displayConstraints() {
    clearScreen();
    cout<<"\n\nNOTE: The input constraints of the program are as mentioned below. You are requested to follow the correct expected input format to get the correct output. If you fail to do so, the correctness of the output is not guaranteed, and no one else except you are responsible for the repercussions of the same.";
    cout<<"\n\n1. NON-TERMINAL SYMBOL: Denoted by SINGLE CAPITAL LETTER (eg.: 'E', 'T', 'F')."
        <<"\n2. TERMINAL SYMBOL: Denoted by SINGLE LOWERCASE LETTER (eg.: 'i', 'a', 'b'). ALL OTHER NON-CAPITAL LETTER SYMBOLS LIKE '(', ')', etc. WILL BE TREATED AS TERMINALS ONLY.";

    cout<<"\n\nThese are some other assumptions that the program makes to simplify stuff: "
        <<"\n1. There is only ONE non-terminal on the LHS of any production rule."
        <<"\n2. There can be ONE or MORE symbols (terminal OR non-terminal) on the RHS of the production rule."
        <<"\n3. Each production rule is unique, and DOES NOT consist of composite RHS (ie, with a pipeline '|' symbol to denote different production rules with the same LHS)."
        <<"\n4. The grammar used is NOT ambiguous, and is not recursive or conflicting."
        <<"\n5. The LHS non-terminal in the production with the reduced start symbol (usually denoted as S' -> S), is denoted in this program as 'A'. Thus, it would be A -> .S, if your start symbol was 'S'. This makes the symbol 'A' reserved for denoting the above production, and thus you should not enter it in your production rules. You are requested to rename your production rule's variable accordingly.";

    cout<<"\n\nThanks for reading! ";
    pause();
    clearScreen();
}

void addNewProdRule() {
    clearScreen();
    prodRule p {0, ""};
    cout<<"\nAdding new production rule:\n\n";
    cout<<"\nEnter LHS of rule (one non-terminal symbol): ";
    cin>>p.LHS;
    cout<<"Enter RHS of rule (without spaces, terminals/non-terminals): ";
    cin>>p.RHS;

    if (prodRules.find(p) != prodRules.end()) { cout<<"\nRule already exists! Cannot add duplicates!"; return; }
    prodRules.insert(p);

    nonTerminals.insert(p.LHS);
    for (char c : p.RHS) {
        if (isupper(c)) nonTerminals.insert(c);
        else terminals.insert(c);
    }

    cout<<"\nProduction rule added successfully! "; pause(); clearScreen();
}

void showProdRules() {
    clearScreen();
    cout<<"\n\nHere are the the production rules you have already input:\n";
    int idx = 1;
    for (prodRule p : prodRules) {
        cout<<"\nRULE #"<<idx<<": "<<p.LHS<<" -> "<<p.RHS;
        idx++;
    }
    cout<<"\n\n";
    pause(); clearScreen();
}

void removeProdRule(prodRule& p) {
    if (prodRules.find(p) == prodRules.end()) { cout<<"Rule not found!"; return; }
    prodRules.erase(p);
    nonTerminals.erase(p.LHS);
    for (char c : p.RHS) {
        if (isNonTerminal(c))
            nonTerminals.erase(c);
        else
            terminals.erase(c);
    }
    cout<<"\nRule removed successfully!";
    pause();
}

void generateCanonicalGraph(char ss) {
    clearScreen();
    states.clear();

    cout<<"\n\n\t\tGenerating graph, please wait....";
    //initialize the start set and begin the recursion...
    set<prodRule> initRules;
    for (prodRule p : prodRules) {
        p.RHS = "." + p.RHS;
        initRules.insert(p);
    }

    //insert the reduced start symbol production rule.
    {
        prodRule p;
        p.LHS = 'A';
        p.RHS = ".";
        p.RHS += ss;
        initRules.insert(p);
    }

    CanonicalState initState;
    initState.rules = initRules;

    //logic: to perform 'kind-of' BFS on all expandable nodes until we expand all nodes.
    unexpandedStatesQueue.push(initState);

    while (!unexpandedStatesQueue.empty()) {
        CanonicalState currState = unexpandedStatesQueue.front();
        unexpandedStatesQueue.pop();

        map<char, int> done;

        for (prodRule p : currState.rules) {
            //displayStates();
            //pause();
            string rhs = p.RHS;
            int i = 0; //lookAhead pointer
            while (i < rhs.length() && rhs[i] != '.') i++;

            if (i < rhs.length() - 1) {

                char lookAhead = rhs[i+1];
                //on reading lookahead symbol, we go to the next state.
                swap(rhs[i], rhs[i+1]);
                i++;
                currState.gotoStates[lookAhead].rules.insert({p.LHS, rhs});

                if (i >= rhs.length() - 1) continue;

                char readSymbol = lookAhead;

                lookAhead = rhs[i+1];
                if (isNonTerminal(lookAhead)) {
                    //add all production rules of non-terminal into the new state.
                    queue<char> lhs;
                    lhs.push(lookAhead);

                    while (!lhs.empty()) {
                        char c = lhs.front();
                        if (done[c]) { lhs.pop(); continue; }

                        for (prodRule pr : prodRules) {
                            if (pr.LHS == c) {
                                if (isNonTerminal(pr.RHS[0]) && !done[pr.RHS[0]]) {
                                    lhs.push(pr.RHS[0]);
                                }

                                pr.RHS = "." + pr.RHS;
                                currState.gotoStates[readSymbol].rules.insert(pr);
                            }
                        }
                        done[c] = 1;
                        lhs.pop();
                    }
                }
                //else currState.gotoStates[lookAhead].rules.insert({p.LHS, rhs});
            }
        }
        auto it = find(all(states), currState);
        if (it == states.end())
            pb(states, currState);
        else {
            it->gotoStates.insert(all(currState.gotoStates));
        }
        for (auto& st : currState.gotoStates) {
            it = find(all(states), st.s);
            if (it == states.end()) {
                unexpandedStatesQueue.push(st.s);
                pb(states, st.s);
            }
            else {
                it->gotoStates.insert(all(st.s.gotoStates));
            }
        }
    }
    displayStates();
    pause(); clearScreen();
}

int main() {
    showIntro();

    int ch = 0;
    while (ch != 6) {
        showMenu();
        cin>>ch;
        switch(ch) {
            case 1: addNewProdRule(); break;

            case 2: if (prodRules.size() == 0) { cout<<"\nNothing to show!"; pause(); }
                    else showProdRules();
                    break;

            case 3: if (prodRules.size() == 0) { cout<<"\nNothing to remove!"; break; }
                    cout<<"\n\nEnter production rule to be removed...";
                    {
                        prodRule p {'0', ""};
                        cout<<"\nEnter LHS non-terminal: ";
                        cin>>p.LHS;
                        cout<<"Enter RHS: ";
                        cin>>p.RHS;
                        removeProdRule(p);
                    }
                    break;

            case 4: if (prodRules.size() == 0) { cout<<"\nNo rules found! Please add at least one rule to begin with!"; pause(); break; }
                    cout<<"\nEnter start symbol: ";
                    char ss; cin>>ss;
                    while (ss == 'A' || nonTerminals.find(ss) == nonTerminals.end()) {
                        cout<<"\nThe symbol you entered is not found in the grammar, or is a reserved symbol. Please try again!\nEnter start symbol: ";
                        cin>>ss;
                    }
                    generateCanonicalGraph(ss);
                    break;

            case 5: displayConstraints();
                    break;

            case 6: cout<<"\nExiting...\n\n"; return 0;

            default: cout<<"\nInvalid option! Please try again!";
        }
    }
    return 0;
}
