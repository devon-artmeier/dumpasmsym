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
#include <bitset>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "helpers.hpp"
#include "symbols.hpp"

static bool CompareSymbols(const Symbol symbol_1, const Symbol symbol_2)
{
	return symbol_1.value < symbol_2.value;
}

void Symbols::LoadSymbols(const std::string& file_name)
{
	if (this->LoadPsyQSymbols(file_name)) {
		return;
	}
	if (this->LoadVObjSymbols(file_name)) {
		return;
	}

	throw std::runtime_error(("\"" + file_name + "\" is not a valid file.").c_str());
}

bool Symbols::LoadPsyQSymbols(const std::string& file_name)
{
	this->input_file_name = file_name;

	std::ifstream input(file_name, std::ios::in | std::ios::binary);
	if (!input.is_open()) {
		throw std::runtime_error(("Cannot open \"" + file_name + "\" for reading.").c_str());
	}

	char read_buffer[4];
	ReadInput(input, read_buffer, 3);
	read_buffer[3] = '\0';

	if (strcmp(read_buffer, "MND")) {
		return false;
	}

	ReadInput(input, read_buffer, 1);
	if (read_buffer[0] != 1) {
		return false;
	}

	input.seekg(8, std::ios_base::beg);

	while (true) {
		if (input.peek() == -1) {
			break;
		}

		long long value = ReadInputNumberPsyQ(input, true);

		char type;
		ReadInput(input, &type, 1);

		std::string name = ReadInputStringPsyQ(input);

		if (type == 1 || type == 2) {
			this->symbols.push_back({ name, value });
		}
	}

	return true;
}

bool Symbols::LoadVObjSymbols(const std::string& file_name)
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

	ReadInputNumberVObj(input, false);
	ReadInputNumberVObj(input, false);
	ReadInputStringVObj(input);
	ReadInputNumberVObj(input, false);

	int symbol_count = ReadInputNumberVObj(input, false);

	while (symbol_count-- > 0) {
		std::string name = ReadInputStringVObj(input);
		long long   type = ReadInputNumberVObj(input, false);

		ReadInputNumberVObj(input, false);
		ReadInputNumberVObj(input, false);

		long long value = ReadInputNumberVObj(input, true);

		ReadInputNumberVObj(input, false);

		if (type == 3) {
			this->symbols.push_back({ name, value });
		}
	}

	return true;
}

void Symbols::AddSymbolInclude(std::string symbol)
{
	this->symbol_includes.push_back(symbol);
}

void Symbols::AddPrefixInclude(std::string prefix)
{
	this->prefix_includes.push_back(prefix);
}

void Symbols::AddSuffixInclude(std::string suffix)
{
	this->suffix_includes.push_back(suffix);
}

void Symbols::AddSymbolExclude(std::string symbol)
{
	this->symbol_excludes.push_back(symbol);
}

void Symbols::AddPrefixExclude(std::string prefix)
{
	this->prefix_excludes.push_back(prefix);
}

void Symbols::AddSuffixExclude(std::string suffix)
{
	this->suffix_excludes.push_back(suffix);
}

void Symbols::Filter()
{
	for (int i = 0; i < this->symbols.size(); i++) {
		bool dont_filter = this->prefix_includes.empty() && this->suffix_includes.empty();

		for (const auto& prefix_include : prefix_includes) {
			if (StringStartsWith(this->symbols[i].name, prefix_include)) {
				dont_filter = true;
				break;
			}
		}

		for (const auto& suffix_include : suffix_includes) {
			if (StringEndsWith(this->symbols[i].name, suffix_include)) {
				dont_filter = true;
				break;
			}
		}

		for (const auto& prefix_exclude : prefix_excludes) {
			if (StringStartsWith(this->symbols[i].name, prefix_exclude)) {
				dont_filter = false;
				break;
			}
		}

		for (const auto& suffix_exclude : suffix_excludes) {
			if (StringEndsWith(this->symbols[i].name, suffix_exclude)) {
				dont_filter = false;
				break;
			}
		}

		for (const auto& symbol_include : symbol_includes) {
			if (this->symbols[i].name.compare(symbol_include) == 0) {
				dont_filter = true;
				break;
			}
		}

		for (const auto& symbol_exclude : symbol_excludes) {
			if (this->symbols[i].name.compare(symbol_exclude) == 0) {
				dont_filter = false;
				break;
			}
		}

		if (!dont_filter) {
			this->symbols.erase(this->symbols.begin() + i--);
		}
	}

	std::sort(this->symbols.begin(), this->symbols.end(), CompareSymbols);
}

void Symbols::Output(const std::string& file_name, ValueType value_type, NumberBase number_base)
{
	std::ofstream output(file_name, std::ios::out);
	if (!output.is_open()) {
		throw std::runtime_error(("Cannot open \"" + file_name + "\" for writing.").c_str());
	}

	int line_length = 0;
	for (const auto& symbol : this->symbols) {
		if (symbol.name.size() > line_length) {
			line_length = symbol.name.size();
		}
	}

	if ((line_length & 7) != 0) {
		line_length &= ~7;
	}
	line_length += 8;

	output << "; ------------------------------------------------------------------------------" << std::endl <<
	          "; Symbols extracted from" << std::endl << "; " << this->input_file_name << std::endl <<
	          "; ------------------------------------------------------------------------------" << std::endl << std::endl;

	for (auto& symbol : this->symbols) {
		output << std::left << std::setw(line_length) << symbol.name << "equ ";
		
		if (value_type == ValueType::Signed32 || value_type == ValueType::Signed64) {
			if (symbol.value < 0) {
				output << "-";
				symbol.value = -symbol.value;
			} else {
				output << " ";
			}
		}

		if (value_type == ValueType::Unsigned32 || value_type == ValueType::Signed32) {
			symbol.value &= 0xFFFFFFFF;
		}

		switch (number_base) {
			case NumberBase::Hex:
				output << "0x" << std::hex << std::uppercase << symbol.value << std::endl;
				break;
			case NumberBase::Decimal:
				output << symbol.value << std::endl;
				break;
			case NumberBase::Binary:
				output << "0b";
				if (value_type == ValueType::Unsigned32 || value_type == ValueType::Signed32) {
					output << std::bitset<32>(symbol.value);
				} else {
					output << std::bitset<64>(symbol.value);
				}
				output << std::endl;
				break;
		}
	}

	output << std::endl << "; ------------------------------------------------------------------------------" << std::endl;
}
