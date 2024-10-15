/*
	Copyright(c) 2024 Devon Artmeier

	Permission to use, copy, modify, and /or distribute this software
	for any purpose with or without fee is hereby granted.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
	WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIE
	WARRANTIES OF MERCHANTABILITY AND FITNESS.IN NO EVENT SHALL THE
	AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
	DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
	PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
	TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
	PERFORMANCE OF THIS SOFTWARE.
*/

#include "shared.hpp"

static long long ReadInputNumber(std::ifstream& input, int bytes)
{
	unsigned char read_buffer[8] = { 0 };
	long long     value          = 0;

	ReadInput(input, reinterpret_cast<char*>(read_buffer), bytes);
	while (bytes--) {
		value = (value << 8) | read_buffer[bytes];
	}

	return value;
}

static std::string ReadInputString(std::ifstream& input)
{
	unsigned char char_count;
	ReadInput(input, reinterpret_cast<char*>(&char_count), 1);

	char* string_buffer = new char[char_count + 1];
	ReadInput(input, string_buffer, char_count);
	string_buffer[char_count] = '\0';

	std::string string = string_buffer;
	delete[] string_buffer;

	return string;
}

bool Symbols::LoadBinarySymbols(const std::string& file_name)
{
	std::ifstream input(file_name, std::ios::in | std::ios::binary);
	if (!input.is_open()) {
		throw std::runtime_error(("Cannot open \"" + file_name + "\" for reading.").c_str());
	}

	char read_buffer[5];
	ReadInput(input, read_buffer, 4);
	read_buffer[4] = '\0';

	if (strcmp(read_buffer, "BSYM")) {
		return false;
	}

	long long symbol_count = ReadInputNumber(input, 4);
	std::cout << symbol_count << std::endl;

	while (symbol_count--) {
		std::string name = ReadInputString(input);
		std::cout << name << std::endl;
		long long value  = ReadInputNumber(input, 8);
		std::cout << value << std::endl;
		this->symbols.push_back({ name, value });
	}

	return true;
}
