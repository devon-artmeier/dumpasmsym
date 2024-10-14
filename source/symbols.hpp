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

#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP

struct Symbol
{
	std::string name;
	long long   value;
};

enum class ValueType
{
	Unsigned32,
	Unsigned64,
	Signed32,
	Signed64,
};

enum class NumberBase
{
	Hex,
	Decimal,
	Binary
};

class Symbols
{
public:
	void LoadSymbols     (const std::string& file_name);
	void AddSymbolInclude(std::string symbol);
	void AddPrefixInclude(std::string prefix);
	void AddSuffixInclude(std::string suffix);
	void AddSymbolExclude(std::string symbol);
	void AddPrefixExclude(std::string prefix);
	void AddSuffixExclude(std::string suffix);
	void Filter          ();
	void Output          (const std::string& file_name, ValueType value_type, NumberBase number_base);

private:
	bool LoadPsyqSymbols    (const std::string& file_name);
	bool LoadVasmVobjSymbols(const std::string& file_name);

	std::string              input_file_name;
	std::vector<Symbol>      symbols;
	std::vector<std::string> symbol_includes;
	std::vector<std::string> prefix_includes;
	std::vector<std::string> suffix_includes;
	std::vector<std::string> symbol_excludes;
	std::vector<std::string> prefix_excludes;
	std::vector<std::string> suffix_excludes;
};

#endif // VOBJ_HPP
