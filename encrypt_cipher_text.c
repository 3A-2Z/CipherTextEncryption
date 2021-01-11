#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_WORD_LENGTH 16
#define GNUM 149														  
void lowercase(char **tmp_line);										  //changes all leters within a word to lowercase
void size_check(char *****words_hold, int **curr_size, int **wc);		  //changes storage size to handle different document sizes
void read_exact(char **input, char filename[]);							  //reads string to exact space to avoid null values
void read_text(FILE *file, char ****words_hold, int *wc, int *curr_size); //read file lines

void secret_message(char **mesage_hold);								  //asks for message to encode
void char_instance(char ****words, int *wc, FILE **mess, char charectar); //finds charectar instance 
void encode(char ****words, int *wc);									  //encode message
void decode(char ****words, FILE **file);								  //decode message
void print_options();													  //prints options
void enter_encryption(FILE **f, char **input);							  //for encrypted text
void enter_cipher(FILE **f, char **input);								  //accepts cipher text passes it to main
void free_words(char ****words, int *wc);								  //to free words
void print_options()
{
	printf("enter a number 1-4 for options\n");
	printf("option1: enter cipher text\n");
	printf("option2: encode text\n");
	printf("option3: decode text\n");
	printf("option4: quit\n");
}
/*
 *reads cipher text name and store in variable input 
 */
void enter_cipher(FILE **f, char **input)
{
	char input_filename[100] = ""; //buffer for input filename

	printf("enter cipher text name:\n");
	char ***input_tmp = &input;
	read_exact(*input_tmp, input_filename);//reads file name and stores to char** input

	*f = fopen(*input, "r");
	if (*f == NULL)
	{
		printf("memmory error, cipher text file invalid\n");
		exit(1);
	}
	
	input_tmp = NULL;
}
void enter_encryption(FILE **f, char **input)
{
	char input_filename[100] = ""; //buffer for input filename

	printf("enter encrypted text name:\n");
	char ***input_tmp = &input;
	read_exact(*input_tmp, input_filename);
	*f = fopen(**input_tmp, "r");
	if (*f == NULL)
	{
		printf("memmory error,encrypted file is invalid\n");
		exit(1);
	}

	input_tmp = NULL;
}
/*
 *frees spaces allocated for words 
 */
void free_words(char ****words, int *wc)
{
	int i = 0;
	char **tmp_words = **words;
	for (i = 0; i < (*wc); i++)
	{
		free(tmp_words[i]);
		tmp_words[i] = NULL;
	}
}

void decode(char ****words, FILE **fout)
{
	char *decode = calloc(1500, sizeof(char));//variable to hold decoded message
	if(decode == NULL){
		printf("memory error, allocation to decode variable\n");
		exit(1);
	}
	int c = 0;
	char **tmp_words = **words;
	int decode_count = 0;
	int pair_count = 0;//keeps track of converted charectars to integers
	int char_count = 0;//keeps track of the number of charectars for integer conversion
	char tmp_char[] = {'\0', '\0', '\0', '\0'};//maximum 1500 charectars implys a max number of charectars per number representation to be 4
	int tmp_pair[] = {-1, -1};//hold cordinates as integers once the are converted
	int space = 0;
	while ((c = fgetc(*fout)) != EOF)
	{
		if((space>=1)&&(c==' '))//handles cases of continuos spaces
		{
			space++;
		}
		else
		{
		if (pair_count >= 2)//if the pair is filled, the charectar will be sought
		{	
			
			
			char char_tmp = tmp_words[tmp_pair[0]][tmp_pair[1]];//grabs charectar from word
			decode[decode_count++] = char_tmp;//charectar is stored in the secret message array
			tmp_pair[0] = -1;//sets as negative integers as means to check if cordinate has been stored
			tmp_pair[1] = -1;
			pair_count = 0;
			if (space >= 1)//to check if a space has been found
			{	
				int i;
				for(i=0;i<space;i++){
				decode[decode_count++] = ' ';//adds space after previous charectar conversion
				}
				space = 0;
			}
			
		}
		if (c == ' ')
		{

			
			if (tmp_char[0] != '#')
			{
				tmp_pair[pair_count++] = atoi(tmp_char);//converts charectar to integer if charectar exists
				tmp_char[0] = '\0';//resets tmp_char
				tmp_char[1] = '\0';
				tmp_char[2] = '\0';
				tmp_char[3] = '\0';
				char_count = 0;
				space = 1;
			}
			else if(tmp_char[0] == '#')
			{
				decode[decode_count++] = ' ';
				tmp_char[0] = '\0';
			}
			
		}
		if (c == ',')
		{
			if (tmp_char[0] != '#')
			{
				tmp_pair[pair_count++] = atoi(tmp_char);//converts charectar that exists befor comma
				tmp_char[0] = '\0';
				tmp_char[1] = '\0';
				tmp_char[2] = '\0';
				tmp_char[3] = '\0';
				char_count = 0;
			}
			else if(tmp_char[0] == '#' )
			{
				pair_count=0;
				tmp_char[0] = '\0';
				tmp_char[1] = '\0';
				tmp_char[2] = '\0';
				tmp_char[3] = '\0';
			}
			
		}
		if (isdigit(c))
		{
			tmp_char[char_count++] = c;//if the charectar has integer equivalent, it is stored as charectar to be converted to integer
		}
		if (c == '#')
		{

			decode[decode_count++] = '#';//adds unknown charectar to decode
			tmp_char[0] = '#';
		}
		}
	}
	tmp_pair[pair_count] = atoi(tmp_char);
	decode[decode_count++] = tmp_words[tmp_pair[0]][tmp_pair[1]];//converts leftover charectar

	printf("%s\n", decode);//prints decoded message to stdin
	free(decode);
	decode = NULL;

	tmp_words = NULL;
}
/*
 *encodes secret message by finding random charectar instance
 */
void encode(char ****words, int *wc)
{
	FILE *mess;
	char secret[100] = "";
	char* mess_name=NULL;//hold name for secret message text file
	printf("enter file name for secret message:");
	read_exact(&mess_name,secret); 
	mess = fopen(mess_name, "w+");
	int i = 0;
	char ***tmp_words = *words;

	char *tmp_message = calloc(1500, sizeof(char));
	if(tmp_message==NULL){
		printf("memmory allocation error with allocation to tmp_message variable\n");
		exit(1);
	}
	secret_message(&tmp_message);
	for (i = 0; i < (strlen(tmp_message) - 1); i++)
	{

		char_instance(&tmp_words, wc, &mess, tmp_message[i]);
		if (((tmp_message[i] != ' ') && (tmp_message[i + 1] != ' ')) && (tmp_message[strlen(tmp_message) - 2] != tmp_message[i]))
		{
			fseek(mess, 0, SEEK_CUR);
			fputc(',', mess);//writes comma to file only if the current and next elements within the secret message are not white space
		}
	}
	
	mess_name = NULL;
	free(tmp_message);
	tmp_message = NULL;

	tmp_words = NULL;
	fclose(mess);
}
/*
 *used to find charectar instance of a charectar within a word 
 * search words array and writes to encoding file
 */
void char_instance(char ****words_hold, int *wc, FILE **mess, char charectar)
{
	int instance = 0;

	int num = random() % 10;//choses random charectar instance between 0-9
	num++;
	
	char **tmp_words = **words_hold;

	FILE *tmp_file = *mess;
	if (tmp_file == NULL)
	{

		printf("memory error:encode file is not valid\n");
		exit(1);
	}

	int no_break = 10;//value assigned as way to break charectar search for charectar instances beyond thoose in the document
	while (no_break == 10)
	{
		int i, b;
		int boole = 0;//to break first for loop when charectar instance is not found in first search of word list
		for (i = 0; i < (*wc); i++)
		{
			if (boole == 1)
			{
				break;
			}

			for (b = 0; b < strlen(tmp_words[i]); b++)//loop searches word for charectar
			{
				if (tmp_words[i] != NULL)
				{
					if (tmp_words[i][b] == charectar)//increments number of charectar instances found
					{
						instance++;
					}
					if (charectar == 32)//if the chrectar is a white space , it will break all loops and write it file
					{

						fseek(tmp_file, 0, SEEK_CUR);//used to write charectar in next position 
						fputc(' ', tmp_file);
						boole = 1;
						instance = -1;
						no_break = -5;
						break;
					}
					if ((instance == num))//if the random number charectar instance is found 
					{

						char tmp[50];//stores coordinate string to be wriiten to file
						snprintf(tmp, sizeof(int) + 1, "%d", i);
						strcat(tmp, ",");
						char tmp2[50];
						snprintf(tmp2, sizeof(int) + 1, "%d", b);
						
						strcat(tmp, tmp2);
						int x;
						for (x = 0; x < (strlen(tmp)); x++)//writes individual charectars to ecoded text
						{
							fseek(tmp_file, 0, SEEK_CUR);
							fputc(tmp[x], tmp_file);
							
						}
						
						boole = 1;
						no_break = -5;
						break;
					}
				}
			}
		}
		if ((instance == 0))//if charectar is not found at all in first search, # will be written as it does not exist in file
		{

			fseek(tmp_file, 0, SEEK_CUR);
			fputc('#', tmp_file);
			boole = 1;

			no_break = -5;
		}
	}
	tmp_file = NULL;
	tmp_words = NULL;
}
/*
 * funstion reads in secret message to store in varible
 * for parsing
 */
void secret_message(char **message_tmp)
{
	char message[1500] = "";
	char *tmp = *message_tmp;

	printf("enter message:\n");
	fgets(message, 1500, stdin);
	strcpy(tmp, message);
	lowercase(&tmp);
	tmp = NULL;
}
/*
*converts all charectar in word to lowercae
*/
void lowercase(char **tmp_line)
{
	char *tmp = *tmp_line;
	int i = 0;
	while (tmp[i] != '\0')
	{
		tmp[i] = tolower(tmp[i]);
		i++;
	}
	tmp = NULL;
}
/*takes storage pointer and char array for file name 
 * cuts string to only store only charectars to null terminator
 * this allows fopen to read the file desired by user
 */
void read_exact(char **input, char filename[])
{
	int k = 0;			  //for stdin
	int string_count = 0; //counter for string
	while ((filename[string_count++] = fgetc(stdin)) != '\n')
	{

		if ((k == '\n') && (string_count != 1))
		{

			//stops at newline char to replace the charectar with null terminator
			break;
		}
	}

	*input = malloc(string_count * sizeof(char)); //allocates exact memmory for string
	
	strcpy(*input, filename);
	*(*input + (string_count - 1)) = '\0'; //sets last charectar to null terminator
}
/*
*read_text takes the cipher text and extracts the words, 
*allocating space within the 2nd dimension of the words array
*to hold the string 
*/
void read_text(FILE *file, char ****words_hold, int *wc, int *curr_size)
{
	char *line = NULL;
	ssize_t read = 0;
	size_t len = 0;
	char **tmp = NULL;
	tmp = **words_hold; //tmp var holds words list

	while ((read = getline(&line, &len, file)) != EOF)
	{

		char *tmp_line = NULL;
		tmp_line = calloc((read + 100), sizeof(char *));//allocates space for the string literal from vaiable line

		stpcpy(tmp_line, line);//string from variable line is now mutable for strtok
		lowercase(&tmp_line);

		char *n = strtok(tmp_line, " "); //tokenized line

		while (n != NULL)
		{

			if (strcmp("\n", n) != 0)//prevents a newline from becoming a word within words array
			{
				tmp[(*wc)] = calloc(MAX_WORD_LENGTH, sizeof(tmp[(*wc)])); //allocates memory for word
				if(tmp[(*wc)] == NULL){
					printf("memory allocation error for word number %d\n",(*wc));
					exit(1);
				}
				strcpy(tmp[(*wc)], n);//copy tokenized word int words array

				(*wc)++;//increases word count
			}
			n = strtok(NULL, " ");
		}

		free(n);
		n = NULL;

		free(tmp_line);
		tmp_line = NULL;

		free(line);
		line = NULL;
	}
	printf("word count %d\n",(*wc));
}

int main()
{
	srandom(GNUM);
	char **words = NULL;								//array containing all words
	int curr_size = 5000;								//current size of words dynamic array
	FILE *fd;											//decode file pointer
	words = (char **)calloc(curr_size, sizeof(char *)); //allocates size of word collection
	if(words == NULL){
		printf("memmory error to allocation of word array\n");
		exit(1);
	}
	char ***words_hold = &words;
	int wc = 0; //word counter

	char *input = NULL;//variable for cipher text name
	FILE *f = NULL;
	char *enc = NULL; //variable for encryption name

	char option[2];//stores option selected

	while ((option[0] != 'q') && (option[0] != 'Q'))
	{
		print_options();

		char p = 0;
		printf("enter option number:");
		while ((p = fgetc(stdin)) != '\n')//reads in option selected
		{
			option[0] = p;
		}

		

		switch (option[0])
		{
		case '1'://option to read in cipher text
			enter_cipher(&f, &input);
			read_text(f, &words_hold, &wc, &curr_size);

			break;
		case '2':
			if (f == NULL)//if a cipher text file has not been selected, the same funcions in option 1 will be called
			{
				enter_cipher(&f, &input);
				read_text(f, &words_hold, &wc, &curr_size);
			}
			encode(&words_hold, &wc);//will encode secret message

			break;
		case '3':
			if (f == NULL)//if a cipher text file has not been selected, the same funcions in option 1 will be called
			{
				enter_cipher(&f, &input);
				read_text(f, &words_hold, &wc, &curr_size);
			}
			enter_encryption(&fd, &enc);//to read encrypted file
			decode(&words_hold, &fd);//to decode encrypted file and print to stdin
			fclose(fd);//closes encrypted file
			break;
		case '4':
			option[0] = 'q';
			break;
		default:
			printf("invalid choice (enter 1,2,3, or 4),program terminated\n");
			option[0] = 'q';
		}
	}

	free(enc);
	free(input);
	if(words_hold!=NULL)
	{
		free_words(&words_hold, &wc);//prevents seg fault if user goes straight to option4 or types invalid charectar
	}
	words_hold = NULL;
	free(words);
	if(f != NULL)//prevents seg fault if user goes straight to option 4 or types invalid charectar
	{
		fclose(f);
	}
	return 0;
}
