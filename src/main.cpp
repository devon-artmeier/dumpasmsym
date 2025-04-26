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

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cout << "Usage: extasmsym -o [output] <-m [mode]> <-v [type]> <-b [base]> <-iy [symbol]> <-xy [symbol]>" << std::endl <<
		             "                 <-ip [prefix]> <-xp [prefix]> <-is [suffix]> <-xs [suffix]> [input files]" << std::endl << std::endl <<
		             "           -o [output]     - Output file" << std::endl <<
		             "           <-m [mode]>     - Output mode" << std::endl <<
		             "                             bin - Binary (default)" << std::endl <<
		             "                             asm - Assembly" << std::endl <<
		             "                             c   - C" << std::endl <<
		             "           <-v [type]>     - Value type (TEXT OUTPUT MODE ONLY)" << std::endl <<
		             "                             u32 - Unsigned 32-bit (default)" << std::endl <<
		             "                             u64 - Unsigned 64-bit" << std::endl <<
		             "                             s32 - Signed 32-bit" << std::endl <<
		             "                             s64 - Signed 64-bit" << std::endl <<
		             "           <-b [base]>     - Numerical system (TEXT OUTPUT MODE ONLY)" << std::endl <<
		             "                             hex - Hexadecimal (default)" << std::endl <<
		             "                             dec - Decimal" << std::endl <<
		             "                             bin - Binary" << std::endl <<
		             "           <-iy [symbol]>  - Only include symbol" << std::endl <<
		             "           <-xy [symbol]>  - Exclude symbol" << std::endl <<
		             "           <-ip [prefix]>  - Only include symbols with prefix" << std::endl <<
		             "           <-xp [prefix]>  - Exclude symbols with prefix" << std::endl <<
		             "           <-is [suffix]>  - Only include symbols with suffix" << std::endl <<
		             "           <-xs [suffix]>  - Exclude symbols with suffix" << std::endl <<
		             "           [input files]   - List of input files" << std::endl << std::endl <<
		             "Valid input file formats:" << std::endl << std::endl <<
		             "           Binary file generated from this tool" << std::endl <<
		             "           Psy-Q symbol file" << std::endl <<
		             "           vasm vobj file" << std::endl <<
		             "           vasm vlink symbol file (default format only)" << std::endl << std::endl;
		return -1;
	}

	Symbols                  symbols;
	std::vector<std::string> input_files;
	std::string              output_file = "";
	OutputMode               output_mode = OutputMode::Binary;
	ValueType                value_type  = ValueType::Unsigned32;
	NumberBase               number_base = NumberBase::Hex;

	try {
		for (int i = 1; i < argc; i++) {
			if (CheckArgument(argc, argv, i, "o")) {
				if (!output_file.empty()) {
					throw std::runtime_error("Output file already defined.");
				}

				output_file = argv[i];
				continue;
			}

			if (CheckArgument(argc, argv, i, "m")) {
				std::string mode = StringToLower(argv[i]);

				if (mode.compare("bin") == 0) {
					output_mode = OutputMode::Binary;
				} else if (mode.compare("asm") == 0) {
					output_mode = OutputMode::Asm;
				} else if (mode.compare("c") == 0) {
					output_mode = OutputMode::C;
				} else {
					throw std::runtime_error(("Invalid output mode \"" + (std::string)argv[i] + "\"").c_str());
				}

				continue;
			}

			if (CheckArgument(argc, argv, i, "v")) {
				std::string type = StringToLower(argv[i]);

				if (type.compare("u32") == 0) {
					value_type = ValueType::Unsigned32;
				} else if (type.compare("u64") == 0) {
					value_type = ValueType::Unsigned64;
				} else if (type.compare("s32") == 0) {
					value_type = ValueType::Signed32;
				} else if (type.compare("s64") == 0) {
					value_type = ValueType::Signed64;
				} else {
					throw std::runtime_error(("Invalid value type \"" + (std::string)argv[i] + "\"").c_str());
				}

				continue;
			}

			if (CheckArgument(argc, argv, i, "b")) {
				std::string type = StringToLower(argv[i]);

				if (type.compare("hex") == 0) {
					number_base = NumberBase::Hex;
				} else if (type.compare("dec") == 0) {
					number_base = NumberBase::Decimal;
				} else if (type.compare("bin") == 0) {
					number_base = NumberBase::Binary;
				} else {
					throw std::runtime_error(("Invalid numerical system \"" + (std::string)argv[i] + "\"").c_str());
				}

				continue;
			}

			if (CheckArgument(argc, argv, i, "iy")) {
				symbols.AddSymbolInclude(argv[i]);
				continue;
			}

			if (CheckArgument(argc, argv, i, "xy")) {
				symbols.AddSymbolExclude(argv[i]);
				continue;
			}

			if (CheckArgument(argc, argv, i, "ip")) {
				symbols.AddPrefixInclude(argv[i]);
				continue;
			}

			if (CheckArgument(argc, argv, i, "xp")) {
				symbols.AddPrefixExclude(argv[i]);
				continue;
			}

			if (CheckArgument(argc, argv, i, "is")) {
				symbols.AddSuffixInclude(argv[i]);
				continue;
			}

			if (CheckArgument(argc, argv, i, "xs")) {
				symbols.AddSuffixExclude(argv[i]);
				continue;
			}

			input_files.push_back(argv[i]);
		}

		if (input_files.empty()) {
			throw std::runtime_error("Input symbol files not defined.");
		}
		if (output_file.empty()) {
			throw std::runtime_error("Output symbol file not defined.");
		}

		for (const auto& input_file : input_files) {
			symbols.LoadSymbols(input_file);
		}
		symbols.GetOutputSymbols();
		symbols.Output(output_file, value_type, number_base, output_mode);
	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
