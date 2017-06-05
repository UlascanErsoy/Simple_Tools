#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
/** Grades AA BA BB CB CC DC DD FF
  *	   	   40 35 30 25 20 15 10 00 
  *A = 2.0 B = 1.5 C = 1.0 D = 0.5 F 0.0
  *
  * Commands -open (char* dir) 
  * 	     -close 
  *          -add (int weight/credit) (char* name) (char* grade)
  *          -remove (int index)
  *          -save (char* dir)
  * 	     -list 
  *          -evaluate 
  *          -quit
  *	     -help
  *DATA STRUCTURE ///////////////////////////////////////////////
  *0x00 - 0x04 (INT) lenght of the list
  *Value-> 2bytes Weight->4bytes 4bytes->Lenght of the name Name->Indefinite read until 
  *
  * 
  */                                                           

typedef struct grade{char* value; int weight; char* name;}grade;//Grade structure 
typedef struct Queue{unsigned int size; unsigned int count; ; grade* _array;}Queue;
typedef enum{false ,true} bool;//Booleans are really cool

#define s_com '-'
#define isEqual( str1 , str2)(strcmp(str1 , str2) == 0)
#define HELP "-open (directory) \n-add (credit) (name) (grade AA-FF) \n-remove (index) \
			  \n-list\n-save(directory) \n-evaluate \n-quit \n-about \n-replace (index) (credit) (name) (grade AA-FF) \n"
#define ABOUT "Created by Ulascan Ersoy 2017 ITU EEB , GPA Calculator! V0.0.0.2"																		

const char  READ[2] = "rb";	//File mode Read binary
const char WRITE[2] = "wb"; //File mode Write binary

//Function prototypes
int open(char* dir);				       //outputs 1 if succesfull
int add(char* credit , char* name , char* _grade);     //outputs 1 if succesfull
int replace(char* index , char* credit , char* name , char* _grade); //outputs 1 if succesfull
int _remove(char* index);		               //outputs 1 if succesfull
int save(char* dir);			      	       //outputs 1 if succesfull
void list(); 				       	       //Doesn't do jack-shit if unsuccesfull
void evaluate();				       //Doesn't do jack-shit if unsuccesfull
void list_flush();
char* input(FILE* f);
unsigned int grade_size(grade g);


//List 
Queue* p_grades , _grades;

unsigned int q_init(Queue* _grades){

_grades = malloc(8);
_grades->count = 0 ;
_grades->size  = 0 ;

return 1;
}//End of q_init 




int main(){ 
	
	bool quit = false;//Quit flag
	char* command ;   //command

	q_init(p_grades);  //Initialize _grades queue


	while(!quit){
	
	command = input(stdin); //Read user-input

	if(command[0]!= s_com){  printf("Not a command -help \n");}					//Command interpretation
		else{
			int j , _args = 0;
			char* _body;
			char* argv[10];
			for(j = 0 ; command[j+1]!= '\0' ; ++j) {
				if(command[j] == '-')	
				argv[_args] = &command[j]; //add the command

				if(command[j] == ' '){

					command[j] = '\0';
					_args++;//Fetch the amount of arguments	
					argv[_args] = &command[j];
					argv[_args]++;
					
				}//End of if

			}//End of for loop
			
			//COMMANDS

			if(isEqual(argv[0],"-quit"  ) 
			|| isEqual(argv[0],"-q"    ))
			quit = true;
			if(isEqual(argv[0],"-help"  ) 
		    	|| isEqual(argv[0],"-h"    ))
			puts(HELP);
			if(isEqual(argv[0],"-about" ))
			puts(ABOUT);
			if(isEqual(argv[0],"-open"  ))
			open(argv[1]);
			if(isEqual(argv[0], "-add"  ))
			add(argv[1],argv[2],argv[3]);
			if(isEqual(argv[0],"-remove"))
			_remove(argv[1]);
			if(isEqual(argv[0], "-save" ))
			save(argv[1]);
			if(isEqual(argv[0], "-list" ))
			list();
			if(isEqual(argv[0], "-evaluate" ))
			evaluate();
			if(isEqual(argv[0], "-flush" ))
			list_flush();
			if(isEqual(argv[0], "-replace"))
			replace(argv[1] , argv[2] , argv[3] , argv[4]);

			

		}//End of command
		
	}//End of the main-loop


return 0;
}  //End of main

//Replaces a given index with new data
int replace(char* index , char* credit , char* name , char* _grade){

	
	int _index = atoi(index); //Convert index to integer
	if(_index > _grades.count - 1)return 0; //Index out of bounds

	grade *pg , g;	//grade

	//Calc the initial size
	g = _grades._array[_index];
	unsigned int i_size = grade_size(g);

	unsigned int _credit = atoi(credit);
	if(_credit <= 0 || _credit >= 10){
		puts("Error : (-) credits value!");
		return -1;
	}//End of if
	if(strlen(_grade)!= 2 || (!isEqual(_grade , "AA") && !isEqual(_grade , "BA") &&!isEqual(_grade , "BB") &&                       	
	   !isEqual(_grade , "CB") && !isEqual(_grade , "CC") && !isEqual(_grade , "DC") && !isEqual(_grade , "DD")&& 
	   !isEqual(_grade , "FF"))) { //Check if the format is correct									
		puts("Error : Please enter your grade in AA-FF format!");								
		return -1;
	}//End of if

	g.name  = name;  	//Name of the class
	g.value = _grade;	//Grade AA-FF
	g.weight= _credit;	//Weight for avaraging

	//Reallocate memory 
	_grades._array = realloc(_grades._array,(_grades.size += (i_size - grade_size(g)) * 8)); //Add the size difference to the total
	_grades._array[_index] = g;
													





}//End of replace


//Opens the file and adds all the items to the list
int open(char* dir){
	 
	FILE* f = fopen( dir , READ );
	if(!f){
		puts("Error ; Invalid directory!");
		return -1;
	}//End of if
	if(_grades.size > 0){
		printf("Do you want delete your current list? (Y/N)");
		char c;
		scanf("%c", &c); 
	
			if(c == 'Y' || c == 'y'){
				
				list_flush();
		

			}else{

				puts("Cancelled!");
				return -1;
				
				}//end of if Y
		
		}//end of if size

	 int temp , lenght , i;						//counter setup
	 fread(&temp ,sizeof(int), 1 ,f);
	 lenght = temp;								//Temporary val
	 	
		
	 for(i = 0 ; i < lenght ; i++){
	
		int weight , len;
		char grade[2] , name ;
		char* nm , *gr ,*_cred ,*_name;
		gr = malloc(16);
	fread(&weight , sizeof(int) , 1 ,f);
	fread(&grade  , sizeof(char)   , 2 ,f);
	fread(&len    ,sizeof(int)     , 1 ,f);
	nm = malloc (sizeof(char) * len);
	_name = malloc (sizeof(char) * len);							
	fread(&name   , sizeof(char), len  ,f);
	nm = &name;

	_cred = malloc(sizeof(char) * 2);
	sprintf(_cred , "%d" , weight);
	sprintf(_name , "%s" ,    nm );
	sprintf(gr ,"%s"  ,  grade);

	add(_cred, _name , gr);
	
	
	}//End of for-loop

}//End of open

//Takes strings from stdin with indefinete sizes
char* input(FILE* f){

	int size = 15;
	int ch;
	char* str;
	int len = 0;
	str =  realloc(NULL , sizeof(char) * size);
	if(!str)return str;
	
		while(EOF!=(ch=fgetc(f)) && ch != '\n'){

			str[len++] = ch;
			if(len == size)
				str = realloc(str, sizeof(char) * (size+=16));


		}//End of while
	str[len++] = '\0';
	return realloc(str , sizeof(char)* size);
}//End of input


//Adds elements to the list
int add(char* credit , char* name , char* _grade){

	grade *pg , g;	//grade


	unsigned int _credit = atoi(credit);
	if(_credit <= 0 || _credit >= 10){
		puts("Error : (-) credits value!");
		return -1;
	}//End of if
	if(strlen(_grade)!= 2 || (!isEqual(_grade , "AA") && !isEqual(_grade , "BA") &&!isEqual(_grade , "BB") &&     
	   !isEqual(_grade , "CB") && !isEqual(_grade , "CC") && !isEqual(_grade , "DC") && !isEqual(_grade , "DD")&& 
	   !isEqual(_grade , "FF"))) { //Check if the format is correct												 
		puts("Error : Please enter your grade in AA-FF format!");												  
		return -1;
	}//End of if

	g.name  = name;  	//Name of the class
	g.value = _grade;	//Grade AA-FF
	g.weight= _credit;	//Weight for avaraging

	//Reallocate memory 
//	p_grades = malloc((_grades.size + grade_size(g))* 8);
	_grades._array = realloc(_grades._array,(_grades.size += grade_size(g) * 8));
	_grades._array[_grades.count] = g;
	_grades.count++;														

	return 0;

}//End of add

//Saves the file
int save(char* dir){

	FILE* f = fopen(dir , WRITE);
	if(!f){

		puts("Error: Invalid directory!");
		return -1;

	}//End of if
	if(_grades.size == 0){
		puts("Error: Your list is empty");
		return -1;

	}//End of if

	long int lenght = _grades.count;

	fwrite(&lenght , 1 ,sizeof(int) , f );	//Write the header

	int i;
	for(i = 0 ; i < lenght ; i++){

		int len = strlen(_grades._array[i].name);
	
		fwrite(&_grades._array[i].weight , 1 , sizeof(int) , f); //Write the weight
		fwrite(_grades._array[i].value , 2 , sizeof(char) , f);	 //Write the grade
		fwrite(&len , sizeof(int), 1 , f);
		fwrite(_grades._array[i].name ,len,sizeof(char),f);      //Write the name

	}//End of for loop

	fclose(f);
 

}//End of save

/**Removes the given index from the list
  *Reallocates the memory
  */
int _remove(char* index){

	//Convert to int
	int _index = atoi(index);
	//Check if out-of-bounds
	
	if(_grades.size == 0){

		puts("Error : Your list is empty!");
		return -1;

	}//End of if
	if(_index < 0 || _index > _grades.count - 1 ){
		puts("Error : Index out of bounds!");
		return -1;
	}//End of if

	 grade g = _grades._array[_index];
	_grades._array[_index] = _grades._array[_grades.count - 1];						//Swap the last item with the one we want to remove					
	_grades._array = realloc(_grades._array,_grades.size +grade_size(g) * 8);	//Reallocate memory
	_grades.count--;															//-1 the list size
	if(_grades.count == 0)_grades.size = 0;

return 0;
}//End of remove


//Prints the list elements
void list(){

	int i , lenght = _grades.count;			 //Counter setup
	puts("Index\t\t Credit\t\t Name\t\t Grade"); //Formatting

	for(i = 0 ; i < lenght ; i++){

		printf("%d\t\t %d\t\t %s\t\t %s\n", i , _grades._array[i].weight , 
				   _grades._array[i].name ,_grades._array[i].value); 

	}//End of for loop
	
}//End of list


//Evaluate the gpa Go through individual chars(ABCDF) and add their respective values to the sum
void evaluate(){

	int i , j , len  = _grades.count;
	double sum , avarage ,t_weight = 0;
	
	for(i = 0 ; i<len ; i++){
		double temp = 0;
		for(j=0 ; j<2 ; j++){

			if(_grades._array[i].value[j] == 'A')
			temp+= 2;
			if(_grades._array[i].value[j] == 'B')
			temp+=1.5;
			if(_grades._array[i].value[j] == 'C')
			temp+=1;
			if(_grades._array[i].value[j] == 'D')
			temp+=0.5;

		}//End of for-loop
		t_weight += _grades._array[i].weight;
		sum+= temp * _grades._array[i].weight;

	}//end of for-loop

	avarage = sum / t_weight;
	printf("Your gpa : %.2f\n" ,avarage);

}//End of evaluate

//Return the size of a grade element
unsigned int grade_size(grade g){

	return 12 + strlen(g.name);

}//End of grade_size


//Clears the list
void list_flush(){

	int i ;
	char* c = "0";

	for(i = 0 ; _grades.count != 0 ; i++){
		
		_remove(c);

	}//end of for

}//end of flush
