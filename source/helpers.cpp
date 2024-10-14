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

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include "helpers.hpp"

std::string StringToLower(const std::string& str)
{
	std::string lower_str = str;
	std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), [](unsigned char c) { return std::tolower(c); });
	return lower_str;
}

bool CheckArgument(const int argc, char* argv[], int& index, const std::string& option, bool ignore_case)
{
	std::string option_copy = option;
	if (ignore_case) {
		option_copy = StringToLower(option);
	}

	if (strcmp(argv[index], ("-" + option_copy).c_str()) == 0) {
		if (++index >= argc) {
			throw std::runtime_error(("Missing parameter for \"-" + option + "\"").c_str());
		}
		return true;
	}
	return false;
}

void ReadInput(std::ifstream& input, char* const read_buffer, const std::streamsize read_count)
{
	if (read_buffer == nullptr) {
		return;
	}

	input.read(read_buffer, read_count);
	std::streamsize actual_read_count = input.gcount();

	if (actual_read_count == 0) {
		throw std::runtime_error("Reached end of file prematurely.");
	} else if (actual_read_count < 0) {
		throw std::runtime_error("Failed to read from file.");
	}
}

long long ReadInputNumberPsyQ(std::ifstream& input, bool is_signed)
{
	long long value = 0;
	char      read_buffer;

	for (int i = 0; i < 4; i++) {
		ReadInput(input, reinterpret_cast<char*>(&read_buffer), 1);
		value |= static_cast<long long>(read_buffer) << (i << 3);
	}

	if (is_signed && (value & (1LL << 31))) {
		value |= ~(static_cast<long long>(1LL << 32) - 1);
	}

	return value;
}

long long ReadInputNumberVObj(std::ifstream& input, bool is_signed)
{
	unsigned char read_buffer;
	unsigned char bytes;
	int           size;

	ReadInput(input, reinterpret_cast<char*>(&bytes), 1);
	if (bytes <= 0x7F) {
		return static_cast<long long>(bytes);
	}

	long long value = 0;
	if (bytes -= 0x80) {
		size = bytes * 8;

		int i = 0;
		while (bytes--) {
			ReadInput(input, reinterpret_cast<char*>(&read_buffer), 1);
			value |= static_cast<long long>(read_buffer) << (i++ * 8);
		}

		if (is_signed && (value & (1LL << (size - 1)))) {
			value |= ~(static_cast<long long>(1LL << (size)) - 1);
		}
	}

	return value;
}

std::string ReadInputStringPsyQ(std::ifstream& input)
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

std::string ReadInputStringVObj(std::ifstream& input)
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

bool StringStartsWith(const std::string& str, const std::string& prefix)
{
	return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

bool StringEndsWith(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}
