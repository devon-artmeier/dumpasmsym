/*
	Copyright (c) 2025 Devon Artmeier

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

void Symbols::OutputC(const std::string& file_name, ValueType value_type, NumberBase number_base)
{
	std::ofstream output(file_name, std::ios::out);
	if (!output.is_open()) {
		throw std::runtime_error(("Cannot open \"" + file_name + "\" for writing.").c_str());
	}

	if (input_file_names.empty()) {
		output << "// ------------------------------------------------------------------------------" << std::endl <<
		          "// No valid symbol files found" << std::endl;
		output << "// ------------------------------------------------------------------------------";
	} else {
		int line_length = GetLineLength();

		output << "// ------------------------------------------------------------------------------" << std::endl <<
				  "// Symbols extracted from" << std::endl;
		for (const auto& input_file_name : input_file_names) {
			output << "// " << input_file_name << std::endl;
		}
		output << "// ------------------------------------------------------------------------------" << std::endl << std::endl;

		for (auto& symbol : this->symbols_out) {
			output << "#define " << std::left << std::setw(line_length) << symbol.name << " (";
			WriteOutputValue(output, symbol.value, "0x", "0b", value_type, number_base);
			if (!this->value_offset.empty()) {
				output << "+" << this->value_offset;
			}
			output << ")" << std::endl;
		}

		output << std::endl << "// ------------------------------------------------------------------------------";
	}
}
