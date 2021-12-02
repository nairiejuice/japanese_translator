#include<iostream>
#include<fstream>
#include<string>
#include "scanner.cpp"
using namespace std;

//#include <iostream>
//#include <fstream>
//#include <string>
//#include "scanner.cpp"
//using namespace std;

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
ofstream fout;
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
void getEword();
void gen(string line_type);

// Done by: Jacob
// method to turn on and off tracing messages
const bool Tracing = false; // change to false to disable tracing mesages

void tracing(string grammarType)
{
   if (Tracing)
   {
      cout << "Processing " << grammarType << endl;
   }
}

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.

// Type of error: mismatch error
// Done by: Kelyn
void syntaxerror1(tokentype expected, string given)
{
   cout << "SYNTAX ERROR: expected " << tokenName[expected] << " but found " << given << endl;
   fout << "SYNTAX ERROR: expected " << tokenName[expected] << " but found " << given << endl;
   cout << "Skip or replace the token? (s or r)";
   cin >> response;
   if (response == "r")
   {
      saved_token = expected;
      match(expected);
   }
   else if (response == "s")
   {
      token_available = false;
      match(expected);
   }
   else
   {
      cout << "ERROR: value entered not a valid choice" << endl;
      exit(1);
   }
   // exit(1);
}
// Type of error: Lexical
// Done by: Wesley
void syntaxerror2(string saved_lexeme, string function)
{
   cout << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in " << function << endl;
   fout << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in " << function << endl;
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
tokentype next_token()
{
   if (!token_available)
   {                                      // if there is no saved token yet
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
// Done by: Wesley, Kelyn & Jacob
bool match(tokentype expected)
{
   if (!(next_token() == expected))
   { // mismatch has occurred with the next token
      // calls a syntax error function here to generate a syntax error message here and do recovery
      // syntax_error2(saved_lexeme, ……)
      syntaxerror1(expected, saved_lexeme); // this might need to be edited with the correct argument
   }
   else
   {
      token_available = false; // eat up the token
      // cout << "we are in match, token has been eaten up" << endl;
      cout << "Matched " << tokenName[expected] << endl;
   }
   return true; // say there was a match
}

// ----- RDP functions - one per non-term -------------------

// 1 (refer to Project-partB_w12A14A)
// Grammar: <story> ::= <s> { <s> }  // stay in the loop as long as a possible start
// Done by: Wesley
void story()
{
   // cout << "Processing <story>\n" << endl;
   tracing("<story>");
   s();
   while (true)
   {
      // next_token() == 2 would be 'eofm'
      if (next_token() == 2)
      {
         break;
      }
      else
      {
         s();
      }
   }
   if (Tracing)
   {
      cout << "Successfully parsed <story>." << endl;
   }
}

// Grammar: <s>::= [CONNECTOR #getEword#   #gen(“CONNECTOR”)# ] <noun> #getEword#   SUBJECT #gen(“ACTOR”)# <after subject>
// Done by: Kelyn & Jacob
void s()
{
   // cout << "Processing <s>" << endl;
   tracing("<s>");
   if (next_token() == CONNECTOR)
   {
      match(CONNECTOR);
      getEword();
      gen("CONNECTOR");
   }
   noun();
   getEword();
   match(SUBJECT);
   gen("ACTOR");
   afterSubject();
}

// Grammar: <after subject> ::= <verb>#getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | <noun> #getEword#   <after noun>
// Done by: Kelyn & Jacob
void afterSubject()
{
   // cout << "Processing <afterSubject>" << endl;
   tracing("<afterSubject>");
   switch (next_token())
   {
   case WORD2:
      verb();
      getEword();
      gen("ACTION");
      tense();
      gen("TENSE");
      match(PERIOD);
      break;

   case WORD1:
   case PRONOUN: // noun might be multiple things
      noun();
      getEword();
      afterNoun();
      break;
   default:
      syntaxerror2(saved_lexeme, "afterSubject");
   }
}

// Grammar: <after noun> ::= <be> #gen(“DESCRIPTION”)# #gen(“TENSE”)#   PERIOD | DESTINATION #gen(“TO”)#  <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | OBJECT #getEword#  #gen(“OBJECT”)# <after object>  
// Done by: Kelyn & Jacob
void afterNoun()
{
   // cout << "Processing <afterNoun>" << endl;
   tracing("<afterNoun>");
   switch (next_token())
   {
   case IS:
   case WAS:
      be();
      gen("DESCRIPTION");
      gen("TENSE");
      match(PERIOD);
      break;
   case DESTINATION:
      match(DESTINATION);
      gen("TO");
      verb();
      getEword();
      gen("ACTION");
      tense();
      gen("TENSE");
      match(PERIOD);
      break;
   case OBJECT:
      match(OBJECT);
      gen("OBJECT");
      afterObject();
      break;
   default:
      syntaxerror2(saved_lexeme, "afterNoun");
   }
}

// Grammar: <after object> ::= <verb> #getEword#  #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | <noun> #getEword#  DESTINATION #gen(“TO”)#  <verb> #getEword#  #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD
// Done by: Kelyn & Jacob
void afterObject()
{
   // cout << "Processing <afterObject>" << endl;
   tracing("<afterObject>");
   switch (next_token())
   {
   case WORD2:
      verb();
      getEword();
      gen("ACTION");
      tense();
      gen("TENSE");
      match(PERIOD);
      break;
   case WORD1:
   case PRONOUN:
      noun();
      getEword();
      match(DESTINATION);
      gen("TO");
      verb();
      getEword();
      gen("ACTION");
      tense();
      gen("TENSE");
      match(PERIOD);
      break;
   default:
      syntaxerror2(saved_lexeme, "afterObject");
   }
}

// 7
// Grammar: <noun> ::= WORD1 | PRONOUN
// Done by: Kelyn & Jacob
void noun()
{
   // cout << "Processing <noun>" << endl;
   tracing("<noun>");
   switch (next_token())
   {
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
void verb()
{
   // cout << "Processing <verb>" << endl;
   tracing("<verb>");
   match(WORD2);
}

// 9
// Grammar: <be> ::=   IS | WAS
// Done by: Wesley & Jacob
void be()
{
   // cout << "Processing <be>" << endl;
   tracing("<be>");
   switch (next_token())
   {
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
void tense()
{
   // cout << "Processing <tense>" << endl;
   tracing("<tense>");
   switch (next_token())
   {
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
// int main()
// {
//    cout << "Enter the input file name: ";
//    cin >> filename;
//    fin.open(filename.c_str());
//    fout.open("errors.txt");

//    //** calls the <story> to start parsing
//    //** closes the input file
//    story();
//    fin.close();
//    fout.close();

// } // end
  //** require no other input files!
  //** syntax error EC requires producing errors.txt of error messages
  //** tracing On/Off EC requires sending a flag to trace message output functions

//=================================================
// File translator.cpp written by Group Number: 4
//=================================================

// ----- Additions to the parser.cpp ---------------------
map<string, string> dictionary;
string saved_E_word;
string japanese_word;
string english_word;

//  Done by: Kelyn & Jacob
// ** Declare Lexicon (i.e. dictionary) that will hold the content of lexicon.txt
// Make sure it is easy and fast to look up the translation.
// Do not change the format or content of lexicon.txt
void loadMap(){
    while(fin >> japanese_word >> english_word){
        dictionary[japanese_word] = english_word;
    }
}

//  Done by: Wesley & Kelyn
// ** Additions to parser.cpp here:
//    getEword() - using the current saved_lexeme, look up the English word
//                 in Lexicon if it is there -- save the result
//                 in saved_E_word
void getEword() {
  if (dictionary.find(saved_lexeme) != dictionary.end()) {
    saved_E_word = dictionary.at(saved_lexeme);
  } else {
    saved_E_word = saved_lexeme;
  }
}

//  Done by: Wesley & Jacob
//    gen(line_type) - using the line type,
//                     sends a line of an IR to translated.txt
//                     (saved_E_word or saved_token is used)
void gen(string line_type) {
  if (line_type == "TENSE") {
    cout << line_type << ": " << tokenName[saved_token] << endl << endl;
    fout << line_type << ": " << tokenName[saved_token] << endl << endl;
  } else {
    cout << line_type << ": " << saved_E_word << endl;
    fout << line_type << ": " << saved_E_word << endl;
  }
}

// ----- Changes to the parser.cpp content ---------------------

// ** Comment update: Be sure to put the corresponding grammar 
//    rule with semantic routine calls
//    above each non-terminal function 

// ** Each non-terminal function should be calling
//    getEword and/or gen now.


// ---------------- Driver ---------------------------

// The final test driver to start the translator
// Done by:  Wesley & Kelyn
int main()
{
  //** opens the lexicon.txt file and reads it into Lexicon
  //** closes lexicon.txt 
  fin.open("lexicon.txt");
  loadMap();
  fin.close();

  //** opens the output file translated.txt
  fout.open("translated.txt");

  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.c_str());

  //** calls the <story> to start parsing
  story();

  //** closes the input file 
  //** closes traslated.txt
  fin.close();
  fout.close();
 
}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions

