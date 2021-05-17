Assignment: Programming Assignment 2
Name: Toby Maxime Savage
Email: tosa5156@colorado.edu

###### FILES ######
1) /home/kernel/modules/Makefile
This file contains the object declaration of the device driver in order to build the module. Here, you will find the line 'obj-m:=part2Device', which builds the device driver module.

2) /home/kernel/modules/part2Device.c
This file is the Device Driver LKM source file. The program is written in C and contains all of the implementations of the read, write, open, close, and seek functions. 

3) /home/kernel/modules/userTester.c
This file is the tester file for the device driver LKM. This function builds a buffer that is used in the read, write, open, close, and seek functions. By running this program, you will be able to test the buffer by reading, writing, and seeking through the buffer.


###### SETUP ######
1) Place all of the files in this .zip package into the correct directories above. (You may have to create your own 'modules' folder)

2) ​Run the command: 'make -C /lib/modules/$(uname -r)/build M=$PWD', which compiles the module object defined in the Makefile.

3) Run the command 'sudo rmmod part2Device' to remove the module (run this after modification of source file)

4) Run the command 'sudo ins mod part2Device.ko', which inserts the module into the linux kernel.

###### TESTING ######
1) First, compile the userTest.c program by typing 'gcc userTester.c -o userTester' into the terminal

2) To run the user program, type './userTester'. This will execute the program.

3) Now, you may chose between 'r' = read, 'w' = write, 's' = seek, and 'e' = exit program.

###### Additional Commands ######
1) 'sudo dmesg --clear'
This command can be used to clear out your 'dmesg' logs so you can clearly see the newly created logs you are testing without any confusion.

2) 'dd if=/dev/zero of=/dev/part2Device bs=1024 count=1'
In the event you change the .c source file, and need to run the 'make' command again, you may have to empty out your buffer. By recompiling the .c source file, you may change the address of the buffer, thus resulting to garbage inside the old buffer location. By Running this command, you clear out what is inside your new buffer.