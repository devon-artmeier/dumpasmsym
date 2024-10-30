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

class Symbols
{
public:
	void LoadSymbols     (const std::string& file_name);
	void AddSymbolInclude(const std::string& symbol);
	void AddPrefixInclude(const std::string& prefix);
	void AddSuffixInclude(const std::string& suffix);
	void AddSymbolExclude(const std::string& symbol);
	void AddPrefixExclude(const std::string& prefix);
	void AddSuffixExclude(const std::string& suffix);
	void GetOutputSymbols();
	void Output          (const std::string& file_name, const ValueType value_type, const NumberBase number_base, const OutputMode output_mode);

private:
	void AddSymbol          (std::string name, long long value);
	int  GetLineLength      ();
	bool LoadBinarySymbols  (const std::string& file_name);
	bool LoadPsyqSymbols    (const std::string& file_name);
	bool LoadVasmVobjSymbols(const std::string& file_name);
	bool LoadVlinkSymSymbols(const std::string& file_name);
	void OutputBinary       (const std::string& file_name, const ValueType value_type, const NumberBase number_base);
	void OutputAsm          (const std::string& file_name, const ValueType value_type, const NumberBase number_base);
	void OutputC            (const std::string& file_name, const ValueType value_type, const NumberBase number_base);
	
	std::vector<std::string>                   input_file_names;
	std::unordered_map<std::string, long long> symbols;
	std::vector<Symbol>                        symbols_out;
	std::vector<std::string>                   symbol_includes;
	std::vector<std::string>                   prefix_includes;
	std::vector<std::string>                   suffix_includes;
	std::vector<std::string>                   symbol_excludes;
	std::vector<std::string>                   prefix_excludes;
	std::vector<std::string>                   suffix_excludes;
};

#endif // SYMBOLS_HPP
