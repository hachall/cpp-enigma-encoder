#ifndef COMPONENTS_H
#define COMPONENTS_H
#include<fstream>
#include<string>
using namespace std;

/*
Component Class
  - validates configuration data and outputs ERROR_CODES if input streams
    are badly formed
  - uses the large overlap in state and behaviour to form plugboards, reflectors
    and rotors out of a generic Component class
  - component differentiates nuances between components on setup
  - FURTHER WORK: rotors have more functionality so could be seperated as a
    derived class using inheritance
*/
class Component {
private:
  /*UNIVERSAL ATTRIBUTES*/
  enum {Plugboard, Reflector, Rotor};
  int type;
  int alphabet_map[26] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};

  /*ROTOR SPECIFIC ATTRIBUTES*/
  int rotor_num;
  int* notches;
  int num_notches;
  int position;

public:
  /*UNIVERSAL CLASS FUNCTIONS*/

  /*Constructor
    creates a new Component of the correct type
    - type: component type in ["plugboard", "reflector", "rotor"]
    - rotor_num: rotor_num needed to set rotor in EnigmaMachine->rotors[]
      set to -1 for components that don't use it ("plugboard" & "reflector")
  */
  Component(string type, int rotor_num);

  /*Configure()
    validates the form of an input stream and passes data to setMappings() or
    setMappingsRotor() helper function
    checks all characters are integers, and that the data abides by the
    specifications of each component
    - in_stream: input file stream
    - returns integer ERROR_CODE, 0 if successful operation
  */
  int Configure(ifstream& in_stream);

  /*setMappings()
    for plugboard and reflector Components.
    simple pair mappings applied to this->alphabet_map
    - mappings: array of mapping information - pairs of integers need to be
      swapped
    - num_items: helper for 'for' loop - not necessarily 26
  */
  void setMappings(int mappings[], int num_items);

  /*checkNumeric()
    checks if a character is a number [0-9]
    - ch: character
    - returns integer ERROR_CODE, 0 if successful operation
  */
  int checkNumeric(char);

  /*convertReversible()
    takes in an input and maps it to an output using this->alphabet_map
    - input: input integer (0-25)
    - returns an output integer (0-25)
  */
  int convertReversible(int input);

  /*Rotor SPECIFIC CLASS FUNCTIONS*/

  /*setMappingsRotor()
    for rotor Components.
    index mappings applied to this->alphabet_map
    sets notches in this->notches (can be up to 26)
    - mappings: array of mapping information - mapping occurs between integers and
      the index it exists at in mappings
    - num_items: helper for num_notches and 'for' loop - can be more than 26
      entries in mappings past the index of 25 are notch positions
  */
  void setMappingsRotor(int[], int num_items);

  /*setPosition()
    sets starting position of rotor, stored at this->position
    position is required for calculations of input/output offsets
    - position: rotor starting position
  */
  void setPosition(int position);

  /*rotate()
    increment position up by 1
    modulus with 26 to ensure position is cyclical in range 0-25
    - returns bool result if next rotor also needs to rotate (as a rotor notch
      has reached the top position)
  */
  bool rotate();

  /*convertForwards() & convertBackwards()
    assymetrical mapping means that logic for mapping through a rotor is slightly
    different going through forwards and backwards but functionality is the same
    for both functions
    - input: input integer (0-25)
    - returns output integer (0-25)
  */
  int convertForwards(int input);
  int convertBackwards(int input);

  /*Destructor
    if rotor, deallocates notches array
  */
  ~Component();
};

#endif
