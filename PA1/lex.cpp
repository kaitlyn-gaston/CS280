#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include "lex.h"

using namespace std;

LexItem getNextToken(istream& in, int& linenumber){
    
    enum TokState {BEGIN, IID, ISTRING, IREAL, IINT, INCOMMENT};
    TokState lex_state = BEGIN;
    LexItem i;
    string lex;
    char char_ch;
    bool hasDot = true;

    while(in.get(char_ch)){
        

        switch(lex_state){
                
  
            case(BEGIN):
                
                if(char_ch == '\n'){
                    ++linenumber;
                }

                else if(isspace(char_ch)) {
                    continue;
                } 

                else if(isalpha(char_ch) || char_ch =='_'){
                    
                    lex+= char_ch;
                    lex_state = IID;
                }
               
                else if(char_ch == '@' || char_ch =='$'){
                    lex+=char_ch;
                    lex_state = IID;
                }
                else if(isdigit(char_ch)){
                    lex+=char_ch;
                    lex_state = IINT;
                    
                }
                else if(char_ch == ' ' || char_ch == '\t'){
                     continue;   
                }

                else if(char_ch == '\''){
                    lex_state = ISTRING;
                }

                else if(char_ch == '{') {
                    lex_state = INCOMMENT;
                    
                }
                
                else {
                    
                    switch(char_ch);
                    
                    if(char_ch == '+'){
                        return LexItem(PLUS, lex, linenumber);
                    }
                    
                    else if(char_ch == '^'){
                        return LexItem(VAR, lex, linenumber);
                    }
                    else if(char_ch == '/'){ 
                        return LexItem(DIV, lex, linenumber);
                    }
                    else if(char_ch == '=' && in.peek() != '='){
                        return LexItem(EQ, lex, linenumber);
                    }
                    else if(char_ch == '=' && in.peek() == '='){
                        return LexItem(EQ, lex, linenumber);
                    }
                    else if(char_ch == '*'){
                       if(in.peek()!= '*')  
                       {
                           return LexItem(MULT, lex, linenumber);
                            
                       }
                        
                    }
                    else if(char_ch == '>'){
                        return LexItem(GTHAN, lex, linenumber);
                    }
                    else if(char_ch == ':' && in.peek() == '='){
                        in.get();
                        return LexItem(ASSOP, lex, linenumber);
                    }
                    else if(char_ch == '<'){
                        return LexItem(LTHAN, lex, linenumber);
                    }
                    else if(char_ch == '.'){
                        return LexItem(DOT, lex, linenumber);
                    }
                    
                    else if (char_ch == '-') {
                        lex += char_ch;
                        char c1 = in.peek();
                        if (tolower(c1) == 'e' || tolower(c1) == 'l' || tolower(c1) == 'g') {
                            lex += c1;
                            in.get();
                        }else if(char_ch =='-'){
                            return LexItem(MINUS, lex, linenumber); 
                        }
                        else {
                            return LexItem(ERR, lex, linenumber);
                        }
                    
                    }else if(char_ch == ';'){
                        return LexItem(SEMICOL, lex, linenumber);
                    }
                    else if(char_ch == ':'){
                        return LexItem(COLON, lex, linenumber);
                    }
                    else if(char_ch== ','){
                        lex+=char_ch;
                        return LexItem(COMMA, lex, linenumber);
                    }
                    else if(char_ch == '('){
                        return LexItem(LPAREN, lex, linenumber);
                    }
                    else if(char_ch == ')'){
                        return LexItem(RPAREN, lex, linenumber);
                    }
                    else{
                        lex+=char_ch;
                        return LexItem(ERR, lex, linenumber);
                    }
                }
                    
                break;
            
                
            case(ISTRING):
                
                if(char_ch == '\''){
                    return LexItem(SCONST, lex, linenumber);
                }
                else if(char_ch == '\n'){
                    lex = "'" + lex;
                    return LexItem(ERR, lex, linenumber);
                }
                else {
                    lex += char_ch;
                }

                break;

             case(IID):
                
                if(char_ch =='_' || isdigit(char_ch) || isalpha(char_ch) || char_ch =='$'){
                    
                    lex+=char_ch;    
                } 
                else {
                    in.putback(char_ch);
                    string a ="";
                    for(char c : lex){
                         a+= c;
                    }
                    return id_or_kw(a,linenumber);                   
                }
                
                break;
                    
            case(IINT):

                if(isdigit(char_ch)){
                    lex += char_ch;
                }
               
                else if((char_ch == '.')){
                    lex += char_ch;
                    lex_state = IREAL;
                }
                

                else{
                    in.putback(char_ch);
                    return LexItem(ICONST, lex, linenumber);
                    
                } 
                break;
                
            case(INCOMMENT):
                if(char_ch == '\n'){
                    ++linenumber;
                }
                else if (char_ch == '}') {
                    lex_state=BEGIN;
                }
                else if (in.eof())  {
                    break;
                }
                break;
            case(IREAL):
                if (char_ch == '\n') {
                    lex_state=BEGIN;
                }
                if(isdigit(char_ch)){
                    lex += char_ch;
                }
                
                else if(char_ch == '.' && hasDot)
                  {
                    lex+= char_ch;
                    return LexItem(ERR, lex, linenumber);
                    exit(1);
                }
                else if(char_ch == '.' && !hasDot){
                    lex += char_ch;
                    hasDot = true;
                    }

                else{
                    in.putback(char_ch);
                    return LexItem(RCONST, lex, linenumber);
                    }
                break;
        }
        }
    return LexItem(DONE, "DONE", linenumber);
 
}

LexItem id_or_kw(const string& lexeme, int linenum) {
    static const map<string, Token> keyword_map = {
        { "writeln", WRITELN },
        { "if", IF },
        { "else", ELSE },
        { "then", THEN },
        { "write", WRITE},
        { "integer", INTEGER},
        { "string", STRING},
        { "true", TRUE},
        { "false", FALSE},
        { "begin", BEGIN },
        { "end", END },
        { "boolean", BOOLEAN},
        { "var", VAR},
        { "real", REAL},
        { "program", PROGRAM},
        { "and", AND},
        { "mod", MOD},
        { "or", OR},
        { "not", NOT},
        { "/", DIV},
        { "div", IDIV},
    };
    
    auto it = keyword_map.find(lexeme);
    if (it != keyword_map.end()) {
        return { it->second, lexeme, linenum };
    }
    
    return { IDENT, lexeme, linenum };
}

ostream& operator<<(ostream& out, const LexItem& tok){
 
    Token first = tok.GetToken();
    switch(first){
        case WRITE:
            out << "WRITE"<<endl;
            break;
        case WRITELN:
            out << "WRITELN"<<endl;
            break;
        case IF:
            out << "IF"<<endl;
            break;
        case ELSE:
            out << "ELSE"<<endl;
            break;
        case PLUS:
            out << "PLUS"<<endl;
            break;
        case MINUS:
            out << "MINUS"<<endl;
            break;
        case MULT:
            out << "MULT"<<endl;
            break;
        case DIV:
            out << "DIV"<<endl;
            break;
        case INTEGER:
            out << "INTEGER"<<endl;
            break;
        case REAL:
            out << "REAL"<<endl;
            break;
        case BOOLEAN:
            out << "BOOLEAN"<<endl;
            break;
        case STRING:
            out << "STRING"<<endl;
            break;
        case VAR:
            out << "VAR"<<endl;
            break;
        case DONE:
            out << "DONE"<<endl;
            break;
        case BEGIN:
            out << "BEGIN"<<endl;
            break;
        case END:
            out << "END"<<endl;
            break;
        case THEN:
            out << "THEN"<<endl;
            break;
        case PROGRAM:
            out << "PROGRAM"<<endl;
            break;
        case TRUE:
            out << "BCONST: \"true\""<<endl;
            break;
        case FALSE:
            out << "BCONST: \"false\""<<endl;
            break;
        case IDIV:
            out << "IDIV"<<endl;
            break;
        case MOD:
            out << "MOD"<<endl;
            break;
        case AND:
            out << "AND"<<endl;
            break;
        case OR:
            out << "OR"<<endl;
            break;
        case NOT:
            out << "NOT"<<endl;
            break;
        case DOT:
            out << "DOT"<<endl;
            break;
        case COLON:
            out << "COLON"<<endl;
            break;
        case LPAREN:
            out << "LPAREN"<<endl;
            break;
        case ASSOP:
            out << "ASSOP"<<endl;
            break;
        case RPAREN:
            out << "RPAREN"<<endl;
            break;
        case EQ:
            out << "EQ"<<endl;
            break;
        case LTHAN:
            out << "LTHAN"<<endl;
            break;
        case GTHAN:
            out << "GTHAN"<<endl;
            break;
        case COMMA:
            out << "COMMA"<<endl;
            break;
        case SEMICOL:
            out << "SEMICOL"<<endl;
            break;
        case ERR:
            out << "ERR"<<endl;
            break;
        case IDENT:
            out << "IDENT: \"" << tok.GetLexeme() <<"\""<<endl;
            break;
        case ICONST:
            out << "ICONST: \"" << tok.GetLexeme() <<"\""<<endl;
            break;    
        case RCONST:
            out << "RCONST: \"" << tok.GetLexeme() <<"\""<<endl;
            break;
        case SCONST:
            out << "SCONST: \"" << tok.GetLexeme() <<"\""<<endl;
            break;
        case BCONST:
            out << "BCONST: \"" << tok.GetLexeme() <<"\""<<endl;
            break;
    }
    
    
           
    return out;
}

int main(int argc, char* argv[]) {  
    
    map<string,int> ident_map;
    map<string,int> map_map;
    map<double,int> map_ints;
    map<string,int> bool_map;
    
    ifstream input_file(argv[1]);

         //filename
        if (argc < 2) {
            cout << "NO SPECIFIED INPUT FILE." << endl;
            return 0;
        } 

        if(input_file.is_open()) {
            input_file.seekg(0, ios::end);
            if(input_file.tellg() == 0) {
                cout << "Empty File." <<endl;
                return 0;
            }
            input_file.seekg(0, ios::beg);
        }
    //input file
        if (!input_file.is_open()) {
            cout << "CANNOT OPEN THE FILE " << argv[1] << endl;
            return 0;
        }
    // flags
    bool flag_v = false;
    bool flag_ident = false;
    bool flag_nums = false;
    bool flag_strings = false;
    bool flag_bool = false;
    string file_name = argv[1];
    string input_file_name = "";
    for (int i = 1; i < argc; i++) {
        string flag = argv[i];
        if (flag == "-v") {
            flag_v = true;
        } else if (flag == "-sconst") {
            flag_strings = true;
        }else if (flag == "-bconst"){
            flag_bool = true;
        } else if (flag == "-nconst") {
            flag_nums = true;
        } else if (flag == "-ident") {
            flag_ident = true;
        } else if ((flag != "-v" || flag != "-nconst" || flag != "-sconst" || flag != "-ident" || flag != "-bconst" )&& flag[0] == '-') {
            cout << "UNRECOGNIZED FLAG {" << flag << "}" << endl;
            return 0;
        } else{
            if (input_file_name != "") {
                cout << "ONLY ONE FILE NAME IS ALLOWED." << endl;
                return 0;
            }
            input_file_name = flag;               
        }
    }
   
    int lineNum = 1;
    
    int line_number = 1;
    
    LexItem toke = getNextToken(input_file, line_number);
    
    int numbers_num = 0;
    
    int num_strings = 0;
    
    int tokensNum = 0;
    
    int identifiers_num = 0;
    
    int bool_num = 0;
    
    
    
    while (toke.GetToken() != DONE && (toke.GetToken() != ERR || toke.GetLexeme() == "COMMENT")) {        
        
        if (flag_v) {
            cout << toke;       
         }
        
        tokensNum++;
        if (toke.GetToken() == IDENT ) {                 
            ident_map[toke.GetLexeme()]++;
            identifiers_num++;
        }
        if (toke.GetToken() == SCONST) {            
            map_map[toke.GetLexeme()]++;
            num_strings++;
        }
        if (toke.GetToken() == TRUE || toke.GetToken() == FALSE) {            
            bool_map[toke.GetLexeme()]++;
            bool_num++;
        }
        if (toke.GetToken() == ICONST || toke.GetToken() == RCONST) {
            map_ints[stod(toke.GetLexeme())]++;
            numbers_num++;

        }

        toke = getNextToken(input_file, line_number);
        if (toke.GetLinenum() > line_number) {
            lineNum++;
            line_number = toke.GetLinenum();
        }
        }
       
        if (toke.GetToken() == ERR) {
            if (flag_nums) {
                cout << "Error in line " << toke.GetLinenum() << ": Unrecognized Lexeme {" << toke.GetLexeme() << "}" <<endl;
            }
            else if (flag_strings) {
                cout << "Error in line " << toke.GetLinenum() << ": Unrecognized Lexeme {" << toke.GetLexeme() << "}" <<endl;
            }
            else {
                cout << "Error in line " << toke.GetLinenum() << ": Unrecognized Lexeme {" << toke.GetLexeme() << "}" <<endl;
            }
            return 0;
        }        
        // print toke counts
        cout<<endl;
        cout << "Lines: " << line_number-1 << endl ;
        cout << "Total Tokens: " << tokensNum << endl;
        cout << "Identifiers: " << ident_map.size() << endl;
        cout << "Numbers: " << map_ints.size() << endl;
        cout << "Booleans: " << bool_map.size() << endl;
        cout << "Strings: " << map_map.size() << endl;
    
        //-vflags     
        // Check if there are any numbers in the input
        if (flag_nums) {
            //If there are identifiers, print them
            if (identifiers_num > 1) {
                cout << "IDENTIFIERS:" << endl;
                string identifiers;
                for (auto it = ident_map.begin(); it != ident_map.end(); it++) {
                    identifiers += it->first + ", ";
                }
                // Remove the last comma and space from the string and print it
                identifiers = identifiers.substr(0, identifiers.size() - 2);
                cout << identifiers << endl;
            }  

            if (numbers_num > 0) {
                cout << "NUMBERS:" << endl;
                for (auto it = map_ints.begin(); it != map_ints.end(); it++) {
                    cout << it->first << endl;
                }
            }            
            
            if (flag_bool) {
                if (bool_num > 0) {
                    cout << "BOOLEANS:" << endl;
                    for (auto it = bool_map.begin(); it != bool_map.end(); it++) {
                        cout << it->first << endl;
                    }
                }
            }
            
            if (flag_strings) {
                if (num_strings > 0) {
                    cout << "STRINGS:" << endl;
                    for (auto it = map_map.begin(); it != map_map.end(); it++) {
                        cout << "\"" << it->first << "\"" << endl;
                    }
                }
            }

            // Exit the program with a status of 0
            return 0;
        }
    // Print out all identifiers if the flag is set and there are some present
        if (flag_ident && identifiers_num > 0) {
            cout << "IDENTIFIERS:" << endl;
            string identifiers;
            for (auto it = ident_map.begin(); it != ident_map.end(); it++) {
                identifiers += it->first + ", ";
            }
            // Remove the last comma and space from the string and print it
            identifiers = identifiers.substr(0, identifiers.size() - 2);
            cout << identifiers << endl;
        }
    
        else if (flag_bool && bool_num > 0) {
            cout << "BOOLEANS:" << endl;
            for (auto it = bool_map.begin(); it != bool_map.end(); it++) {
                cout << it->first << endl;                                         
            }
        }
        
        else if (flag_strings && num_strings > 0) {
            cout << "STRINGS:" << endl;
            for (auto it = map_map.begin(); it != map_map.end(); it++) {
                cout << "\"" << it->first << "\"" << endl;                                         
            }
        }

        // Print out all numbers if the flag is set and there are some present
        else if (flag_nums && numbers_num > 0) {
            cout << "NUMBERS:" << endl;
            for (auto it = map_ints.begin(); it != map_ints.end(); it++) {
                cout << it->first << endl;
            }
        }

        // Close the input file and exit the program with a status of 0
        input_file.close();
        return 0;

        }
