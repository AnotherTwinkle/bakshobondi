import os

def wrap_header_with_guard(file_path):
    file_name = os.path.basename(file_path)
    guard_name = file_name.upper().replace('.', '_')  # e.g., myfile.h -> MYFILE_H

    with open(file_path, 'r') as f:
        content = f.read()

    # Skip if already has a guard
    if f"#ifndef {guard_name}" in content:
        print(f"Skipping {file_path}, already has include guard.")
        return

    wrapped_content = f"#ifndef {guard_name}\n#define {guard_name}\n\n{content}\n\n#endif // {guard_name}\n"

    with open(file_path, 'w') as f:
        f.write(wrapped_content)
    
    print(f"Processed {file_path}")

def process_directory(directory):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".h"):
                file_path = os.path.join(root, file)
                wrap_header_with_guard(file_path)

if __name__ == "__main__":
    directory = input("Enter directory path: ").strip()
    process_directory(directory)
