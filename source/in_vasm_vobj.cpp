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

static long long ReadInputNumber(std::ifstream& input, bool is_signed)
{
	unsigned char read_buffer[8] = { 0 };
	long long     value          = 0;
	unsigned char bytes;
	int           size;

	ReadInput(input, reinterpret_cast<char*>(&bytes), 1);
	if (bytes <= 0x7F) {
		return static_cast<long long>(bytes);
	}

	if (bytes -= 0x80) {
		if (bytes > 8) {
			throw std::runtime_error(("Too many bytes specified for number (" + std::to_string(bytes) + ")").c_str());
		}
		size = bytes * 8;

		ReadInput(input, reinterpret_cast<char*>(read_buffer), bytes);
		while (bytes--) {
			value = (value << 8) | read_buffer[bytes];
		}

		if (is_signed && (value & (1LL << (size - 1)))) {
			value |= ~(static_cast<long long>(1LL << (size)) - 1);
		}
	}

	return value;
}

static std::string ReadInputString(std::ifstream& input)
{
	unsigned char read_buffer;

	std::string string = "";
	while (true) {
		ReadInput(input, reinterpret_cast<char*>(&read_buffer), 1);
		if (read_buffer == 0) {
			break;
		}
		string += read_buffer;
	}

	return string;
}

bool Symbols::LoadVasmVobjSymbols(const std::string& file_name)
{
	this->input_file_name = file_name;

	std::ifstream input(file_name, std::ios::in | std::ios::binary);
	if (!input.is_open()) {
		throw std::runtime_error(("Cannot open \"" + file_name  + "\" for reading.").c_str());
	}

	char read_buffer[5];
	ReadInput(input, read_buffer, 4);
	read_buffer[4] = '\0';

	if (strcmp(read_buffer, "VOBJ")) {
		return false;
	}
	input.seekg(1, std::ios_base::cur);

	ReadInputNumber(input, false);
	ReadInputNumber(input, false);
	ReadInputString(input);
	ReadInputNumber(input, false);

	int symbol_count = ReadInputNumber(input, false);

	while (symbol_count-- > 0) {
		std::string name = ReadInputString(input);
		long long   type = ReadInputNumber(input, false);

		ReadInputNumber(input, false);
		ReadInputNumber(input, false);

		long long value = ReadInputNumber(input, true);

		ReadInputNumber(input, false);

		if (type == 3) {
			this->symbols.push_back({ name, value });
		}
	}

	return true;
}
