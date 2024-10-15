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

static bool CompareSymbols(const Symbol symbol_1, const Symbol symbol_2)
{
	return symbol_1.value < symbol_2.value;
}

void Symbols::LoadSymbols(const std::string& file_name)
{
	if (this->LoadBinarySymbols(file_name)) {
		return;
	}
	if (this->LoadPsyqSymbols(file_name)) {
		return;
	}
	if (this->LoadVasmVobjSymbols(file_name)) {
		return;
	}

	throw std::runtime_error(("\"" + file_name + "\" is not a valid file.").c_str());
}

void Symbols::AddSymbolInclude(const std::string& symbol)
{
	this->symbol_includes.push_back(symbol);
}

void Symbols::AddPrefixInclude(const std::string& prefix)
{
	this->prefix_includes.push_back(prefix);
}

void Symbols::AddSuffixInclude(const std::string& suffix)
{
	this->suffix_includes.push_back(suffix);
}

void Symbols::AddSymbolExclude(const std::string& symbol)
{
	this->symbol_excludes.push_back(symbol);
}

void Symbols::AddPrefixExclude(const std::string& prefix)
{
	this->prefix_excludes.push_back(prefix);
}

void Symbols::AddSuffixExclude(const std::string& suffix)
{
	this->suffix_excludes.push_back(suffix);
}

void Symbols::Filter()
{
	for (int i = 0; i < this->symbols.size(); i++) {
		bool dont_filter = this->symbol_includes.empty() && this->prefix_includes.empty() && this->suffix_includes.empty();

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

void Symbols::Output(const std::string& file_name, const ValueType value_type, const NumberBase number_base, const OutputMode output_mode)
{
	switch (output_mode) {
		case OutputMode::Binary:
			this->OutputBinary(file_name, value_type, number_base);
			break;
		case OutputMode::Asm:
			this->OutputAsm(file_name, value_type, number_base);
			break;
		case OutputMode::C:
			this->OutputC(file_name, value_type, number_base);
			break;
	}
}

int Symbols::GetLineLength()
{
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

	return line_length;
}
