#! /usr/bin/python
#
# Usage: update-headers.py [--force]
#
#                               POK header
#
# The following file is a part of the POK project. Any modification should
# be made according to the POK licence. You CANNOT use this file or a part
# of a file for your own project.
#
# For more information on the POK licence, please see our LICENCE FILE
#
# Please follow the coding guidelines described in doc/CODING_GUIDELINES
#
#                                      Copyright (c) 2007-2020 POK team

from datetime import datetime
import itertools
import os
import sys

MARKER = "POK header"

HEADER = """
                              {marker}

The following file is a part of the POK project. Any modification should
be made according to the POK licence. You CANNOT use this file or a part
of a file for your own project.

For more information on the POK licence, please see our LICENCE FILE

Please follow the coding guidelines described in doc/CODING_GUIDELINES

                                     Copyright (c) 2007-{year} POK team
""".format(marker = MARKER, year = datetime.now().year).split("\n")[1:-1]

extensions = {}

def register(handler, *exts):
    """Register a handler for a list of suffixes."""
    for ext in exts:
        extensions[ext] = handler

class Lang(object):

    def __init__(self, begin_delimeter, middle_delimeter = None, end_delimeter = "", empty_after = 1):
        self.begin_delimeter = begin_delimeter if begin_delimeter != middle_delimeter else ""
        self.middle_delimeter = middle_delimeter or begin_delimeter
        self.end_delimeter = end_delimeter
        self.empty_after = empty_after

    def has_header(self, content):
        """Return True if the header marker is present."""
        return any(MARKER in l for l in content)

    def fix_header(self, filename, force):
        """Fix a header in place."""
        try:
            content = [l.strip("\n") for l in open(filename)]
            content = self.add_header(content, force)
            open(filename, "w").write("\n".join(content) + "\n")
        except Exception as e:
            print("Error while handling file {}: {}".format(filename, e))
            raise

    def add_header(self, content, force):
        """Add a header if absent, or after removing an existing one if force is True."""
        if self.has_header(content):
            if force: (before, content) = self.remove_header(content)
            else: return content
        else:
            before = []
        return before + \
            [l.rstrip() for l in ([self.begin_delimeter] if self.begin_delimeter != self.middle_delimeter else []) + \
             ["{} {}".format(self.middle_delimeter, l) for l in HEADER]] + \
             ([self.end_delimeter] if self.end_delimeter else []) + \
             ([""] * self.empty_after + content if content else [])

    def remove_header(self, content):
        """Remove the header, preserving the lines before and after it. The lines before
        the header are returned as the first element of the couple, the rest of the file
        without the header as the second element."""
        i = 0
        (bd, md, ed) = self.begin_delimeter.strip(), self.middle_delimeter.strip(), self.end_delimeter.strip()
        begin_found = 0
        while MARKER not in content[i]:
            if content[i].strip().startswith(bd):
                begin_found = i
            i += 1
        if self.begin_delimeter != self.middle_delimeter:
            i = begin_found
        while i < len(content):
            l = content[i].strip()
            if (bd and l.startswith(bd)) or l.startswith(md):
                del content[i]
                continue
            if ed and l.startswith(ed):
                del content[i]
                break
            break
        return content[:i], list(itertools.dropwhile(lambda s: s.strip() == "", content[i:]))

register(Lang("--"), "ads", "adb", "aadl")  # Ada and AADL
register(Lang("/*", " *", " */"), "c", "h")  # C
register(Lang("%"), "tex")                  # LaTeX
register(Lang("#"), "pm", "pl", "py")       # Perl and Python

force = sys.argv[1:2] == ["--force"]

for filename in (f.strip() for f in os.popen("git ls-files")):
    handler = extensions.get(filename.split(".")[-1])
    if handler:
        handler.fix_header(filename, force)
