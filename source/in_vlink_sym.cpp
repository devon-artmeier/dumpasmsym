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

bool Symbols::LoadVlinkSymSymbols(const std::string& file_name)
{
	std::ifstream input(file_name, std::ios::in | std::ios::binary);
	if (!input.is_open()) {
		throw std::runtime_error(("Cannot open \"" + file_name + "\" for reading.").c_str());
	}

	std::string line;
	while (std::getline(input, line)) {
		if (!line.empty()) {
			size_t colon = line.find(':');
			if (colon == std::string::npos) {
				return false;
			}

			std::string value_str = line.substr(0, colon);
			std::string name      = line.substr(colon + 1);
			if (value_str.empty() || name.empty()) {
				return false;
			}

			long long value = 0;
			try {
				if (value_str.find("0x") != std::string::npos) {
					value = std::stoull(value_str, nullptr, 16);
				} else if (value_str.find("0b") != std::string::npos) {
					value = std::stoull(value_str, nullptr, 2);
				} else {
					value = std::stoull(value_str, nullptr, 10);
				}
			} catch (...) {
				return false;
			}

			this->AddSymbol(name, value);
		}
	}

	return true;
}
