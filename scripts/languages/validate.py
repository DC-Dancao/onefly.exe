#!/usr/bin/env python3
import os
import re
import sys

import twlang

os.chdir(os.path.dirname(__file__) + "/../..")

                                                                                                     
cfmt = """
(                                  # start of capture group 1
%                                  # literal "%"
(?:                                # first option
(?:[-+0 #]{0,5})                   # optional flags
(?:\\d+|\\*)?                      # width
(?:\\.(?:\\d+|\\*))?               # precision
(?:h|l|ll|w|I|I32|I64)?            # size
[cCdiouxXeEfgGaAnpsSZ]             # type
) |                                # OR
%%)                                # literal "%%"
"""

total_errors = 0


def print_validation_error(error, filename, error_line):
	print(f"Invalid: {translated}")
	print(f"- {error} in {filename}:{error_line + 1}\n")
	global total_errors
	total_errors += 1


if len(sys.argv) > 1:
	languages = sys.argv[1:]
else:
	languages = twlang.languages()
local = twlang.localizes()

for language in languages:
	translations = twlang.translations(language)

	for (english, _), (line, translated, _) in translations.items():
		if not translated:
			continue

		                                                                                
		if re.findall(cfmt, english, flags=re.VERBOSE) != re.findall(cfmt, translated, flags=re.VERBOSE) and "1$" not in translated:
			print_validation_error("Non-matching formatting", language, line)

		                    
		if "…" in english and "..." in translated:
			print_validation_error("Usage of ... instead of the … character", language, line)

if total_errors:
	print(f"Found {total_errors} {'error' if total_errors == 1 else 'errors'} ")
	sys.exit(1)
