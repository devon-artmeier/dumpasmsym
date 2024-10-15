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

std::string StringToLower(const std::string& str)
{
	std::string lower_str = str;
	std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), [](unsigned char c) { return std::tolower(c); });
	return lower_str;
}

bool CheckArgument(const int argc, char* argv[], int& index, const std::string& option, const bool ignore_case)
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

bool StringStartsWith(const std::string& str, const std::string& prefix)
{
	return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

bool StringEndsWith(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void WriteOutputValue(std::ofstream& output, long long value, const std::string& hex_prefix, const std::string& bin_prefix,
                      const ValueType value_type, const NumberBase number_base)
{
	if (value_type == ValueType::Signed32 || value_type == ValueType::Signed64) {
		if (value < 0) {
			output << "-";
			value = -value;
		} else {
			output << " ";
		}
	}

	if (value_type == ValueType::Unsigned32 || value_type == ValueType::Signed32) {
		value &= 0xFFFFFFFF;
	}

	switch (number_base) {
		case NumberBase::Hex:
			output << hex_prefix << std::hex << std::uppercase << value;
			break;
		case NumberBase::Decimal:
			output << value;
			break;
		case NumberBase::Binary:
			output << bin_prefix;
			if (value_type == ValueType::Unsigned32 || value_type == ValueType::Signed32) {
				output << std::bitset<32>(value);
			} else {
				output << std::bitset<64>(value);
			}
			break;
	}
}
