# partner_pairings
This program creates both comprehensive and random partner pairings for class settings, for example for pair programming in CS classes.

Comprehensive here means that the program makes sure to pair every student with everyone else in the class before repeating previous pairings. The program also achieves that deterministically, meaning that comprehensive pairs always come out the same for the same list of students. 

Random pairings do not come out deterministically and always change. 

The program handles both even and odd numbers of students in class rosters.

# Compiling and running
To compile:
```
clang -o partners partners.c
```
To run:
```
./partners -r(option) input_file output_file
```
Note that ```-r``` is the option to create pairs randomly rather than deterministically
