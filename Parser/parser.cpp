#include<iostream>
#include<fstream>
#include<string>
#include "scanner.cpp"
using namespace std;

/* INSTRUCTION:  Complete all ** parts.
   You may use any method to connect this file to scanner.cpp
   that you had written.  
  e.g. You can copy scanner.cpp here by:
          cp ../ScannerFiles/scanner.cpp .  
       and then append the two files into one: 
          cat scanner.cpp parser.cpp > myparser.cpp
*/

//=================================================
// File parser.cpp written by Group Number: 4
//=================================================

// ----- Four Utility Functions and Globals -----------------------------------

tokentype  saved_token;     // global buffer for the token the scanner returned.
string saved_lexeme;
bool token_available = false;

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.  

// Type of error: mismatch error
// Done by: Kelyn
void syntaxerror1(tokentype expected, string given){
   cout<<"SYNTAX ERROR: expected "<< expected<< " but found "<< given<<endl;
   exit(1);
    }
// Type of error: **
// Done by: Wesley
void syntaxerror2(parser_function function) {
   cout << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in " << function << endl;
   exit(1);
}

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Purpose: Looks ahead to see what token comes next from the scanner.
// How: Check first to see if the token_available flag is false.
//       If so, saved_token gets scanner() result.
//       and the flag is set to true.
//      A token is grabbed but is not eaten up.
//      Returns the saved_token
// Done by: Wesley
tokentype next_token() {
   if (!token_available) { // if there is no saved token yet
      scanner(saved_token, saved_lexeme); // call scanner to grab a new token
                                          // saved_token is the token type and
                                          // saved_lexeme is the word that is read in
                                          // mark the fact that you have saved it
      token_available = true;
      if (saved_token == ERROR) {
         syntax_error1(saved_token, saved_lexeme);
      }
   }
   return saved_token; // return the saved token
}

// Purpose: Checkssand eats up the expected token.
// How: Check to see if expected is different from next_token()
//       and if so, generates a syntax error and handles the error
//       else token_available becomes false (eat up) and returns true.    
// Done by: Wesley & Kelyn
boolean match(tokentype expected) {
   if (!(next_token() == expected)) { // mismatch has occurred with the next token
      // calls a syntax error function here to generate a syntax error message here and do recovery
      //syntax_error2(saved_lexeme, ……)
      syntaxerror1(expected, saved_lexeme); // this might need to be edited with the correct argument
   } else {
      token_available = false; // eat up the token
      cout << "we are in match, token has been eaten up" << endl;
      return true; // say there was a match
       }
}

// ----- RDP functions - one per non-term -------------------

// 1 (refer to Project-partB_w12A14A)
// Grammar: <story> ::= <s> { <s> }  // stay in the loop as long as a possible start 
// Done by: Wesley
void story() {
   cout << "this is the story function" << endl;
   s();
   while (!next_token == EOFM) {
      s();
   }
   cout << "end of story function" << endl;
}

// Grammar: <s>::= [CONNECTOR] <noun> SUBJECT <after subject>
// Done by: Kelyn
void s(){
    if(next_token() == CONNECTOR){
       match(CONNECTOR);
    }
    noun();
    match(SUBJECT);
    afterSubject();
}

// Grammar: <after subject> ::= <verb> <tense> PERIOD | <noun> <after noun>
// Done by: Kelyn
void afterSubject(){
       switch(next_token()){
            case verb:
                verb();
                tense();
                match(PERIOD);
                break;
         
            case noun:
                noun();
                afterNoun();
                break;
            default:
               syntaxerror2();
       }
}

// Grammar: <after noun> ::= <be> PERIOD | DESTINATION <verb> <tense> PERIOD| OBJECT <after object>   
// Done by: Kelyn
void afterNoun(){
   switch(next_token()){
      case be:
         be();
         match(PERIOD);
         break;
      case DESTINATION:
         match(DESTINATION);
         verb();
         tense();
         match(PERIOD);
         break;
      case OBJECT:
         match(OBJECT);
         afterObject();
         break;
      default:
         syntaxerror2();
   }
}

//Grammar: <after object> ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> <tense> PERIOD
//Done by: Kelyn
void afterObject(){
   switch(next_token()){
      case verb:
         verb();
         tense();
         match(PERIOD);
         break;
      case noun:
         noun();
         match(DESTINATION);
         verb();
         tense();
         match(PERIOD);
         break;
      default:
         syntaxerror2();
   }

}

// 7
// Grammar: <noun> ::= WORD1 | PRONOUN 
// Done by: Kelyn
void noun(){
   switch(next_token()){
      case WORD1:
         match(WORD1);
         break;
      case PRONOUN:
         match(PRONOUN);
         break;
      default:
         syntaxerror2();
   }
}

// 8
// Grammar: <verb> ::= WORD2
// Done by: Wesley
void verb(){
   cout << "this is the verb function" << endl;
   match(WORD2);
}

// 9
// Grammar: <be> ::=   IS | WAS
// Done by: Wesley
void be(){
   cout << "this is the be function" << endl;
   switch (next_token()) {
      case IS:
         match(IS);
         break;
      case WAS:
         match(WAS);
         break;
      default:
         syntaxerror2(); // need to pass something in
   }
}

// 10
// Grammar: <tense> := VERBPAST  | VERBPASTNEG | VERB | VERBNEG
// Done by: Wesley
void tense(){
   cout << "this is the tense function" << endl;
   switch (next_token()) {
      case VERBPAST:
         match(VERBPAST);
         break;
      case VERBPASTNEG:
         match(VERBPASTNEG);
         break;
      case VERB:
         match(VERB);
         break;
      case VERBNEG:
         match(VERBNEG);
         break;
      default:
         syntaxerror2(); // need to pass something in
   }
}


string filename;

//----------- Driver ---------------------------

// The new test driver to start the parser
// Done by:  Wesley
int main()
{
  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.c_str());

  //** calls the <story> to start parsing
  //** closes the input file
  story();
  fin.close();

}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions