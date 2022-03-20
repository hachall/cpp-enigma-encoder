#include"errors.h"
#include"components.h"
#include"machine.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*ERROR HANDLING*/
static void print_error(int error_code) {
  std::cerr << "ERROR: " << errors.at(error_code) << std::endl;
}

/*
MAIN FUNCTION
- handles external file manipulation, and passes successfully loaded input streams
to EnigmaMachine class to configure
- handles error codes, and error logging
- runs reversible encryption on standard input stream
*/
int main(int argc, char*argv[]) {

  /*insufficient input arguments*/
  if (argc < 4) {
    print_error(INSUFFICIENT_NUMBER_OF_PARAMETERS);
    return(INSUFFICIENT_NUMBER_OF_PARAMETERS);
  }

  /*initialise enigma machine*/
  EnigmaMachine* enigma = new EnigmaMachine();
  int num_rotors = argc - 4;
  enigma->setNumRotors(num_rotors);

  /*loop on command line input files to 'set up' machine components
  if a file cannot be opened it is not passed to EnigmaMachine and the program exits*/
  for (int i = 1 ; i < argc ; i++) {
    int config_status;
    ifstream in_stream;
    in_stream.open(argv[i]);
    //attempt to open file
    if (in_stream.fail()) {
      print_error(ERROR_OPENING_CONFIGURATION_FILE);
      return(ERROR_OPENING_CONFIGURATION_FILE);
    }

    if (i == 1) {
      config_status = enigma->setupComponent(in_stream, "plugboard");
    } else if (i == 2) {
      config_status = enigma->setupComponent(in_stream, "reflector");
    } else if (i == (argc - 1)) {
      config_status = enigma->configureStartingPositions(in_stream);
    } else {
      config_status = enigma->setupComponent(in_stream, "rotor", i-3);
    }

    in_stream.close();
    if (config_status > 0) {
      print_error(config_status);
      return (config_status);
    }

  }

  /*encryption functionality, applied to standard input*/
  char input;
  cin >> input;
  while (!cin.eof()) {
    int input_status = enigma->EncryptDecrypt(input);
    if (input_status > 0) {
      print_error(input_status);
      return(input_status);
    }
    cin >> input;
  }

  delete enigma;
  return NO_ERROR;
}
