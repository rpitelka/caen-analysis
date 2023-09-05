def convertCSV(input_text):
    # Open the input text file
    with open(input_text, 'r') as input_file:
        # Read the lines from the input file
        lines = input_file.readlines()

    # Create or open the output CSV file
    with open('output.csv', 'w') as output_file:
        # Iterate through the lines from the input file
        for line in lines:
            # Split the line into two columns using white space as the separator
            columns = line.split()
            
            # Check number of columns
            if len(columns) == 2:
                # Join the columns with commas and write to the output file
                output_line = ','.join(columns) + '\n'
                output_file.write(output_line)
            elif len(columns) == 3:
                # Write the first two columns as a row with a repeated first entry
                output_line = "{},{}\n".format(columns[0], columns[1])
                output_file.write(output_line)
                
                # Write the first and third columns as another row with a repeated first entry
                output_line = "{},{}\n".format(columns[0], columns[2])
                output_file.write(output_line)

    print("Conversion complete. Output saved to 'output.csv'")

input_text = "InWindow_BiPos_GTID.txt"
# input_text = "OutWindow_BiPo212_GTID.txt"
# input_text = "OutWindow_BiPo214_GTID.txt"
convertCSV(input_text)
