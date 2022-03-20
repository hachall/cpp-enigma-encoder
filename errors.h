#ifndef ERRORS_H
#define ERRORS_H

#include<iostream>
#include<map>
#include<string>
// using namespace std;

/* error codes  */
#define INSUFFICIENT_NUMBER_OF_PARAMETERS         1
#define INVALID_INPUT_CHARACTER                   2
#define INVALID_INDEX                             3
#define NON_NUMERIC_CHARACTER                     4
#define IMPOSSIBLE_PLUGBOARD_CONFIGURATION        5
#define INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS  6
#define INVALID_ROTOR_MAPPING                     7
#define NO_ROTOR_STARTING_POSITION                8
#define INVALID_REFLECTOR_MAPPING                 9
#define INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS  10
#define ERROR_OPENING_CONFIGURATION_FILE          11
#define NO_ERROR                                  0

std::map<int, std::string> const errors{
		{INSUFFICIENT_NUMBER_OF_PARAMETERS, "Insufficient number of parameters - there must be at least 3 additional files passed on the command line"},
		{INVALID_INPUT_CHARACTER, "Invalid input character - enter only capital letters [A-Z]"},
		{INVALID_INDEX, "Invalid index - mappinsg must be a number between [0-25]"},
		{NON_NUMERIC_CHARACTER, "Non numeric character - mapping files must only contain numeric characters [0-9]"},
		{IMPOSSIBLE_PLUGBOARD_CONFIGURATION, "Impossible plugboard configuration"},
		{INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS, "Incorrect number of plugboard parameters"},
		{INVALID_ROTOR_MAPPING, "Invalid rotor mapping"},
		{NO_ROTOR_STARTING_POSITION, "No rotor starting position"},
		{INVALID_REFLECTOR_MAPPING, "Invalid reflector mapping"},
		{INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS, "Incorrect number of reflector parameters"},
		{ERROR_OPENING_CONFIGURATION_FILE, "Error opening configuration file"}
};




#endif
