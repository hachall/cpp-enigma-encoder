# Enigma Engine
An enigma encryptor/decryptor. The engine encypts using polyalphabetic substitution with a new alphabet mapping for every character, by reconfiguring on each input. The plaintext is received via stdin and the ciphertext is outputted to stdout.

The enigma machine is defined by the plugboard mappings, reflector connections, the permutations of rotors used and their initial starting positions. It is this huge number of possible initial conditions that made the enigma machine so hard to crack!

Arguments:
- 1: Plugboard definition file
- 2: Reflector definition file
- 3 to (N-1): Rotor definition files
- N: Rotor starting positions

As long as the starting conditions are the same, the plaintext and ciphertext will map to each other and vice versa (the power of the reflector).

https://en.wikipedia.org/wiki/Enigma_machine
