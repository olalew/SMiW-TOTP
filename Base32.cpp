#include "Base32.h"

Base32::Base32() {
	// TODO Auto-generated constructor stub

}

Base32::~Base32() {
	// TODO Auto-generated destructor stub
}

int Base32::charToInt(char c) {
	switch(c) {
		case 'A': return 0;
		case 'B': return 1;
		case 'C': return 2;
		case 'D': return 3;
		case 'E': return 4;
		case 'F': return 5;
		case 'G': return 6;
		case 'H': return 7;
		case 'I': return 8;
		case 'J': return 9;
		case 'K': return 10;
		case 'L': return 11;
		case 'M': return 12;
		case 'N': return 13;
		case 'O': return 14;
		case 'P': return 15;
		case 'Q': return 16;
		case 'R': return 17;
		case 'S': return 18;
		case 'T': return 19;
		case 'U': return 20;
		case 'V': return 21;
		case 'W': return 22;
		case 'X': return 23;
		case 'Y': return 24;
		case 'Z': return 25;
		case '2': return 26;
		case '3': return 27;
		case '4': return 28;
		case '5': return 29;
		case '6': return 30;
		case '7': return 31;
		default:
			return -1;
		}
}

ParsedKey Base32::fromBase32String(String str) {
	const int mask = 31;
	const int shift = 5;

  int buffer = 0;
	int next = 0;
	int bitsLeft = 0;

  ParsedKey parsedKey;
	parsedKey.keyLength = 32; // sstr.length() * shift / 8;

	for(int i = 0; i < str.length(); i++) {
		char parsedValue = charToInt(str[i]);

		if (parsedValue < 0) {
			parsedKey.status = KEY_PARSER_STATUS::UDEFINED_ERROR;
			return parsedKey;
		}

		buffer <<= shift;
		buffer |= parsedValue & mask;
		bitsLeft += shift;

		if (bitsLeft >= 8) {
      char value = (char)(buffer >> (bitsLeft - 8));

      Serial.println(str[i]);
      Serial.println((uint8_t)value);
      Serial.println();

			parsedKey.hmacKey[next++] = (char)(buffer >> (bitsLeft - 8));
			bitsLeft -= 8;
		}
	}

  Serial.println("PARSING DONE ...............");

	parsedKey.status = KEY_PARSER_STATUS::OK;
	return parsedKey;
}