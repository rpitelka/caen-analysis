import subprocess

# Specify the path to your macro file
macro_file_path = "biposlist.mac"

# Read the content of the macro file
with open(macro_file_path, 'r') as file:
    macro_content = file.read()

# Replace "FILENAME" with "test"
macro_content = macro_content.replace("FILENAME", "test")

# Write the modified content back to the macro file
with open(macro_file_path, 'w') as file:
    file.write(macro_content)

# Execute the macro via the command line
macro_command = f"macro_executable {macro_file_path}"  # Replace 'macro_executable' with the actual command to run your macro
subprocess.run(macro_command, shell=True)
