#include "Enigma.h"

std::array<Rotor, 5> Enigma::s_Rotors = {
	Rotor({ 4, 10, 12, 5, 11, 6, 3, 16, 21, 25, 13, 19, 14, 22, 24, 7, 23, 20, 18, 15, 0, 8, 1, 17, 2, 9 }, 0, Q),
	Rotor({ 0, 9, 3, 10, 18, 8, 17, 20, 23, 1, 11, 7, 22, 19, 12, 2, 16, 6, 25, 13, 15, 24, 5, 21, 14, 4 }, 0, E),
	Rotor({ 1, 3, 5, 7, 9, 11, 2, 15, 17, 19, 23, 21, 25, 13, 24, 4, 8, 22, 6, 0, 10, 12, 20, 18, 16, 14 }, 0, V),
	Rotor({ 4, 18, 14, 21, 15, 25, 9, 0, 24, 16, 20, 8, 17, 7, 23, 11, 13, 5, 19, 6, 10, 3, 2, 12, 22, 1 }, 0, J),
	Rotor({ 21, 25, 1, 17, 6, 8, 19, 24, 20, 15, 18, 3, 13, 7, 11, 23, 0, 22, 12, 9, 16, 14, 5, 4, 2, 10 }, 0, Z)
};

Reflector Enigma::s_Reflector = Reflector({ 24, 17, 20, 7, 16, 18, 11, 3, 15, 23, 13, 6, 14, 10, 12, 8, 4, 1, 5, 25, 2, 22, 21, 9, 0, 19 });

Key::Key(int keycode)
	: keycode(keycode)
{
	auto it = std::find(s_Keys.begin(), s_Keys.end(), keycode);
	if (it != s_Keys.end())
	{
		int location = it - s_Keys.begin();
		int offset = location % 19 % 10;

		std::array<float, 2> rowOffset = s_RowOffsets[location - offset];

		pos = { rowOffset[0] + 105.0f * offset, rowOffset[1] };
	}
}

bool Rotor::Rotate()
{
	m_Position = (m_Position + 1) % 26;

	return m_Position == m_Notch;
}

Letter Rotor::Substitute(Letter letter)
{
	return m_Settings[(m_Position + letter) % 26];
}

Letter Rotor::ReverseSubstitute(Letter letter)
{
	auto it = std::find(m_Settings.begin(), m_Settings.end(), letter);
	if (it != m_Settings.end())
	{
		Letter l = -m_Position + (int)(it - m_Settings.begin());
		if (l < 0)
		{
			return 26 + l;
		}

		return l;
	}

	return -1;
}

void Rotor::SetPosition(Letter position)
{
	m_Position = position;
}

Letter Reflector::Substitute(Letter letter)
{
	return m_Settings[letter];
}

void Enigma::SetSettings(int rotor1Index, int rotor2Index, int rotor3Index, Letter rotor1Position, Letter rotor2Position, Letter rotor3Position)
{
	m_FirstRotor = s_Rotors[rotor1Index];
	m_SecondRotor = s_Rotors[rotor2Index];
	m_ThirdRotor = s_Rotors[rotor3Index];

	m_FirstRotor.SetPosition(rotor1Position);
	m_SecondRotor.SetPosition(rotor2Position);
	m_ThirdRotor.SetPosition(rotor3Position);
}

Letter Enigma::SubstituteLetter(Letter letter)
{
	// rotate
	if (m_FirstRotor.Rotate())
	{
		if (m_SecondRotor.Rotate())
		{
			m_ThirdRotor.Rotate();
		}
	}

	// forward pass
	Letter temp = m_ThirdRotor.Substitute(m_SecondRotor.Substitute(m_FirstRotor.Substitute(letter)));

	// reflect
	temp = s_Reflector.Substitute(temp);

	// backward pass
	temp = m_FirstRotor.ReverseSubstitute(m_SecondRotor.ReverseSubstitute(m_ThirdRotor.ReverseSubstitute(temp)));

	return temp;
}


















std::array<int, 26> Key::s_Keys =
{
	KEY(Q), KEY(W), KEY(E), KEY(R), KEY(T), KEY(Y), KEY(U), KEY(I), KEY(O), KEY(P),
	KEY(A), KEY(S), KEY(D), KEY(F), KEY(G), KEY(H), KEY(J), KEY(K), KEY(L),
	KEY(Z), KEY(X), KEY(C), KEY(V), KEY(B), KEY(N), KEY(M)
};

std::map<int, std::array<float, 2>> Key::s_RowOffsets =
{
	{ 0, { 10.0f, 220.0f } },
	{ 10, { 60.0f, 115.0f } },
	{ 19, { 160.0f, 10.0f } }
};