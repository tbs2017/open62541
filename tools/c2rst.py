import sys
import os
import binascii
import re

remove_keyword = ["UA_EXPORT ", " UA_FUNC_ATTR_WARN_UNUSED_RESULT"]

def clean_comment(line):
    m = re.search("^( \* |/\*\* )(.*?)( \*/)?$", line)
    if not m:
        return "\n"
    return m.group(2) + "\n"

def clean_line(line):
    for keyword in remove_keyword:
        line = line.replace(keyword, "")
    return line

def comment_start(line):
    m = re.search("^/\*\*[ \n]", line)
    if not m:
        return False
    return True

def comment_end(line):
    m = re.search(" \*/$", line)
    if not m:
        return False
    return True

def first_line(c):
    "Searches for the first comment"
    for i in range(len(c)):
        if comment_start(c[i]):
            return i
    return -1

def last_line(c):
    "Searches for the latest ifdef (closing the include guard)"
    last = 1
    for i in range(1, len(c)):
        m = re.search("^#ifdef", c[i])
        if m:
            last = i
    return last

if len(sys.argv) < 2:
    print("Usage: python c2rst.py input.c/h output.rst")
    exit(0)

with open(sys.argv[1]) as f:
    c = f.readlines()

with open(sys.argv[2], 'w') as rst:
    in_doc = False
    for i in range(first_line(c), last_line(c)):
        line = c[i]
        doc_start = False
        doc_end = False
        if in_doc:
            doc_end = comment_end(line)
            line = clean_comment(line)
        else:
            doc_start = comment_start(line)
            if doc_start:
                doc_end = comment_end(line)
                line = clean_comment(line)

        if doc_start:
            in_doc = True

        if not ((doc_start or doc_end) and line == "\n"):
            if not in_doc:
                line = "   " + line
            rst.write(clean_line(line))

        if doc_end:
            rst.write("\n.. code-block:: c\n\n")
            in_doc = False
