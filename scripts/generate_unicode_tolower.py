#!/usr/bin/env python3

                                                 
 
                                      
                                                                
 
                                                                    
                                                                                         
                                                                                         

import sys

import unicode


def generate_cases():
	ud = unicode.data()
	return [(unicode.unhex(u["Value"]), unicode.unhex(u["Simple_Lowercase_Mapping"])) for u in ud if u["Simple_Lowercase_Mapping"]]


def gen_header():
	print("""\
/* AUTO GENERATED! DO NOT EDIT MANUALLY! See scripts/generate_unicode_tolower.py */
#include <cstdint>
#include <unordered_map>

extern const std::unordered_map<int32_t, int32_t> UPPER_TO_LOWER_CODEPOINT_MAP;""")


def gen_data(cases):
	print("""\
/* AUTO GENERATED! DO NOT EDIT MANUALLY! See scripts/generate_unicode_tolower.py */
#include "tolower_data.h"

const std::unordered_map<int32_t, int32_t> UPPER_TO_LOWER_CODEPOINT_MAP = {""")
	for upper_code, lower_code in cases:
		print(f"\t{{{upper_code}, {lower_code}}},")
	print("};")


def main():
	header = "header" in sys.argv
	data = "data" in sys.argv

	if header:
		gen_header()
	elif data:
		gen_data(generate_cases())


if __name__ == "__main__":
	main()
