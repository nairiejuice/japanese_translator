#include<iostream>
#include<fstream>
#include<string>
#include <map> //in order to check if the word exists in the reservedwords table
using namespace std;

/* Look for all **'s and complete them */

//=====================================================
// File scanner.cpp written by: Group Number: 4
//=====================================================

// --------- Two DFAs ---------------------------------

// WORD DFA 
// Done by: Jacob & Wesley
// RE:   **
bool word (string s)
{

  int state = 0;
  /*
  0 = q0
  1 = q0q1
  2 = q0qy
  3 = qsa
  4 = qs
  5 = qc
  6 = qt
  7 = qy
  */
  int charpos = 0;
  /* replace the following todo the word dfa  */
  while (s[charpos] != '\0') 
    {
      //at q0
      if (state == 0){
        switch (s[charpos]){
          //vowel -> q0q1
          case 'a':
          case 'e':
          case 'i':
          case 'E':
          case 'I':
          case 'o':
          case 'u': state = 1; break;
          //'dwzjy' -> qsa
          case 'd':
          case 'w':
          case 'z':
          case 'j':
          case 'y': state = 3; break;
          //s -> qs
          case 's': state = 4; break;
          //c -> qc
          case 'c': state = 5; break;
          //'bghkmnpr' -> qy
          case 'b':
          case 'g':
          case 'h':
          case 'k':
          case 'm':
          case 'n':
          case 'p':
          case 'r': state = 7; break;
          //t -> qt
          case 't': state = 6; break;
          
          default: return false;
        }
        //at q0q1
      } else if (state == 1) {
        switch (s[charpos]) {
          case 'a':
          case 'e':
          case 'E':
          case 'i':
          case 'I':
          case 'o':
          case 'u': state = 1; break;
          case 'n': state = 2; break;
          //'dwzjy' -> qsa
          case 'd':
          case 'w':
          case 'z':
          case 'j':
          case 'y': state = 3; break;
          //'s' -> qs
          case 's': state = 4; break;
          //'c; -> qc
          case 'c': state = 5; break;
          //'t' -> qt
          case 't': state = 6; break;
          //'bghkmpr' -> qy
          case 'b':
          case 'g':
          case 'h':
          case 'k':
          case 'm':
          case 'p':
          case 'r': state = 7; break;
          
          
          default: return false;
        }
      } else if (state == 2) {
        switch (s[charpos]) {
          case 'b':
          case 'g':
          case 'h':
          case 'k':
          case 'm':
          case 'n':
          case 'p':
          case 'r': state = 7; break;
          
          case 'd':
          case 'w':
          case 'z':
          case 'j':
          case 'y': state = 3; break;

          case 'a':
          case 'e':
          case 'i':
          case 'E':
          case 'I':
          case 'o':
          case 'u': state = 1; break;
          
          case 'c': state = 5; break;

          case 's': state = 4; break;

          case 't': state = 6; break;

          default: return false;
        }
  /*
  0 = q0
  1 = q0q1
  2 = q0qy
  3 = qsa
  4 = qs
  5 = qc
  6 = qt
  7 = qy
  */
      } else if (state == 3) {
        switch (s[charpos]) {
          case 'a':
          case 'e':
          case 'i':
          case 'E':
          case 'I':
          case 'o':
          case 'u': state = 1; break;
          default: return false;
        }
      } else if (state == 4) {
        switch (s[charpos]) {
          case 'h': state = 3; break;
          case 'a':
          case 'e':
          case 'i':
          case 'E':
          case 'I':
          case 'o':
          case 'u': state = 1; break;
          default: return false;
        }
      } else if (state == 5) {
        switch (s[charpos]) {
          case 'h': state = 3; break;

          default: return false;
        }
      } else if (state == 6) {
        switch (s[charpos]) {
          case 'a':
          case 'e':
          case 'E':
          case 'i':
          case 'I':
          case 'o':
          case 'u': state = 1; break;

          case 's': state = 3; break;

          default: return false;
        }
      } else if (state == 7) {
        switch (s[charpos]) {
          case 'a':
          case 'e':
          case 'E':
          case 'i':
          case 'I':
          case 'o':
          case 'u': state = 1; break;

          case 'y': state = 3; break;

          default: return false;
        }
      }
      else return(false);
      charpos++;
    }//end of while

  // where did I end up????
  if (state == 0 || state == 1 || state == 2) return(true);  // end in a final state
  //if (1 == 1) return true;
   else return(false);
  //
}

// PERIOD DFA 
// Done by: Wesley
bool period (string s)
{  // complete this **
  if (s[0] == '.') return true;
  return false;
}

// ------ Three  Tables -------------------------------------

// TABLES Done by: Wesley

// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, EOFM, etc.
enum tokentype {WORD1, WORD2, PERIOD, ERROR, EOFM, VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR};

// ** For the display names of tokens - must be in the same order as the tokentype.
string tokenName[30] = {"WORD1", "WORD2", "PERIOD", "ERROR", "EOFM", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR"}; 

// ** Need the reservedwords table to be set up here. 
// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.

map<string, tokentype> reservedwords =
    {
        {"masu", tokentype::VERB},
        {"masen", tokentype::VERBNEG},
        {"mashita", tokentype::VERBPAST},
        {"masendeshita", tokentype::VERBPASTNEG},
        {"desu", tokentype::IS},
        {"deshita", tokentype::WAS},

        {"o", tokentype::OBJECT},
        {"wa", tokentype::SUBJECT},
        {"ni", tokentype::DESTINATION},
        
        {"watashi", tokentype::PRONOUN},
        {"anata", tokentype::PRONOUN},
        {"kare", tokentype::PRONOUN},
        {"kanojo", tokentype::PRONOUN},
        {"sore", tokentype::PRONOUN},

        {"mata", tokentype::CONNECTOR},
        {"soshite", tokentype::CONNECTOR},
        {"shikashi", tokentype::CONNECTOR},
        {"dakara", tokentype::CONNECTOR},

        {"eofm", tokentype::EOFM}};

// ------------ Scaner and Driver ----------------------- 

ifstream fin;  // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// ** Done by: Wesley & Kelyn
int scanner(tokentype& tt, string& w)
{

  // ** Grab the next word from the file via fin
  // 1. If it is eofm, return right now.   

  fin >> w;
  if (w == "eofm") return 0;

  /*  **
  2. Call the token functions (word and period) 
     one after another (if-then-else).
     Generate a lexical error message if both DFAs failed.
     Let the tokentype be ERROR in that case.
  */
//edited by Kelyn
  //cout << "we are at: " << w << endl;
  if (word(w)){
      if (reservedwords.find(w) != reservedwords.end()){
        tt = reservedwords.at(w); //assigns tt the token type at the word found in reserved words table
      }
      else if (w[w.length() - 1] == 'E' || w[w.length() - 1] == 'I') {
        cout << "letter is: " << w[w.length() - 1] << ", word is: " << w << endl;
        tt = WORD2;
      } 
      else {
        tt = WORD1;
      }
      return 0;
  }
  else if(period(w)){
    tt = PERIOD;
    return 0;
  }
  else{
    tt = ERROR;
    //cout << "You have encountered a Lexical Error OH NO!!" << endl;
  }

  /*
  3. If it was a word,
     check against the reservedwords list.
     If not reserved, tokentype is WORD1 or WORD2
     decided based on the last character.
  */
 //edited by Kelyn


  //4. Return the token type & string  (pass by reference)

  //We just have to assign it
  return 0;

}//the end of scanner



// The temporary test driver to just call the scanner repeatedly  
// This will go away after this assignment
// DO NOT CHANGE THIS!!!!!! 
// Done by:  Rika
int main()
{
  tokentype thetype;
  string theword; 
  string filename;

  cout << "Enter the input file name: ";
  cin >> filename;

  fin.open(filename.c_str());

   while (true)
    {
       scanner(thetype, theword);  // call the scanner which sets
                                   // the arguments  
       if (theword == "eofm") break;  // stop now
     
      
      cout << "Type is:" << tokenName[thetype] << endl;
      cout << "Word is:" << theword << endl;  
    }

   cout << "End of file is encountered." << endl;
   fin.close();

}// end

