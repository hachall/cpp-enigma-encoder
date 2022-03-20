#include"errors.h"
#include"machine.h"
// #include"components.h"
#include<iostream>
using namespace std;

EnigmaMachine::EnigmaMachine() {
};

int EnigmaMachine::setupComponent(ifstream& in_stream, string type, int rotor_num ) {
  Component* new_comp = new Component(type, rotor_num);
  int config_status = new_comp->Configure(in_stream);
  if (config_status > 0) {
    return config_status;
  }

  if (type == "plugboard") {
    this->plugboard = new_comp;
  } else if (type == "reflector") {
    this->reflector = new_comp;
  } else {
    this->rotors[rotor_num] = new_comp;
  }

  return NO_ERROR;
}

void EnigmaMachine::setNumRotors(int num_rotors) {
  this->num_rotors = num_rotors;
  this->rotors = new Component* [num_rotors + 1] {};
}

int EnigmaMachine::configureStartingPositions(ifstream& in_stream) {
  int positions[this->num_rotors]{};
  int number;
  int i = 0;

  while(true) {
    //Check to see if next block of characters (until next whitespace) are all numeric
    char ch;
    int cur_position = in_stream.tellg();
    in_stream >> ws;
    in_stream.get(ch);
    if (in_stream.eof()) break;
    while (!isspace(ch)) {
      int numeric_status = checkNumeric(ch);
      if (numeric_status > 0) {
        return(numeric_status);
      }
      in_stream.get(ch);
    }
    in_stream.seekg(cur_position);

    //extract number from file
    in_stream >> number;
    if (in_stream.eof()) break;

    //check numbers between range of 0 - 25 (to index alphabet)
    if (number < 0 || number > 25) {
      return INVALID_INDEX;
    }

    if (i > num_rotors - 1 ) {
      break;
    }

    positions[i] = number;
    i++;
  }

  if (i < this->num_rotors) {
    return NO_ROTOR_STARTING_POSITION;
  }

  setStartingPositions(positions);
  return NO_ERROR;
}

int EnigmaMachine::checkNumeric(char ch) {
  int test_int = ch;
  if (test_int < 48 || test_int > 57) {
    return NON_NUMERIC_CHARACTER;
  } else {
    return NO_ERROR;
  }
}

void EnigmaMachine::setStartingPositions(int positions[]) {
  for (int i = 0 ; i < this->num_rotors ; i++) {
    this->rotors[i]->setPosition(positions[i]);
  }
};

void EnigmaMachine::rotateSystem() {
  //rotors rotated from last to first
  //Component->rotate() returns true if next rotor needs to rotate too
  //so loop 'continues', else 'breaks' - hence propogating rotation across system
  for (int i = this->num_rotors - 1 ; i >= 0 ; i--) {
    if (this->rotors[i]->rotate()) {
      continue;
    } else {
      break;
    }
  }
}

int EnigmaMachine::EncryptDecrypt(const char letter) {
  int num_equiv = letter;
  if (num_equiv < 65 || num_equiv > 90) {
    return INVALID_INPUT_CHARACTER;
  }
  //map ASCII letter to 0-25 representation
  num_equiv -= 65;

  char enc_character = reversibleEncryption(num_equiv);
  cout << enc_character;

  return NO_ERROR;
}

char EnigmaMachine::reversibleEncryption(int input) {

  this->rotateSystem();

  int encrypted = this->plugboard->convertReversible(input);

  for (int i = num_rotors - 1 ; i >= 0 ; i-- ) {
    encrypted = this->rotors[i]->convertForwards(encrypted);
  }

  encrypted = this->reflector->convertReversible(encrypted);

  for (int i = 0 ; i < num_rotors ; i++ ) {
    encrypted = this->rotors[i]->convertBackwards(encrypted);
  }

  encrypted = this->plugboard->convertReversible(encrypted);

  //map back to ASCII representation for character
  encrypted += 65;
  char output = encrypted;
  return output;
}

EnigmaMachine::~EnigmaMachine() {
  for (int i = 0; i < this->num_rotors ; i++) {
    delete this->rotors[i];
  }
  delete [] this->rotors;
  delete this->plugboard;
  delete this->reflector;

}
