import os
import re


def count_code_lines(directory, exclude_dirs=None):
    total_lines = 0
    pattern = re.compile(r'^\s*//|^\s*/\*|\*/|^\s*\*')

    if exclude_dirs is None:
        exclude_dirs = []

    for root, dirs, files in os.walk(directory):
        dirs[:] = [d for d in dirs if d not in exclude_dirs]
        for file in files:
            if file.endswith('.hpp') or file.endswith('.cpp'):
                file_path = os.path.join(root, file)
                with open(file_path, 'r', encoding='utf-8') as f:
                    lines = f.readlines()
                    code_lines = 0
                    in_comment_block = False
                    for line in lines:
                        line = line.strip()
                        if not in_comment_block:
                            if pattern.match(line) or not line:
                                continue
                            else:
                                code_lines += 1
                                if '/*' in line:
                                    in_comment_block = True
                        else:
                            if '*/' in line:
                                in_comment_block = False
                    print(f"{file_path}: {code_lines} lines")
                    total_lines += code_lines

    print(f"Total lines of code: {total_lines}")


# 指定要计算的目录
directory_to_scan = "E:\\Code\\CppTide\\src"
exclude_dirs = ["models"]
count_code_lines(directory_to_scan, exclude_dirs)
