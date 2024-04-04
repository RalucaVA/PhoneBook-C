#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

#define CSV_FILE_PATH           "phoneBook.csv"
#define MAX_STRING_LEN          50

struct Contact {
    char *name;
    char *phoneNumber;
    char *emailAddress;
    char *comment;
};
int contactNumber;
struct Contact *phoneBook;

char* make_copy(char * array){
    if(array==NULL){
        return NULL;
    }
    
    char* destination;
    destination = malloc(strlen(array) + 1);
    strcpy(destination, array);
    return destination;
}

struct Contact make_struct(char *name, char *phoneNumber, char *email, char *comment)
{
        struct Contact new;
        new.name=make_copy(name);
        new.phoneNumber=make_copy(phoneNumber);
        new.emailAddress=make_copy(email);
        new.comment=make_copy(comment);     
        return new;   
}

struct Contact emptyContact()
{
        struct Contact new;
        new.name=make_copy("");
        new.phoneNumber=make_copy("");
        new.emailAddress=make_copy("");
        new.comment=make_copy("");     
        return new;   
}

bool isEmptyContact(struct Contact c)
{
        if (*c.name == 0 && *c.phoneNumber == 0 && *c.emailAddress == 0 && *c.comment == 0)
        {
                return true;
        }
        return false;
}

struct Contact *readFromFile()
{
        //read char by char and if ';' is found that means that a word is finished and if '\n' is found this means that a line is finished 
        struct Contact* phoneBook= malloc(sizeof(struct Contact) * 100);
        struct Contact item;
        FILE *pfinput;
        unsigned int currentLine, currentWord, lenLongestValue; 
        char *tempWord;
        int c;
        unsigned int currentCharPos; // current char position
        lenLongestValue = 50;  //the max length of a contact name or email address.
        tempWord = (char*)malloc(lenLongestValue);
        if(pfinput = fopen(CSV_FILE_PATH, "r")) {   //open file to read
                rewind(pfinput);   //sets the file position to the beginning of the file
                currentLine = 0;
                currentWord = 0;
                currentCharPos = 0;
                while((c = fgetc(pfinput)) != EOF)
                {
                        switch(c)
                        {
                                case ';':
                                    tempWord[currentCharPos] = '\0';                                    
                                    switch(currentWord){
                                        case 0: item.name = make_copy(tempWord); break;
                                        case 1: item.phoneNumber = make_copy(tempWord); break;
                                        case 2: item.emailAddress =  make_copy(tempWord); break;
                                        case 3: {
                                                        item.comment =  make_copy(tempWord); 
                                                        phoneBook[currentLine] = item; 
                                                        break;
                                                }
                                    }
                                    currentCharPos = 0;   
                                    currentWord++;                             
                                    break;
                                case '\n':                            
                                    currentWord = 0;
                                    currentCharPos = 0;        
                                    if (tempWord != NULL && tempWord[0] != '\0' && tempWord[0] != '\r')   //ignore empty lines
                                        currentLine++;
                                    tempWord = make_copy("");
                                    break;
                                default:
                                    tempWord[currentCharPos] = c;
                                    currentCharPos++;
                                    break;
                        }
                }

                fclose(pfinput);
        }
        else
        {
                printf("File not found!\n");
        }
        contactNumber = currentLine;
        return phoneBook;
}

void writeInFile(struct Contact newContact, char *fileName)
{
        FILE *pfinput;
        if(pfinput = fopen(fileName, "a")) {   //open file to append
                //build string to write in the file
                fprintf(pfinput, "\n%s; %s; %s; %s;", newContact.name, newContact.phoneNumber, newContact.emailAddress, newContact.comment);
                fclose(pfinput);
        }
        else
        {
                printf("File not found!\n");
        }
}

void addNewContact()
{
        char newName[MAX_STRING_LEN];
        char phoneNumber[MAX_STRING_LEN];
        char email[MAX_STRING_LEN];
        char comment[MAX_STRING_LEN];                

        printf("New contact:");
        printf("\nName:");
        scanf("%s", newName);  
        printf("Phone number:");
        scanf("%s", phoneNumber);
        printf("Email address:");
        scanf("%s", email);
        printf("Comment:");        
        scanf("%s", comment);
        struct Contact newContactToAdd = make_struct(newName, phoneNumber, email, comment);
        writeInFile(newContactToAdd, CSV_FILE_PATH);
        contactNumber++;
}

void deleteContact(char * name)
{
        struct Contact *phoneBook;
        phoneBook = readFromFile();
        bool contactFound = false;
        for (int i=0; i <= contactNumber; i++)
        {
                if (strstr(phoneBook[i].name, name)) 
                {  
                        //found
                        phoneBook[i].name = make_copy("");
                        phoneBook[i].phoneNumber = make_copy("");
                        phoneBook[i].emailAddress = make_copy("");
                        phoneBook[i].comment = make_copy("");
                        contactFound = true;
                }
        }
        if (!contactFound)
        {
                printf("Name <%s> doesn't exist in phone book!\n", name);
                return;
        }
        //create a temporary file
        for (int i=0; i <= contactNumber; i++)
        {
                if (*phoneBook[i].name != 0) 
                {  
                        writeInFile(phoneBook[i], "_temp.csv");          
                }
        }
        //remove the original file
        remove(CSV_FILE_PATH);
        //rename the temporary file to original name
        rename("_temp.csv", CSV_FILE_PATH);
        contactNumber--;
        printf("Name <%s> was deleted!\n", name);
}

void updateContact(char * name)
{
        struct Contact *phoneBook;
        phoneBook = readFromFile();
        int fieldOption;
        char newName[MAX_STRING_LEN];
        char newPhoneNumber[MAX_STRING_LEN];
        char newEmailAddress[MAX_STRING_LEN];
        char newComment[MAX_STRING_LEN];    
        bool contactFound = false;
        for (int i=0; i <= contactNumber; i++)
        {
                if (strcmp(phoneBook[i].name, name) ==0) 
                {  
                        contactFound = true;
                        printf("Choose the corresponding number of the field to modify:\n");
                        printf("1. Name\n");
                        printf("2. Phone number\n");
                        printf("3. Email address\n");
                        printf("4. Comment\n");
                        scanf("%d", &fieldOption); 
                        if (fieldOption != 1 && fieldOption != 2 && fieldOption != 3 && fieldOption != 4)
                        {
                                printf("Field option invalid!\n");                                
                                return;
                        }
                        struct Contact newPerson;
                        switch (fieldOption)
                        {
                        case 1:
                                printf("New name(the old one is <%s>): ", phoneBook[i].name ); 
                                scanf("%s", newName); 
                                newPerson = make_struct(newName, phoneBook[i].phoneNumber, phoneBook[i].emailAddress, phoneBook[i].comment);
                                break;
                        case 2:
                                printf("New phone number(the old one is <%s>): ", phoneBook[i].phoneNumber ); 
                                scanf("%s", newPhoneNumber); 
                                newPerson = make_struct(phoneBook[i].name, newPhoneNumber, phoneBook[i].emailAddress, phoneBook[i].comment);
                                break;
                        case 3:
                                printf("New email address (the old one is <%s>): ", phoneBook[i].emailAddress ); 
                                scanf("%s", newEmailAddress); 
                                newPerson = make_struct(phoneBook[i].name, phoneBook[i].phoneNumber, newEmailAddress, phoneBook[i].comment);
                                break;
                        case 4:
                                printf("New comment (the old one is <%s>): ", phoneBook[i].comment ); 
                                scanf("%s", newComment);  
                                newPerson = make_struct(phoneBook[i].name, phoneBook[i].phoneNumber, phoneBook[i].emailAddress, newComment);
                                break;                                                                
                        default:
                                break;
                        }                       
                        
                        phoneBook[i] = newPerson;
                }
        }
        if (!contactFound)
        {
                printf("Name <%s> doesn't exist in phone book!\n", name);
                return;
        }
        //create a temporary file
        for (int i=0; i <= contactNumber; i++)
        {
                if (*phoneBook[i].name != 0) 
                {  
                        writeInFile(phoneBook[i], "_temp.csv");          
                }
        }
        //remove the original file
        remove(CSV_FILE_PATH);
        //rename the temporary file to original name
        rename("_temp.csv", CSV_FILE_PATH);
        contactNumber--;
        printf("Update successfully done!\n");
}

struct Contact searchContactByName(char * name)
{
        struct Contact *phoneBook;
        phoneBook = readFromFile();
        for (int i=0; i <= contactNumber; i++)
        {
                if (strstr(phoneBook[i].name, name)) 
                {  
                        return phoneBook[i];
                }
        }
        return emptyContact();
}

struct Contact searchContactByPhoneNumber(char * phoneNumber)
{
        struct Contact *phoneBook;
        phoneBook = readFromFile();
        for (int i=0; i <= contactNumber; i++)
        {
                if (strstr(phoneBook[i].phoneNumber, phoneNumber)) 
                {  
                        return phoneBook[i];
                }
        }     
        return emptyContact();
}

void printContact(struct Contact c)
{
        printf("Name=%s; Phone Number=%s, Email=%s, Comment=%s\n", 
                c.name, c.phoneNumber, c.emailAddress, c.comment);        
}

void printPhoneBook()
{
        struct Contact *phoneBook;
        phoneBook = readFromFile();
        for (int i=0; i <= contactNumber; i++)
        {
                printContact(phoneBook[i]);
        }
}

int main()
{
    int choice;
    char * name;
    char * phoneNumber;
    struct Contact newContact;

    printf("Please choose an option!\n\n");
    printf("1. Add a new contact\n");
    printf("2. Update a contact\n");
    printf("3. Delete a contact\n");
    printf("4. Search a contact by name\n");
    printf("5. Search a contact by phone\n");
    printf("6. Print the phone book\n");
    printf("7. Exit\n\n");

    do {

        scanf("%d", &choice);
        switch (choice) {
            case 1: addNewContact();
                    break;
            case 2: printf("Contact name to update: ");
                    scanf("%s", name); 
                    updateContact(name);
                    break;
            case 3: printf("Contact name to delete: ");
                    scanf("%s", name); 
                    deleteContact(name);
                    break;      
            case 4: printf("Contact name to search: ");
                    scanf("%s", name);  
                    struct Contact contact = searchContactByName(name);
                    if(!isEmptyContact(contact))
                    {
                        printContact(contact);
                    }
                    else
                    {
                        printf("Name <%s> doesn't exist in phone book!\n", name);
                    }
                    break;
            case 5: printf("Contact phone number to search: ");
                    scanf("%s", phoneNumber);  
                    struct Contact contactFound = searchContactByPhoneNumber(phoneNumber);
                    if(!isEmptyContact(contactFound))
                    {
                        printContact(contactFound);
                    }
                    else
                    {
                        printf("Phone number <%s> doesn't exist in phone book!\n", phoneNumber);
                    }
                    break; 
            case 6: printf("Phone book content is:\n");  
                    printPhoneBook();
                    printf("\n");
                    break;           
            case 7: printf("Exit!");           
                    break;   
            default: printf("Invalid choice!\n");
                     break;                   
        } 
        if (choice !=7) 
                printf("Please choose a new option!\n\n");
    } while (choice !=7);
    return 0;    
}