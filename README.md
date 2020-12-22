# CAT-16
# A 16-bit computer built from 74-series logic chips on breadboards.

The CAT-16 is a fully-functional 16-bit computer that I designed and built from June to August of this year. The design is VERY loosely based off of Ben Eater's 8-bit breadboard computer (https://eater.net/8bit), but I made many design decisions that differ from his so that the computer would better fit my needs. The name CAT-16 doesn't really mean much at all (CAT because I love cats and 16 because it's a 16-bit computer, but I like the way it sounds. I have uploaded some schematics and other documents that I drew when designing the computer that outline its architecture, but please keep in mind that many of them may differ slightly from the final design since there were a lot of unexpectd problems that I encountered along the way. After building the computer, I wrote a rudimentary (and probably very inelegant) but effective assmebler for it and configured an Arduino to automatically enter the resulting machine code into the CAT-16's memory, greatly simplifying the process of writing and entering code for it.

# Files/Folders:
  - The ROMs folder contains the binary files that I burned into the five EEPROMs that I used to store the computer's microcode.
  - The samples folder contains several assembly language sample programs that I wrote for the CAT-16. They are outlined below.
    - BCDConverter.txt converts a number to binary coded decimal so that it can be displayed on the computer's character LCD.
    - cats.txt expresses my love for cats by repeatedly printing the message "I love cats so much!" on the LCD.
    - divide.txt divides two numbers and stores the result in a memory address.
    - multiply.txt multiplies two numbers through repeated addition.
    - shiftMultiplication.txt multiplies numbers by performing left and right binary shifts on the numbers.
  - BreadboardComputer.ino is an arduino sketch that interfaces the assembler with the CAT-16, allowing the assmebler to enter machine code directly into the CAT-16's    RAM and allowing control of the CAT-16's clock from the host computer.
  - CAT-16 Microcode.xlsx is an Excel spreadsheet that I used when creating all of the microinstructions for the CAT-16's instruction set.
  - a.out is an example of the machine code that the assembler outputs. The leftmost numbers are the addresses at which the data should go (in multiples of two) and      the two rightmost numbers are the data that goes into that address and the subsequent address, respectively).
  - assembler.py is the program that assembles code for the CAT-16. After generating machine code, it can enter it directly into the CAT-16's RAM with the help of an     Arduino. The assembler also allows the user to start and stop the computer's clock, adjust the clock speed, and single step the clock. I should have put             comments in this code because it's really confusing!
  - assemblerOld.py is a slightly older version of the assembler that supported fewer instructions and was slightly less advanced.
  - workingData.txt is a temp file that the assembler uses when it's parsing variable and label names and converting them into addresses.
  
