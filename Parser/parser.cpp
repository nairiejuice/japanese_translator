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

    // global buffer for the token the scanner returned.
tokentype saved_token;
string saved_lexeme;
string response;
bool token_available = false;
void story();
void s();
void afterSubject();
void afterNoun();
void afterObject();
void noun();
void verb();
void be();
void tense();
tokentype next_token();
bool match(tokentype);

//method to turn on and off tracing messages
const bool Tracing = false; //change to false to disable tracing mesages

void tracing (string grammerType){
   if (Tracing){
      cout << "Processing " << grammerType << endl;
   }
}

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.  

// Type of error: mismatch error
// Done by: Kelyn
void syntaxerror1(tokentype expected, string given){
   cout<<"SYNTAX ERROR: expected "<< tokenName[expected] << " but found "<< given<<endl;
   cout<<"Skip or replace the token? (s or r)";
   cin >> response;
   if(response == "r"){
      saved_token = expected;
      match(expected);
   }
   else if(response == "s"){
      token_available = false;
      match(expected);
   }
   else{
      cout<<"ERROR: value entered not a valid choice"<<endl;
      exit(1);
   }
   //exit(1);
}
// Type of error: Lexical
// Done by: Wesley
void syntaxerror2(string saved_lexeme, string function) {
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
   }
   return saved_token; // return the saved token
}

// Purpose: Checkssand eats up the expected token.
// How: Check to see if expected is different from next_token()
//       and if so, generates a syntax error and handles the error
//       else token_available becomes false (eat up) and returns true.    
// Done by: Wesley & Kelyn
bool match(tokentype expected) {
   if (!(next_token() == expected)) { // mismatch has occurred with the next token
      // calls a syntax error function here to generate a syntax error message here and do recovery
      //syntax_error2(saved_lexeme, ……)
      syntaxerror1(expected, saved_lexeme); // this might need to be edited with the correct argument
   } else {
      token_available = false; // eat up the token
      //cout << "we are in match, token has been eaten up" << endl;
      cout << "Matched " << tokenName[expected] << endl;
   }
   return true; // say there was a match
}

// ----- RDP functions - one per non-term -------------------

// 1 (refer to Project-partB_w12A14A)
// Grammar: <story> ::= <s> { <s> }  // stay in the loop as long as a possible start 
// Done by: Wesley
void story() {
   //cout << "Processing <story>\n" << endl;
   tracing("<story>");
   s();
   while (true) {
       //next_token() == 2 would be 'eofm'
       if (next_token() == 2) {
         break;
       } else {
         s();
       }
   }
   if (Tracing){
      cout << "Successfully parsed <story>." << endl;
   }
   
}

// Grammar: <s>::= [CONNECTOR] <noun> SUBJECT <after subject>
// Done by: Kelyn
void s(){
    //cout << "Processing <s>" << endl;
    tracing("<s>");
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
   //cout << "Processing <afterSubject>" << endl;
   tracing("<afterSubject>");
       switch(next_token()){
            case WORD2:
                verb();
                tense();
                match(PERIOD);
                break;
         
            case WORD1:
            case PRONOUN: // noun might be multiple things
                noun();
                afterNoun();
                break;
            default:
               syntaxerror2(saved_lexeme, "afterSubject");
       }
}

// Grammar: <after noun> ::= <be> PERIOD | DESTINATION <verb> <tense> PERIOD| OBJECT <after object>   
// Done by: Kelyn
void afterNoun(){
   //cout << "Processing <afterNoun>" << endl;
   tracing("<afterNoun>");
   switch(next_token()){
      case IS:
      case WAS:
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
         syntaxerror2(saved_lexeme, "afterNoun");
   }
}

//Grammar: <after object> ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> <tense> PERIOD
//Done by: Kelyn
void afterObject(){
   //cout << "Processing <afterObject>" << endl;
   tracing("<afterObject>");
   switch(next_token()){
      case WORD2:
         verb();
         tense();
         match(PERIOD);
         break;
      case WORD1:
      case PRONOUN:
         noun();
         match(DESTINATION);
         verb();
         tense();
         match(PERIOD);
         break;
      default:
         syntaxerror2(saved_lexeme, "afterObject");
   }
}

// 7
// Grammar: <noun> ::= WORD1 | PRONOUN 
// Done by: Kelyn
void noun(){
   //cout << "Processing <noun>" << endl;
   tracing("<noun>");
   switch(next_token()){
      case WORD1:
         match(WORD1);
         break;
      case PRONOUN:
         match(PRONOUN);
         break;
      default:
         syntaxerror2(saved_lexeme, "noun");
   }
}

// 8
// Grammar: <verb> ::= WORD2
// Done by: Wesley
void verb(){
   //cout << "Processing <verb>" << endl;
   tracing("<verb>");
   match(WORD2);
}

// 9
// Grammar: <be> ::=   IS | WAS
// Done by: Wesley
void be(){
   //cout << "Processing <be>" << endl;
   tracing("<be>");
   switch (next_token()) {
      case IS:
         match(IS);
         break;
      case WAS:
         match(WAS);
         break;
      default:
         syntaxerror2(saved_lexeme, "be"); // need to pass something in
   }
}

// 10
// Grammar: <tense> := VERBPAST  | VERBPASTNEG | VERB | VERBNEG
// Done by: Wesley
void tense(){
   //cout << "Processing <tense>" << endl;
   tracing("<tense>");
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
         syntaxerror2(saved_lexeme, "tense"); // need to pass something in
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

