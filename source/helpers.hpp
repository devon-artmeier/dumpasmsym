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

#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <fstream>
#include <string>

extern std::string StringToLower      (const std::string& str);
extern bool        CheckArgument      (const int argc, char* argv[], int& index, const std::string& option, bool ignore_case = true);
extern void        ReadInput          (std::ifstream& input, char* const read_buffer, const std::streamsize read_count);
extern long long   ReadInputNumberPsyQ(std::ifstream& input, bool is_signed);
extern long long   ReadInputNumberVObj(std::ifstream& input, bool is_signed);
extern std::string ReadInputStringPsyQ(std::ifstream& input);
extern std::string ReadInputStringVObj(std::ifstream& input);
extern bool        StringStartsWith   (const std::string& str, const std::string& prefix);
extern bool        StringEndsWith     (const std::string& str, const std::string& suffix);

#endif // HELPERS_HPP
