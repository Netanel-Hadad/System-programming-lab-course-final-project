# System-programming-lab-course-final-project
System programming lab course final project, semester 2024b.

Assembler machine written in C language, coverting assembly code file to machine code.

Project grade 96.

# Project overview:
This program is divided into 4 major stages, which in the end of them, an '.as' file is converted
into '.ob' file if no erros were found in the file. '.ent' and '.ext' files will be created only
if needed, if the file has entries or externs.

## Macro Stage:
In the this stage we read the '.as' file and create a Macro Table containing all the macros 
declared in the file. While reading the file we check for errors only in the macro declerations,
if no erros were found we create '.am' file containing the same code but with the macros defined.
if an error was found, we dont go thorugh the next stage.

## First Run:
In the first run we read the '.am', look for erros in the code and convert lines into machine code.
In this stage we can not convert labels into machine code as a label might be declared after being used, but,
every time a label is declared we add it into a labels list and keep its information.

## Second Run:
In the second run we finish converting the labels in the code, using the list we created in the
first run. In this stage we also track the appearances of all the extern labels, so we could create the
'.ext' file, if needed.
If no errors were found in the first and second run, we go to the next stage.

## Export Stage:
We have the machine code for the entire program, a list of all labels (and if they are entries)
and all the apperances of extern labels and we checked the file for errors.
Now all thats left is to create the '.ob' file and the '.ent' and '.ext' files, if needed.

## Notes:
- when an error is found, a message is printed to the user with the file name, line number
	and an explanation of the error.
- tests files are in the 'test' folders. test1 is a simple file with no erroes,
	test2 has many macro decleration errors, test3 is the example from the project book,
	test4 passed the macro stage but has errors that appear in the first and second run,
	and test5 has no errors and no extern or entries.

# Screenshots
Input file (.as file):

![Screenshot 2025-03-08 214819](https://github.com/user-attachments/assets/0d53e8f2-6164-444f-aa2d-c8d622366978)

Output files (.ext, .ent and .ob files):

![Screenshot 2025-03-08 215122](https://github.com/user-attachments/assets/c61768b0-258b-4875-9522-970835ca2fd3)

![Screenshot 2025-03-08 215110](https://github.com/user-attachments/assets/cebc4efa-598a-4537-af9a-52a13f44bb18)

![Screenshot 2025-03-08 214835](https://github.com/user-attachments/assets/af135513-3158-4371-b7bb-80e11371536a)
