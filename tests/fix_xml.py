#!/usr/bin/env python3

# Simple script to fix XML tags in the test file
import re

def fix_file():
    with open('test_xmlvalidator.cpp', 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Replace all instances of </s> with </System> using regex to catch any weird characters
    content = re.sub(r'</s>', '</System>', content)
    content = re.sub(r'</\w>', '</System>', content)  # Replace any single character closing tag
    
    with open('test_xmlvalidator.cpp', 'w', encoding='utf-8') as f:
        f.write(content)
    
    print("Fixed XML tags in test_xmlvalidator.cpp")

if __name__ == '__main__':
    fix_file()
