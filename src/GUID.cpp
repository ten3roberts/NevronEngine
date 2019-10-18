#include <src/pch.h>
#include "GUID.h"

GUID::GUID()
{
}

void GUID::Generate()
{
	std::bitset<128> bits;

	for (int i = 0; i < 128; i++)
	{
		//Generating random bits
		bits[i] = rand() % 2;
		
	}

	//Sets the four most significant bits of the 7th byte to 0b0100
	bits[6*8+0] = 0;
	bits[6*8+1] = 1;
	bits[6*8+2] = 0;
	bits[6*8+3] = 0;

	//Sets the two most significant bits of the 9th byte to 0b10
	bits[8*8+0] = 1;
	bits[8 * 8 + 1] = 0;

	//Convert to hexadecimal
	static const char* digits = "0123456789ABCDEF";
	for (unsigned int i = 0; i < 128; i += 4); //Loops over eache nibble

	m_value.resize(32);

	for (unsigned int i = 0, j = 0; i < 128; i+=4, j++)
	{
		char mask = 1;
		char nibble = 0;
		for (int j = i+3; j >= (signed int)i; j--) {
			if (bits.test(j))
				nibble |= mask;
			mask <<= 1;
		}
		m_value[i/4] = digits[nibble];
	}

	//Add hyphens to separate it into blocks
	m_value.insert(m_value.begin() + 8, '-');
	m_value.insert(m_value.begin() + 13, '-');
	m_value.insert(m_value.begin() + 18, '-');
	m_value.insert(m_value.begin() + 23, '-');
}
