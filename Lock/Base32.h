#ifndef SERVICES_BASE32_H_
#define SERVICES_BASE32_H_

#include <Arduino.h>

enum class KEY_PARSER_STATUS {
	OK = 0,
	UDEFINED_ERROR = 1
};

struct ParsedKey {
	uint8_t hmacKey[32] = { 0 };
	int keyLength;

	KEY_PARSER_STATUS status;
};

class Base32 {
private:

public:
	Base32();
	virtual ~Base32();

	int charToInt(char c);
	ParsedKey fromBase32String(String str);
};

#endif /* SERVICES_BASE32_H_ */