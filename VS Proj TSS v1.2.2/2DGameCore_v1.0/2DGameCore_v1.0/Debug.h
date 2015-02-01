// Debug.h - External declaration of global 
// output stream object

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <fstream>
#include <string>
using std::ofstream;
using std::endl;
using std::string;

extern ofstream debug;

void FileNotFoundMsg (const string& file);
void ErrorMsg (const string& msg);

#endif // __DEBUG_H__