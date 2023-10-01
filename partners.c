/***********************************************************************
 * Name:   Timur Kasimov                                               *
 * Assignment name ( Homework  3 Partner Pairing Program               *
 *                                                                     *
 * Assignment for <December, 5th, 2022>                                *
 ***********************************************************************/

/* *********************************************************************
 * Academic honesty certification:                                     *
 *   Written/online sources used:                                      *
 *    King textbook, CSC161 website,                                   *
 *   Help obtained                                                     *
 *                professor Barbara Johnson                            *
 *   My/our signature(s) below confirms that the above list of sources *
 *   is complete AND that I/we have not talked to anyone else          *
 *   (e.g., CSC 161 students) about the solution to this problem       *
 *                                                                     *
 *   Signature:     Timur Kasimov                                      *
 ***********************************************************************/
 
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXNAMELEN 50 //maximum first or last name length
#define MAXFILELEN 50 //maximum filename (needed for random assignment)

typedef struct student {
  char first[MAXNAMELEN];
  char last[MAXNAMELEN];
  int assigned;
} student_t;

/*****************
*  recordNames   *
******************/
//reads names from the file stream in the format Last,First and records them in the
//array students that consists of student_t structs
//pre-conditions: takes an array of student_t values, a pointer to an opened file,
//and the number of students to read from the file
//post-conditions: function does not return anything but records names in the student_t
//struct and sets each student assignment to 0.
void
recordNames(student_t students[], FILE * stream, int number){ 
  //skip the first and second line
  int skipped_line;
  fscanf(stream, "%d", &skipped_line);
  fscanf(stream, "%d", &skipped_line); 
  for (int i = 0; i < number; i++){ 
    //first scan full last name before coma, then skip coma, then
    //scan full first time before newline, then skip newline
    fscanf(stream, "%[^,],%[^\n]\n", students[i].last, students[i].first);
    students[i].assigned = 0; //set assignemnt to zero
      }
}//end recordNames

/*********************
*     createSets     *
**********************/
//deterministically creates specified number of sets with specified number of pairs in each set.
//the assignment algorithm takes a student and uses a step variable, incremented
//with each new set, to decide on the second student for the pair.
//pre-conditions: takes an array of students, a pointer to an opened output file,
//an integer that says how many sets to do, and the number of students in the
//students array
//post-conditions: returns none, but manages BOTH ODD AND EVEN number of students
void
createSets(student_t students_array[], FILE * output, int sets, int number){
  for (int i = 1; i <= sets; i++){ //loop for each set    
    int step = i; //step corresponds to the iteration of the set
    while (step >= number)//if step is more than the number of students, reduce it by number - 1        
      step = step - number + 1;
      
    int pair = 1; //keep track of what pair the function is currently working on
    int num_assigned = 0;//how many students assigned so far
    student_t students[number];//create a temporary students array with the same size as the one passed

    for (int k=0; k < number; k++)     
      students[k] = students_array[k];//copy the passed array into the created temporary array   
    fprintf(output, "Set %d:\n", i); //print set number

    for (int j = 0; j < number; j++){ //loop for creating each pair       
      while (j < number && students[j].assigned == 1)
        j++; //find the first unassigned student       
      if (j < number){ //first unassigned student        
        //case for odd number of students
        if (num_assigned == (number - 1))//if all pairs created and only one student left unassigned
          {
            fprintf(output, " and %s %s", students[j].first, students[j].last);//add the last student to the last created pair
            students[j].assigned = 1;
            num_assigned++;                
          }//end case for odd number of students
        else if (num_assigned < (number - 1)){//if at least 2 students are still unassigned                      
          fprintf(output, "\n\tPair %d: %s %s and ", pair,//print pair number and the  first student
                  students[j].first, students[j].last);                  
          students[j].assigned = 1;//mark student assigned
          num_assigned++;//increment number of students assigned
          //case when step doesn't reach beyond the end of array
          if (j+step < number && students[j+step].assigned == 0){//find the second student based on step and size of array              
            fprintf(output, "%s %s", students[j+step].first, students[j+step].last);//print the second student in the pair
            students[j+step].assigned = 1;//mark student assigned
            num_assigned++;//increment number of students assigned 
            pair++;//increment number of pairs of at least two students
          }
          //case when step reaches beyond the end of array
          else if (j+step >= number){//if step gets beyond the array size              
            j = 0;//get back to the beginning of the array
            while (j < number && students[j].assigned == 1)                  
              j++; //find the first unassigned student                  
            if (j < number){ //first unassigned student                  
              fprintf(output, "%s %s", students[j].first, students[j].last);//print the second student in the pair             
              students[j].assigned = 1;//mark student assigned
              num_assigned++;//increment number of students assigned 
              pair++;//increment number of pairs of at least two students
            }//else all students must be assigned, do nothing                  
          }        
        }             
      }         
    }
    fprintf(output, "\n\n\n");//separate each set from each other visually
  }
} //end createSets

/***************************
 *     createRandomSets     *
 ****************************/
//randomly creates specified number of sets with specified number of pairs in each set.
//pre-conditions: takes an array of students, a pointer to an opened output file,
//an integer that says how many sets to do, and the number of students in the
//students array
//post-conditions: returns none, but manages BOTH ODD AND EVEN number of students
void
createRandomSets(student_t students_array[], FILE * output, int sets, int number){
  srand((unsigned) time(NULL));
  for (int i = 1; i <= sets; i++){ //loop for each set
    int pair = 1; //keep track of what pair the function is currently working on
    int num_assigned = 0;//how many students assigned so far
    student_t students[number];//create a temporary students array with the same size as the one passed
    int random;
    int total_pairs = number / 2;

    for (int k=0; k < number; k++)     
      students[k] = students_array[k];//copy the passed array into the created temporary array   
    fprintf(output, "Set %d:\n", i); //print set number

    while (num_assigned < number){ //while loop for creating each pair       
      do {
        random = rand() % number;
      } while (students[random].assigned != 0);//find unassigned student to start pair
 
      //case for odd number of students
      if (num_assigned == (number - 1))//if all pairs created and only one student left unassigned
        {
          fprintf(output, " and %s %s", students[random].first, students[random].last);//add the last student to the last created pair
          students[random].assigned = 1;
          num_assigned++;                
        }//end case for odd number of students
      else if (num_assigned < (number - 1)){ //if at least 2 students are still unassigned                      
        fprintf(output, "\n\tPair %d: %s %s and ", pair,//print pair number and the  first student
                students[random].first, students[random].last);                  
        students[random].assigned = 1;//mark student assigned
        num_assigned++;//increment number of students assigned        

        do {
          random = rand() % number;
        } while (students[random].assigned != 0);//find unassigned student to complete pair                           
        fprintf(output, "%s %s", students[random].first, students[random].last);//print the second student in the pair
        students[random].assigned = 1;//mark student assigned
        num_assigned++;//increment number of students assigned 
        pair++;//increment number of pairs of at least two students
      }                          
    }             
    fprintf(output, "\n\n\n");//separate each set from each other visually
  }         
} //end createRandomSets

int
main(int argc, char *argv[]){
  if (argc == 3){}// if 3 arguments, do nothing
  else if (argc == 4 && (strcmp(argv[1], "-r") == 0)){ //if 4 arguments and 2nd arg is -r
    argv[1] = malloc(sizeof(char) * MAXFILELEN); //allocate memory for argv[1]
    if (argv[1] == NULL){  //check memory allocation was succesfful
      fprintf(stderr, "Error allocating memory");
      return 1;
    }
    strcpy(argv[1], argv[2]); //copy third argument into second argument
    argv[2] = malloc(sizeof(char) * MAXFILELEN); //allocate memory for argv[2]
    if (argv[2] == NULL){  //check memory allocation was succesfful
      fprintf(stderr, "Error allocating memory");
      return 1;
    }
    strcpy(argv[2], argv[3]);//copy third argument into second argument 
  }
  else { //else print usage error
    fprintf(stderr, "Usage: %s -r(optional) input_filename output_filename\n", argv[0]);
    return 1;
  }


  FILE * stream = fopen(argv[1], "r"); //Open the input file
  if (stream == NULL){ //verify file was opened
    fprintf(stderr, "%s : Error opening file %s\n", //report failure
            argv[0], argv[1]);
    return 1;
  }
     
  int number = 0;//for number of students
  int sets = 0;//for number of sets  
  fscanf(stream, "%d %d", &number, &sets);//read the number of students and the number of sets
  
  student_t students[number]; //declare the array of students with the length equal to the num of students
  recordNames(students, stream, number);//record names from stream

  //close stream
  if (fclose(stream)){ //verify file closed
    fprintf(stderr,"%s : Error closing file %s\n", //or report error
            argv[0], argv[1]);  
    return 1;
  }
  
  //open output file for writing
  FILE * output = fopen(argv[2], "w"); //Open the output file for writing
  if (output == NULL){ //verify output file was opened
    fprintf(stderr, "%s : Error opening file %s\n", //report failure
            argv[0], argv[2]);
    return 1;
  }
  
  //create sets deterministically or randomly
  if (argc == 3) //deterministic case
    createSets(students, output, sets, number);
  else if (argc == 4)//random case
    createRandomSets(students, output, sets, number);

  //close output
  if (fclose(output)){ //verify file closed
    fprintf(stderr,"%s : Error closing file %s\n", //or report error
            argv[0], argv[2]);
    return 1;     
  }
  return 0;
}//end main  
