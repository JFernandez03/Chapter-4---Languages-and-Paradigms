#include <iostream>
#include <fstream>
#include <cstdlib> 
using namespace std;

ifstream infp;
enum Tokens { A, B, C, ENDFILE=80, UNKNOWN=99 };
			
const int SIZE = 100;
Tokens nextTokens;
string lexeme;
char nextChar;

int errors = 0; // counter for error messages
int line = 1;   // variable to keep track of the line number from the source code

/******************************************************/
/*   Helping function to display the token as a string */
void prt (Tokens nt) {
	switch (nt) {
		// Type your code here
		case A: cout << "<A>"; break;
		case B: cout << "<B>"; break;
		case C: cout << "<C>"; break;
		case ENDFILE: cout << "<ENDFILE>"; break;
		case UNKNOWN: cout << "<UNKNOWN>"; break;
		default: cout << "<ERROR>";
	}
};

/* errMsg - function to display the error message with the line number of the error detected. */
void errMsg (string msg) {
	cout << "Error at line: " << line << ": " << msg << endl;
	errors++;
	
}

/*****************************************************/
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(char nextChar) {
	lexeme += nextChar;
	if (lexeme.size() > 99) {
		errMsg ("Lexeme size exceeds 100 characters");
		exit(0);
	}
}

char getChar() {
	char ch = infp.get();
	if (ch == '\n') {		
		line++;
	}
	return ch;
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
char getNonSpaceChar() {
	char ch = ' ';
	while (isspace(ch)) {
		ch = getChar();
	}
	return ch;
}

/*******************************************************************
LookupKeyword - a simple lookup code for keywords in the language: */
Tokens lookupKeywords (string lexeme) {
	// return keyword token
	// Type your code here
	if (lexeme == "a") return A;
	else if (lexeme == "b") return B;
	else if (lexeme == "c") return C;
	return UNKNOWN;
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic 
 expressions */
Tokens tokenizer() {
	Tokens nextToken = ENDFILE;
	lexeme = "";
	if (isspace(nextChar)) nextChar = getNonSpaceChar();
	
	if (nextChar == 'a') {
		// Type your code here
		addChar(nextChar);
		nextChar = getChar();
		nextToken = A;
	}
	else if (nextChar == 'b') {
		// Type your code here
		addChar(nextChar);
		nextChar = getChar();
		nextToken = B;
	}
	else if (nextChar == 'c') {
		// Type your code here
		addChar(nextChar);
		nextChar = getChar();
		nextToken = C;
	}
	else if (nextChar == EOF) {
			nextToken = ENDFILE;
			lexeme = "EOF";
	}
	else {
		addChar(nextChar);
		nextToken = UNKNOWN;
		nextChar = getChar();
	}
  
  return nextToken;
}

/*************************************************/
/* <C> -> c
*/
Tokens CParser (Tokens nextToken) {
	// Type your code here
	if (nextToken == C) {
		cout << "Parsed <C>" << endl;
		nextToken = tokenizer();  // move to next token
	} else {
		errMsg("Expected 'c' in <C>");
	}
	return nextToken;
}

/*************************************************/
/* <B> -> b<B> | b
*/
Tokens BParser (Tokens nextToken) {
	// Type your code here
	  if (nextToken == B) {
        cout << "Parsed <B>" << endl;
        nextToken = tokenizer();  // Move to next token after 'b'
        
        // Recursive case: keep parsing <B> if the next token is also 'b'
        if (nextToken == B) {
            return BParser(nextToken);
        }
    } else {
        errMsg("Expected 'b' in <B>");
    }
    return nextToken;
}

/***************************************************/
/* <A> -> a<A> | a
*/
Tokens AParser (Tokens nextToken) {
	// Type your code here
	if (nextToken == A) {
        cout << "Parsed <A>" << endl;
        nextToken = tokenizer();  // Move to next token after 'a'
        
        // Don't recurse indefinitely, check if the next token is 'a' again
        if (nextToken == A) {
            nextToken = AParser(nextToken);  // Only recurse if another 'a' is found
        }
    } else {
        errMsg("Expected 'a' in <A>");
    }
    return nextToken;  // Return after parsing <A>
}

/********************************************/
/* stmt - statement parser by the rule:
   <stmt> -> <A><C><B> | <A>
*/
Tokens S (Tokens nextToken) {
	// Type your code here
	 // First parse <A>
    nextToken = AParser(nextToken);  // Parse <A>
    
    // Check if the next token is <C> (optional case: <A><C><B>)
    if (nextToken == C) {
        nextToken = CParser(nextToken);  // Parse <C>
        nextToken = BParser(nextToken);  // Parse <B>
    }
    
    return nextToken;  // Return next token after parsing <S>
}

/******************************************* End of Syntax Analyzer (parser) **************************************************************************/

/******************************************************/
/* main driver */
int main() {
	infp.open("\\Users\\jaafe\\Desktop\\Languages and Paradigms\\Languages and Paradigms Chapter 4 Project\\prg3.in.txt");
	if (!infp) {
		cout << "ERROR - cannot open file \n";
		errors++;
	}
	else {
		nextChar = ' ';
		Tokens next = tokenizer();
		do {
			next = S (next);
			if (errors > 10) break;
		} while (next != ENDFILE);
	}
	cout << "Total number of errors: " << errors << endl;
	return 0;
}
