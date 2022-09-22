#pragma once

#include "openglib/Window.h"

#include "Letters.h"

#include <algorithm>
#include <array>
#include <map>

#define ROTOR_1 0
#define ROTOR_2 1
#define ROTOR_3 2
#define ROTOR_4 3
#define ROTOR_5 4

struct Key
{
private:
	static std::array<int, 26> s_Keys;
	static std::map<int, std::array<float, 2>> s_RowOffsets;

public:
	glm::vec2 pos = { -95.0f, -95.0f };
	int keycode = NULL;

	Key(int keycode);
};

struct Rotor
{
private:
	std::array<Letter, 26> m_Settings;
	Letter m_Position;
	Letter m_Notch;

public:
	Rotor() {}
	Rotor(std::array<Letter, 26> settings, Letter initialPosition, Letter notch)
		: m_Settings(settings), m_Position(initialPosition), m_Notch(notch) {}

	bool Rotate();
	Letter Substitute(Letter letter);
	Letter ReverseSubstitute(Letter letter);

	void SetPosition(Letter position);
};

struct Reflector
{
private:
	std::array<Letter, 26> m_Settings;

public:
	Reflector() {}
	Reflector(std::array<Letter, 26> settings)
		: m_Settings(settings) {}

	Letter Substitute(Letter letter);
};

struct Plugboard
{

};

class Enigma
{
private:
	static std::array<Rotor, 5> s_Rotors;
	static Reflector s_Reflector;

	Rotor m_FirstRotor, m_SecondRotor, m_ThirdRotor;

public:
	void SetSettings(int rotor1Index, int rotor2Index, int rotor3Index, Letter rotor1Positon, Letter rotor2Position, Letter rotor3Position);
	Letter SubstituteLetter(Letter letter);
};