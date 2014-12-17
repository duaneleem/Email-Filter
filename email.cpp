/* ---------------------------------------------------------------------
Objective: Find valid emails in files and store them to an output file.
Name: Duane Leem
Email: DuaneLeem@alumni.ggu.edu

This program will find all valid emails from a file you give it and output
the results to a designated output file.  This program was developed as 
part of my final project in my C++ class. I've marked up the file so it's
easier to understand what's going.  If you have any question, please feel
free to email me anytime.
--------------------------------------------------------------------- */

//libraries
#include <algorithm> // tolower
#include <cctype>
#include <deque> // collections
#include <fstream> // file I/O operations
#include <iostream>
#include <string>
using namespace std;

//Programmer defined data types
struct Emails
{
  string email; // stores emails
}; // Emails

//Special compiler dependent definitions
//NONE

//global constants/variables
//NONE

//Programmer defined functions
void intro(); // program information
string setInputFileName(); // sets input file name
string setOutputFileName(string iFileName); // sets output file name
void findEmails(string iFileName, string oFileName); // find valid emails & store to output file.
bool isValidEmailCharacter(char c); // check if character is valid.

// required for conversion to lowercase
class toLower {public: char operator()(char c) const {return tolower(c);}};

//main program
int main()
{
  // Output my name and objective and program information.
  intro();

  // Variables
  string iFileName; // input file name
  string oFileName; // output file name

  // Ask for input file name.
  iFileName = setInputFileName(); // sets input file name

  // Ask for output file name.
  oFileName = setOutputFileName(iFileName); // sets output file name

  // Display chosen file names.
  cout << endl; // separator
  cout << "Input file: " << iFileName << endl; // output user's designated input file name.
  cout << "Output file: " << oFileName << endl; // output user's desginated output file name.

  // output a prompt to press ENTER key to continue
  cout << endl << "Press [ENTER] key to continue " << endl;
  cin.ignore(1000, 10); // waits for user to hit <enter> according to page 75.

  // display lines with @ symbols
  findEmails(iFileName, oFileName);
} // main

void intro()
{
  // Output information about this program.
  cout << endl << "**********************************************" << endl;
  cout << "Objective: Find valid emails in files and store them to an output file.\n"; 
  cout << "Programmer: Duane Leem\n"; 
  cout << "Editor(s) used: Sublime Text 2\n"; 
  cout << "Compiler(s) used: VC++ 2013\n"; 
  cout << "File: " << __FILE__ << endl; 
  cout << "Compiled: " << __DATE__ << " at " << __TIME__ << endl;
  cout << "**********************************************" << endl << endl;
} // intro

string setInputFileName()
{
  // Variables
  string iFileName; // name of input file
  string dFileName = "fileContainingEmails.txt"; // default file name
  bool blankFlag = false; // checks for blank user input

  // Request name of file.
  cout << "Enter input filename [default: fileContainingEmails.txt]: ";
  getline(cin, iFileName);

  // if user didn't input anything, set it to fileContainingEmails.txt
  if (iFileName.length() == 0) blankFlag = true;

  // decides which file name to return to main program.
  if (blankFlag == true) iFileName = dFileName;

  return iFileName;
} // setInputFileName

string setOutputFileName(string iFileName)
{
  // Variables
  string oFileName; // name of output file
  string dFileName = iFileName; // default output file name
  bool blankFlag = false; // checks for blank user input

  // Change default file name if original input file name was default.
  if (iFileName == "fileContainingEmails.txt") dFileName = "copyPasteMyEmails.txt";

  // Request name of file.
  cout << "Enter output filename [default: " << dFileName << "]: ";
  getline(cin, oFileName);

  // if user didnt input anything, set it to x.txt
  if (oFileName.length() == 0) blankFlag = true;

  // decides which file name to return to main program.
  if (blankFlag == true) oFileName = dFileName;

  return oFileName;
} // setOutputFileName

void findEmails(string iFileName, string oFileName)
{
  // Variables
  ifstream fin; // used for file input operations
  ofstream fout; // file out operations
  string fileLine; // a line from the file
  string tmpEmail; // single email object (temp)
  string tmpEmails; // collection (temp)
  deque<Emails> emails; // empty email list

  // Open file.
  fin.open(iFileName.c_str());
  if (!fin.good()) "Throw I/O error";

  // Traverse file and look for @ symbols.
  while (fin.good())
  {
    getline(fin, fileLine); // grab a line from file

    // Traverse line to look for @
    for (int i = 0; i < fileLine.length(); i++)
    {
      // Displays line if an @ symbol is found.
      if (fileLine[i] == '@')
      {
        // Variables
        int s = i; // start of email
        int e = i; // end of email
        bool hasDot = false; // checks for dot in the email.
        bool isDuplicate = false; // if it's a duplicate email found
        Emails anEmail; // object for eamil

        // Checks before the @
        do
        {
          // decrements position to first valid character
          s--;

          // break if less than 0
          if (s < 0) break;
        } while (isValidEmailCharacter(fileLine[s]));

        // increments position to first valid character.
        s++;

        // Checks after the @
        do
        {
          // increments to first valid character
          e++;

          // break when at the end of file line
          if (e == fileLine.length()) break;

          // if a dot is found, set hasDot to true.
          if (fileLine[e] == '.') hasDot = true;
        } while (isValidEmailCharacter(fileLine[e]));
        
        // Display valid email.
        if (hasDot)
        {
          // Store to object.
          anEmail.email = fileLine.substr(s, e-s);

          // Check if it's a duplicate.
          for (int count = 0; count < emails.size(); count++)
          {
            // temporary lowercase to compare
            tmpEmail = anEmail.email; // single object
            tmpEmails = emails[count].email; // collection
            transform(tmpEmail.begin(), tmpEmail.end(), tmpEmail.begin(), toLower());
            transform(tmpEmails.begin(), tmpEmails.end(), tmpEmails.begin(), toLower());

            // check for duplicate
            if (tmpEmail == tmpEmails)
            {
              isDuplicate = true;
              break; // break loop
            } // if
          } // lowercase and then checks for duplicates

          // Stores in list as long as it isn't a duplicate.
          if (!isDuplicate)
          {
            emails.push_back(anEmail); // store to collection
            cout << anEmail.email << endl;
          } // if not duplicate
        } // Valid Email
      } // if
    } // for
  } // while

  // Tell user there are no @'s if none were found.
  if (emails.size() == 0)
  {
    cout << endl << "Sorry, no email addresses were found in the file " << iFileName << endl;
  }
  else
  {
    // store emails to output file.
    fout.open(oFileName.c_str()); // opens file
    if (!fout.good()) throw "I/O error"; // checks file
    for (int count = 0; count < emails.size(); count++)
    {
      // write email to output file.
      fout << emails[count].email;
      
      // don't write a semicolon at the last email.
      if (count != emails.size()-1) fout << "; ";
    } // traverses collection and writes to file.
    fout.close(); // closes file

    // inform user
    cout << endl << emails.size() << " email addresses were found, and copied to the file " << oFileName << endl;

    // give instructions on usage.
    cout << endl << "Usage: Open the output file " << oFileName << " and copy/paste the contents\nin the TO, CC, or BCC fields of the email you are composing.  It's\nbest that you use the BCC field so that everyone's email is kept\nconfidential to protect their privacy." << endl;
  } // if

  // Close file.
  fin.close();
} // findEmails

bool isValidEmailCharacter(char c)
{
  // Variables
  bool result = false;

  // Determine if email character is valid.
  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= 48 && c <= 57) || (c == '_') || (c == '.') || (c == '-') || (c == '+'))
  {
    result = true;
  } // determines if character of email is valid.

  return result;
} // isValidEmailCharacter