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

bool Symbols::LoadVasmLstSymbols(const std::string& file_name)
{
	std::ifstream input(file_name, std::ios::in);
	if (!input.is_open()) {
		throw std::runtime_error(("Cannot open \"" + file_name + "\" for reading.").c_str());
	}

	std::string line;
	std::getline(input, line);

	if (line.compare("Sections:") != 0) {
		return false;
	}

	bool found = false;
	while (std::getline(input, line)) {
		if (line.compare("Symbols by value:") == 0) {
			found = true;
			break;
		}
	}
	if (!found) {
		return false;
	}

	while (std::getline(input, line)) {
		if (!line.empty()) {
			size_t space = line.find(' ');
			if (space == std::string::npos) {
				return false;
			}

			std::string value_str = line.substr(0, space);
			std::string name      = line.substr(space + 1);
			if (value_str.empty() || name.empty()) {
				return false;
			}

			long long value = 0;
			try {
				value = std::stoull(value_str, nullptr, 16);
			} catch (...) {
				return false;
			}

			this->AddSymbol(name, value);
		}
	}

	return true;
}
