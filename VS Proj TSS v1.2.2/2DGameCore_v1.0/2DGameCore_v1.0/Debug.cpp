// Debug.cpp - External definition of global 
// output stream object 

#include "Debug.h"

ofstream debug ("ErrorLog.txt");

void FileNotFoundMsg (const string& file)
{  
	debug << "File not found: " << file << endl;
}

void ErrorMsg (const string& msg)
{  
	debug << "ERROR: " << msg << endl;
}