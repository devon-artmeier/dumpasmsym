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

static void StoreNumber(std::ofstream& output, const long long number, const int bytes)
{
	char* write_buffer = new char[bytes];

	for (int i = 0; i < bytes; i++) {
		write_buffer[i] = (number >> (i * 8)) & 0xFF;
	}
	output.write(write_buffer, bytes);

	delete[] write_buffer;
}

static void StoreString(std::ofstream& output, const std::string& string)
{
	char size = string.size();

	output.write(&size, 1);
	output.write(string.c_str(), size);
}

void Symbols::OutputBinary(const std::string& file_name, const ValueType value_type, const NumberBase number_base)
{
	std::ofstream output(file_name, std::ios::out | std::ios::binary);
	if (!output.is_open()) {
		throw std::runtime_error(("Cannot open \"" + file_name + "\" for writing.").c_str());
	}

	char* signature = "BSYM";
	output.write(signature, 4);

	StoreNumber(output, this->symbols.size(), 4);

	for (auto& symbol : this->symbols) {

		StoreString(output, symbol.name);
		StoreNumber(output, symbol.value, 8);
	}
}
