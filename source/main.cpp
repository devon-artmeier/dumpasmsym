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

#include "helpers.hpp"
#include "symbols.hpp"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cout << "Usage: extract-symbols -i [input] -o [output] <-v [type]> <-b [base]> <-f [symbol]> <-x [symbol]> " << std::endl <<
		             "                       <-p [prefix]> <-xp [prefix]> <-s [suffix]> <-xs [suffix]>" << std::endl << std::endl <<
		             "           -i [input]     - Input file" << std::endl <<
		             "           -o [output]    - Output file" << std::endl <<
		             "           <-v [type]>    - Value type" << std::endl <<
		             "                            u32 - Unsigned 32-bit (default)" << std::endl <<
		             "                            u64 - Unsigned 64-bit" << std::endl <<
		             "                            s32 - Signed 32-bit" << std::endl <<
		             "                            s64 - Signed 64-bit" << std::endl <<
		             "           <-b [base]>    - Numerical system" << std::endl <<
		             "                            hex - Hexadecimal (default)" << std::endl <<
		             "                            dec - Decimal" << std::endl <<
		             "                            bin - Binary" << std::endl <<
		             "           <-f [symbol]>  - Force include symbol" << std::endl <<
		             "           <-x [symbol]>  - Exclude symbol" << std::endl <<
		             "           <-p [prefix]>  - Only include symbols with prefix" << std::endl <<
		             "           <-xp [prefix]> - Exclude symbols with prefix" << std::endl <<
		             "           <-s [suffix]>  - Only include symbols with suffix" << std::endl <<
		             "           <-xs [suffix]> - Exclude symbols with suffix" << std::endl;
		return -1;
	}

	Symbols*    symbols     = new Symbols();
	std::string input_file  = "";
	std::string output_file = "";
	ValueType   value_type  = ValueType::Unsigned32;
	NumberBase  number_base = NumberBase::Hex;

	try {
		for (int i = 1; i < argc; i++) {
			if (CheckArgument(argc, argv, i, "i")) {
				if (!input_file.empty()) {
					throw std::runtime_error("Input file already defined.");
				}

				input_file = argv[i];
				continue;
			}

			if (CheckArgument(argc, argv, i, "o")) {
				if (!output_file.empty()) {
					throw std::runtime_error("Output file already defined.");
				}

				output_file = argv[i];
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

			if (CheckArgument(argc, argv, i, "f")) {
				symbols->AddSymbolInclude(argv[i]);
				continue;
			}

			if (CheckArgument(argc, argv, i, "x")) {
				symbols->AddSymbolExclude(argv[i]);
				continue;
			}

			if (CheckArgument(argc, argv, i, "p")) {
				symbols->AddPrefixInclude(argv[i]);
				continue;
			}

			if (CheckArgument(argc, argv, i, "xp")) {
				symbols->AddPrefixExclude(argv[i]);
				continue;
			}

			if (CheckArgument(argc, argv, i, "s")) {
				symbols->AddSuffixInclude(argv[i]);
				continue;
			}

			if (CheckArgument(argc, argv, i, "xs")) {
				symbols->AddSuffixExclude(argv[i]);
				continue;
			}

			throw std::runtime_error(("Unknown argument \"" + (std::string)argv[i] + "\".").c_str());
		}

		if (input_file.empty()) {
			throw std::runtime_error("Input symbol file not defined.");
		}
		if (output_file.empty()) {
			throw std::runtime_error("Output symbol file not defined.");
		}

		symbols->LoadSymbols(input_file);
		symbols->Filter();
		symbols->Output(output_file, value_type, number_base);
	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		delete symbols;
		return -1;
	}

	delete symbols;
	return 0;
}
