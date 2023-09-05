import os
import subprocess

# Specify the path to your macro file
macro_file_path = "/home/rpitelka/caen-analysis/biposlist.mac"

directory_path = "/nfs/disk4/ratds_230515_230715_bronze/"

def editMacro(macro_file_path, placeholder, replacement):
    # Read the content of the macro file
    with open(macro_file_path, 'r') as file:
        macro_content = file.read()

    # Replace placeholder text with actual filename
    macro_content = macro_content.replace(placeholder, replacement)

    # Write the modified content back to the macro file
    with open(macro_file_path, 'w') as file:
        file.write(macro_content)

# Execute the macro via the command line
def runMacro(macro_file_path):
    # Replace 'macro_executable' with the actual command to run macro
    macro_command = "q rat {}".format(macro_file_path)
    subprocess.call(macro_command, shell=True)

placeholder = "FILENAME"
iteration_count = 0
for filename in os.listdir(directory_path):
    if iteration_count % 50 == 0:
        user_input = input("Press Enter to continue or 'q' to quit: ")
        if user_input.lower() == 'q':
            break  # Exit the loop if the user enters 'q'
    if filename.endswith(".root"):
        editMacro(macro_file_path, placeholder, filename)
        runMacro(macro_file_path)
        placeholder = filename
        print(filename + " analyzed and saved")
    else:
        continue
