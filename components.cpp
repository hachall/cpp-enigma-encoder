#include"errors.h"
#include"components.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <algorithm>
using namespace std;

Component::Component(string type, int rotor_num) {
  if (type == "plugboard") {
    this->type = Component::Plugboard;
  } else if (type == "reflector") {
    this->type = Component::Reflector;
  } else {
    this->type = Component::Rotor;
  };
  this->rotor_num = rotor_num;
}

int Component::Configure(ifstream& in_stream) {
  //a component needs up to 26 postion mappings, and in the case of rotors,
  //up to 26 notches
  int mappings[52]{};
  fill_n(mappings, 52, -1);
  int number;
  int i = 0;

  while(true) {
    //Check to see if next block of characters (until next whitespace) are all numeric
    char ch;
    ifstream::pos_type cur_position = in_stream.tellg();
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

    switch (this->type) {
      case Component::Plugboard:
        // configuration for plugboard
        if (i == 26) {
          return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
        }
        //if a number has been seen already - something is being mapped to multiple outputs or itself
        for (int j = 0 ; j <= i; j++) {
          if (mappings[j] == number) {
            return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
          }
        }
        break;
      case Component::Reflector:
        // configuration for reflector
        if (i == 26) {
          return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
        }
        //if a number has been seen already - something is being mapped to multiple outputs or itself
        for (int j = 0 ; j <= i; j++) {
          if (mappings[j] == number) {
            return INVALID_REFLECTOR_MAPPING;
          }
        }
        break;
      case Component::Rotor:
        //configuration for rotors
        //if a number has been seen already - something is being mapped to multiple outputs or itself
        if (i < 26) {
          for (int j = 0 ; j <= 25; j++) {
            if (mappings[j] == number) {
              return INVALID_ROTOR_MAPPING;
            }
          }
        }
        break;
    }

    //store the mapping number
    mappings[i] = number;
    i++;
  }

  switch (this->type) {
    case Component::Plugboard:
      if ( i % 2 == 1) {
        return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
      }
      setMappings(mappings, i);
      break;
    case Component::Reflector:
      if ( i != 26) {
        return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
      }
      setMappings(mappings, i);
      break;
    case Component::Rotor:
      if ( i < 26) {
        return INVALID_ROTOR_MAPPING;
      }
      setMappingsRotor(mappings, i);
  }

  return NO_ERROR;
}

int Component::checkNumeric(char ch) {
  int test_int = ch;
  if (test_int < 48 || test_int > 57) {
    return NON_NUMERIC_CHARACTER;
  } else {
    return NO_ERROR;
  }
}

void Component::setMappings(int mappings[], int num_items) {
  for (int i = 0 ; i < num_items - 1 ; i = i + 2) {
    this->alphabet_map[mappings[i]] = mappings[i+1];
    this->alphabet_map[mappings[i+1]] = mappings[i];
  }
};

void Component::setMappingsRotor(int mappings[], int num_items) {
  int* notches = new int[num_items - 26];

  for (int i = 0 ; i <= 25 ; i++) {
    this->alphabet_map[i] = mappings[i];
  }

  for (int i = 26 ; i < num_items ; i++) {
    notches[i - 26] = mappings[i];
  }
  
  this->notches = notches;
  this->num_notches = num_items - 26;
}

void Component::setPosition(int position) {
  this->position = position;
}

bool Component::rotate() {
  //update position
  this->position++;
  this->position = this->position % 26;
  //indicate if rotation needs to be propogated to next rotor
  for (int i = 0 ; i < this->num_notches ; i++) {
    if (this->notches[i] == this->position) {
      return true;
    }
  }
  return false;
}

int Component::convertReversible(int input) {
  return this->alphabet_map[input];
}

int Component::convertForwards(int input) {
  int offset_input = input + this->position;
  offset_input = offset_input % 26;

  int rotor_output = this->alphabet_map[offset_input];
  int output = rotor_output - position;

  if (output < 0) {
    output += 26;
  }

  return output;
};

int Component::convertBackwards(int input) {
  int offset_input = input + this->position;
  offset_input = offset_input % 26;

  int i = 0;
  for ( ; i < 26 ; i++) {
    if (this->alphabet_map[i] == offset_input) {
      break;
    }
  }

  int output = i - this->position;
  if (output < 0) {
    output += 26;
  }

  return output;
};

Component::~Component() {
  if (this->type == Component::Rotor) {
    delete [] this->notches;
  }
}
