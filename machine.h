#ifndef MACHINE_H
#define MACHINE_H
#include"components.h"
#include<string>

/*
EnigmaMachine Class
- has ownership of Component class members
- handles creation and configuration of Component instances
- deletes Component instances in destructor
- handles encryption functionality of an input character
*/
class EnigmaMachine {
private:
  /*
  Sub Components:
  - initialized in setupComponent()
  - plugboard & reflector required
  - rotors array can be empty
  */
  Component* plugboard;
  Component* reflector;
  Component** rotors;

  int num_rotors = 0;

public:
  /*Default constructor*/
  EnigmaMachine();

  /*setNumRotors()
  sets num_rotors class member
  */
  void setNumRotors(int num_rotors);

  /*setupComponent()
    turns an input file stream into a Component class instance
    - in_stream: input file stream with configuration data for component
    - type: component type in ["plugboard", "reflector", "rotor"]
      differentiates setup type and assignment to EnigmaMachine attribute
    - rotor_num: number of rotors. Defaulted to -1, as not used for plugboard
      or reflector
    - returns integer ERROR_CODE, 0 if successful operation
  */
  int setupComponent(ifstream& in_stream, string type, int rotor_num = -1);

  /*configureStartingPositions()
    receives an input stream of rotor starting positions and assesses the
    viability of the data
    - in_stream: input file stream with starting position data
    - returns integer ERROR_CODE, 0 if successful operation
  */
  int configureStartingPositions(ifstream& in_stream);

  /*setStartingPositions()
    receives an array of approved integer starting positions and applies to
    this->rotors[]
    - positions: array of starting positions
  */
  void setStartingPositions(int positions[]);

  /*checkNumeric()
    checks if a character is a number [0-9]
    - ch: character
    - returns integer ERROR_CODE, 0 if successful operation
  */
  int checkNumeric(char ch);

  /*rotateSystem()
    controls the rotation of the system of rotors in this->rotors[]
    the "right-most" rotor is turned one place using component->rotate(),
    if any rotor notch reaches the top position the 'next' rotor is notified
    to rotate also
  */
  void rotateSystem();

  /*EncryptDecrypt()
    verifies viability of an input letter and passes input as an integer to
    reversibleEncryption() helper function
    prints output to standard output
    - letter: input character
    - returns integer ERROR_CODE, 0 if successful operation
  */
  int EncryptDecrypt(const char letter);

  /*reversibleEncryption()
    takes in an input (integer) and runs it through encryption process
    input goes through the plugboard, the rotors forwards, the reflector, back
    through the rotors backwards and through the plugboard again
    - input: input inetger to be mapped (encrypted) to output by the system
    - returns encrypted character mapping
  */
  char reversibleEncryption(int input);

  /*Destructor
    deallocates all system sub-components
  */
  ~EnigmaMachine();
};

#endif
