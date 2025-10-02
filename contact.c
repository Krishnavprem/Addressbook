#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
#include <ctype.h>

void listContacts(AddressBook *addressBook, int sortCriteria) 
{
    // Sort contacts based on the chosen criteria, ie name->phone->email
    int i;
    if(addressBook->contactCount==0){
        printf("\033[1;31mNo contact available to be displayed\033[0m\n");
        return;
    }
    // --- Sort contacts alphabetically by name ---
    for(i=0;i<addressBook->contactCount - 1;i++){
        for(int j = i+1; j < addressBook -> contactCount ;j++){
            if(strcasecmp(addressBook->contacts[i].name,addressBook->contacts[j].name) > 0){
                //--Swap contacts--
                Contact temp = addressBook->contacts[i];
                addressBook->contacts[i]= addressBook->contacts[j];
                addressBook->contacts[j] = temp;
            }
        }
    }
    printf("\n%-30s %-30s %-30s\n", "Name", "Phone", "Email");
    printf("-----------------------------------------------------------------------------------------\n");

    // Print each contact in tabular format
    for(int i = 0; i < addressBook->contactCount; i++) {
        printf("%-30s %-30s %-30s\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
    printf("\n");
} 

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    //loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
    Contact newcontact;
    int isvalid;
    int i;
    if (addressBook->contactCount >= MAX_CONTACTS){
        printf("\033[1;31mSorry, Address book is full, Cannot add new contact\033[0m\n");
        return;
    }
    do{
        isvalid =1;
        printf("Enter the name: ");
        scanf(" %49[^\n]",newcontact.name);
        
        int length=strlen(newcontact.name);
        if (newcontact.name[0]=='.'){
            isvalid=0; 
        }
        //checking to the length of string if all the entered characters are valid or not
        for(i=0;newcontact.name[i] != '\0';i++){
            if (!((newcontact.name[i]>='a'&& newcontact.name[i]<='z') 
                ||(newcontact.name[i]>='A'&& newcontact.name[i]<='Z') 
                || (newcontact.name[i] ==' '|| newcontact.name[i] =='.'))){
                isvalid =0;
                break;
            }

        }
        if (isvalid==0){
            printf("\033[1;31mInvalid contact name,Please enter valid name\033[0m\n");
        }
    }while(!isvalid);

    do{
        isvalid =1;
        int length;
        printf("Enter phone number: ");
        scanf(" %19[^\n]",newcontact.phone);
        
        length = strlen(newcontact.phone);
        //phone number validation is done
        if(length !=10){
            isvalid=0;
        }
        else if (newcontact.phone[0]=='0'){
            isvalid=0;
        }
        else{
            for(i=0;i<length;i++){
                if (!isdigit((unsigned char)newcontact.phone[i] )){
                    isvalid=0;
                    break;
                }
            }
        }
        //checking if number already exists
        if (isvalid) {
            for (i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(addressBook->contacts[i].phone, newcontact.phone) == 0) {
                    printf("This phone number already exists for contact: %s\n", addressBook->contacts[i].name);
                    isvalid = 0;
                    break;
                }
            }
        }
        if(isvalid==0){
            printf("\033[1;31mPlease enter a valid contact number\033[0m\n");
        }
    }while(!isvalid);


do{
        isvalid = 1;
        printf("Enter email: ");
        scanf(" %49[^\n]", newcontact.email);
        
        int length = strlen(newcontact.email);  // FIXED: Calculate length AFTER input
        
        // First check: validate allowed characters and basic rules
        for(i = 0; newcontact.email[i] != '\0'; i++){
            if (!((newcontact.email[i] >= 'a' && newcontact.email[i] <= 'z') || 
                (newcontact.email[i] >= '0' && newcontact.email[i] <= '9') || 
                newcontact.email[i] == '_' || newcontact.email[i] == '@' || newcontact.email[i] == '.')) {
                isvalid = 0;
                break;
            }
            
            // Check if email starts with @
            if(newcontact.email[i] == '@' && i == 0){
                isvalid = 0;
                break;
            }    
            
            // Check if email ends with .
            if(newcontact.email[i] == '.' && newcontact.email[i+1] == '\0'){
                isvalid = 0;
                break;
            }
        }
        if(isvalid){
            int at = 0, dotafterat = 0, atpos = -1;
            
            // Check for @ and . present or not
            for(i = 0; newcontact.email[i] != '\0'; i++){
                if(newcontact.email[i] == '@'){
                    if(at == 1){  // Multiple @ found
                        isvalid = 0;
                        break;
                    }
                    at = 1;
                    atpos = i;
                }
                
                // Check if . is present after @
                if(at && newcontact.email[i] == '.'){
                    dotafterat = 1;
                }
            }
            if(!at || !dotafterat){
                isvalid = 0;
            }
            
            // Check if email ends with ".com"
            if(isvalid && dotafterat){
                if(length < 4){
                    isvalid = 0;
                }
                else{
                    int k = length - 4;
                    if(newcontact.email[k] != '.' || newcontact.email[k+1] != 'c'
                        || newcontact.email[k+2] != 'o' || newcontact.email[k+3] != 'm'){
                        isvalid = 0;
                    } 
                }
            }
        }
        //checking if email already exists
        if (isvalid) {
            for (i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(addressBook->contacts[i].email, newcontact.email) == 0) {
                    printf("This email already exists for contact: %s\n", addressBook->contacts[i].name);
                    isvalid = 0;
                    break;
                }
            }
        }
        
        if(isvalid == 0){
            printf("\033[1;31mEnter a valid email id\033[0m\n");
        }
    }while(!isvalid);
    //if all validations are done then the contact is saved to addressbook
    addressBook->contacts[addressBook->contactCount++] = newcontact;
    printf("Contact added successfully!\n");
}

void searchContact(AddressBook *addressBook) 
{
    if (addressBook->contactCount == 0) {
        printf("\033[1;31mNo contacts available to search.\033[0m\n");
        return;
    }
    int choice;
    char str[50];
    char c;
    int i;
    int isvalid=1;
    while(1){
        printf("\nSearch by:\n");
        printf("1. Name\n");
        printf("2. Phone Number\n");
        printf("3. Email\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d%c", &choice,&c) !=2 || c !='\n') {
            printf("\033[1;31mInvalid choice\033[0m\n"); 
            while (getchar() != '\n'); 
            continue;
        }
        if (choice==4){
            return;
        }
        if (choice<1 || choice>4){
            printf("\033[1;31mInvalid choice\033[0m\n");
            continue;
        }
        if(choice==1){
            do{
                isvalid=1;
                printf("Enter the name to be searched");
                scanf(" %49[^\n]",str);
                int length=strlen(str);
                if (length==0){
                    isvalid=0;
                }
                if (str[0]=='.'){
                    isvalid=0; 
                }
                //checking to the length of string if all the entered characters are valid or not
                for(i=0;str[i] != '\0';i++){
                    if (!((str[i]>='a'&& str[i]<='z') 
                        ||(str[i]>='A'&& str[i]<='Z') 
                        || (str[i] ==' '|| str[i] =='.'))){
                        isvalid =0;
                        break;
                    }
                }
                if(!isvalid){
                    printf("\033[1;31mEnter a valid name\033[0m\n");
                }
            }while(!isvalid);
            int count = 0, index[50];
            for (i = 0; i < addressBook->contactCount; i++) {
                int len = strlen(str);
                for (int pos = 0; pos <= (int)strlen(addressBook->contacts[i].name) - len; pos++) {
                    if (strncasecmp(&addressBook->contacts[i].name[pos], str, len) == 0) {
                        index[count++] = i;
                        break; // stop after finding first match in this name
                    }
                }
            }

            if(count == 0){
                printf("\033[1;31mContact not found\033[0m\n");
            }
            else {
                for(int j=0; j<count; j++){
                    int id = index[j];
                    printf("\n--CONTACT FOUND--\n");
                    printf("%d. ", id+1);
                    printf("Name: %s\n", addressBook->contacts[id].name);
                    printf("Phone: %s\n", addressBook->contacts[id].phone);
                    printf("Email: %s\n", addressBook->contacts[id].email);
                }
            }
        }
        if (choice ==2){
            do{
                isvalid=1;
                printf("Enter the phone number to be searched: ");
                scanf(" %19[^\n]",str);
                int length=strlen(str);
                //phone number validations
                if (length != 10){
                    isvalid=0;
                }
                //check if number is valid , only digits
                for(i=0;i<length;i++){
                    if(!isdigit((unsigned char)str[i])){
                        isvalid=0;
                        break;
                    }
                }
                if(isvalid==0){
                    printf("\033[1;31mEnter a valid phone number\033[0m\n");
                }
            }
            while(!isvalid);
            int count = 0, index[50];
            for (i = 0; i < addressBook->contactCount; i++) {
                if (strcasecmp(addressBook->contacts[i].phone, str) == 0) {
                    index[count++] = i;
                }
            }

            if (count == 0) {
                printf("\033[1;31mContact not found\033[0m\n");
            } else {
                for (int j = 0; j < count; j++) {
                    int id = index[j];
                    printf("\n--CONTACT FOUND--\n");
                    printf("%d. ", id + 1);
                    printf("Name: %s\n", addressBook->contacts[id].name);
                    printf("Phone: %s\n", addressBook->contacts[id].phone);
                    printf("Email: %s\n", addressBook->contacts[id].email);
                }
            }
        }
        if (choice==3){
            do{
                isvalid=1;
                printf("Enter the email to be searched: ");
                scanf(" %49[^\n]",str);
                
                int length = strlen(str);
                
                int at = 0, dotafterat = 0,atpos=-1;
                
                // Check for @ and . present or not
                for(i = 0; i<length; i++){
                    if (!((str[i] >= 'a' && str[i] <= 'z') || 
                    (str[i] >= '0' && str[i] <= '9') || 
                    str[i] == '_' || str[i] == '@' || str[i] == '.')) {
                    isvalid = 0;
                    break;
                    }
                    if(str[i] == '@'){
                        if(at == 1){  // Multiple @ found
                            isvalid = 0;
                            break;
                        }
                        at = 1;
                        atpos = i;
                    }
                    
                    // Check if . is present after @
                    if(at && str[i] == '.'){
                        dotafterat = 1;
                    }
                }
                if(!at || !dotafterat){
                    isvalid = 0;
                }
                
                // Check if email ends with ".com"
                if(isvalid && length>=4){
                    int k=length-4;
                    if(str[k] != '.' || str[k+1] != 'c'
                            || str[k+2] != 'o' || str[k+3] != 'm'){
                        isvalid = 0;
                    }
                }
                if(isvalid == 0){
                    printf("\033[1;31mEnter a valid email id\033[0m\n");
                }
                
            }while(!isvalid);
            int count = 0, index[50];
            for (i = 0; i < addressBook->contactCount; i++) {
                if (strcasecmp(addressBook->contacts[i].email, str) == 0) {
                    index[count++] = i;
                }
            }

            if (count == 0) {
                printf("\033[1;31mContact not found\033[0m\n");
            } else {
                for (int j = 0; j < count; j++) {
                    int id = index[j];
                    printf("\n--CONTACT FOUND--\n");
                    printf("%d. ", id + 1);
                    printf("Name: %s\n", addressBook->contacts[id].name);
                    printf("Phone: %s\n", addressBook->contacts[id].phone);
                    printf("Email: %s\n", addressBook->contacts[id].email);
                }
            }
        }
    }
}

void editContact(AddressBook *addressBook)
{
    int isvalid;
    int editted=0;
	if(addressBook->contactCount==0){
        printf("\033[1;31mNo contact available to search\033[0m\n");
        return; 
    }
    char name[50];
    int i,count,index[MAX_CONTACTS],choice;
    do{
        isvalid=1;
        count=0;
        printf("Enter the name of the contact to edit: ");
        scanf(" %49[^\n]",name);
        if (strlen(name)==0){
            isvalid=0;
        }
        if (name[0]=='.'){
                isvalid=0; 
            }
        //checking to the length of string if all the entered characters are valid or not
        for(i=0;name[i] != '\0';i++){
            if (!((name[i]>='a'&& name[i]<='z') 
                ||(name[i]>='A'&& name[i]<='Z') 
                || (name[i] ==' '|| name[i] =='.'))){
                isvalid =0;
                break;
            }
        }
        if(!isvalid){
            printf("\033[1;31mEnter a valid name\033[0m\n");
            continue;
        }
        for (i = 0; i < addressBook->contactCount; i++) {
            int len = strlen(name);
            for (int pos = 0; pos <= (int)strlen(addressBook->contacts[i].name) - len; pos++) {
                if (strncasecmp(&addressBook->contacts[i].name[pos], name, len) == 0) {
                    index[count++] = i;
                    break; // stop after finding first match in this name
                }
            }
        }
        if(count==0){
            printf("\033[1;31mNo contact found on the same name\033[0m\n");
            return;
        }
        break;
    }while(!isvalid);
        int idx;
        if(count == 1){
            idx=index[0];//the only one contact present get editted 
        }
        else{
            printf("--Multiple contacts found--");
            for (i = 0; i < count; i++) {
                int id = index[i];
                printf("\n%d.\n", i + 1);
                printf("Name : %s\n", addressBook->contacts[id].name);
                printf("Phone: %s\n", addressBook->contacts[id].phone);
                printf("Email: %s\n", addressBook->contacts[id].email);
            }
            int select;
            printf("Select the index of the contact");
            scanf("%d",&select);
            if (select < 1 || select > count) {
                printf("\033[1;31mInvalid choice.\033[0m\n");
                return;
            }
            idx=index[select-1];
            printf("Contact selected");
            printf("%d. ",select);
            printf("Name %s\n",addressBook->contacts[idx].name);
            printf("Phone: %s\n", addressBook->contacts[idx].phone);
            printf("Email: %s\n", addressBook->contacts[idx].email);
        }
      
    int option;
    printf("\nWhat do you wish to edit the contact by\n");
    printf("1. name\n");
    printf("2. phone\n");
    printf("3. email\n");
    printf("4. all\n");
    printf("Enter the choice: ");
    if (scanf("%d", &option) != 1) {
            printf("\033[1;31mPlease enter a valid choice\033[0m\n");
            while (getchar() != '\n'); 
            return;
        }
    while (getchar() != '\n');

    if(option<1 || option>4){
        return;
    }
    char temp[50];
    if (option == 1){
        do{
            isvalid=1;
            printf("Enter the new name: ");
            scanf(" %49[^\n]",temp);
            int length=strlen(temp);
            if (temp[0]=='.'){
                isvalid=0;
            }
            if(length==0){
                isvalid=0;
            }
            //checking to the length of string if all the entered characters are valid or not
            for(i=0;temp[i] != '\0';i++){
                if (!((temp[i]>='a'&& temp[i]<='z') 
                    ||(temp[i]>='A'&& temp[i]<='Z') 
                    || (temp[i] ==' '|| temp[i] =='.'))){
                    isvalid =0;
                    break;
                }

            }
            if (isvalid==0){
                printf("\033[1;31mInvalid contact name,Please enter valid name\033[0m\n");
            }
        }while(!isvalid);
        if(isvalid==1){
            strcpy(addressBook->contacts[idx].name,temp);
            editted=1;
        }
    }
    if (option == 2){
        do{
            printf("Enter the new phone: ");
            scanf(" %49[^\n]",temp);
            isvalid=1;
            int length = strlen(temp);
            //phone number validation
            if(length !=10){
                isvalid=0;
            }
            else if(temp[0]=='0'){
                isvalid=0;
            }
            else{
                for(i=0;i<length;i++){
                    if (!isdigit((unsigned char)temp[i] )){
                        isvalid=0;
                        break;
                    }
                }
            }
            //checking if number already exists
            if (isvalid) {
                for (i = 0; i < addressBook->contactCount; i++) {
                    if (strcmp(addressBook->contacts[i].phone, temp) == 0) {
                        printf("\033[1;31mThis phone number already exists for contact: %s\033[0m\n", addressBook->contacts[i].name);
                        isvalid = 0;
                        break;
                    }
                }
            }
            if(isvalid==0){
                printf("\033[1;31mPlease enter a valid contact number\033[0m\n");
            }
        }while(!isvalid);
        if(isvalid==1){
            strcpy(addressBook->contacts[idx].phone,temp);
            editted=1;
        }
    }
    if (option == 3){
        do{
            isvalid=1;
            printf("Enter the new email: ");
            scanf(" %[^\n]",temp);
            
            int length = strlen(temp);  
            
            // check for validations
            for(i = 0; temp[i] != '\0'; i++){
                if (!((temp[i] >= 'a' && temp[i] <= 'z') || 
                    (temp[i] >= '0' && temp[i] <= '9') || 
                    temp[i] == '_' || temp[i] == '@' || temp[i] == '.')) {
                    isvalid = 0;
                    break;
                }
                
                // Check if email starts with @
                if(temp[i] == '@' && i == 0){
                    isvalid = 0;
                    break;
                }    
                
                // Check if email ends with .
                if(temp[i] == '.' && temp[i+1] == '\0'){
                    isvalid = 0;
                    break;
                }
            }
            if(isvalid){
                int at = 0, dotafterat = 0, atpos = -1;
                
                // Check for @ and . present or not
                for(i = 0; temp[i] != '\0'; i++){
                    if(temp[i] == '@'){
                        if(at == 1){  // Multiple @ found
                            isvalid = 0;
                            break;
                        }
                        at = 1;
                        atpos = i;
                    }
                    
                    // Check if . is present after @
                    if(at && temp[i] == '.'){
                        dotafterat = 1;
                    }
                }
                if(!at || !dotafterat){
                    isvalid = 0;
                }
                
                // Check if email ends with ".com"
                if(isvalid && dotafterat){
                    if(length < 4){
                        isvalid = 0;
                    }
                    else{
                        int k = length - 4;
                        if(temp[k] != '.' || temp[k+1] != 'c'
                            || temp[k+2] != 'o' || temp[k+3] != 'm'){
                            isvalid = 0;
                        } 
                    }
                }
            }
            //checking if email already exists
            if (isvalid) {
                for (i = 0; i < addressBook->contactCount; i++) {
                    if (strcmp(addressBook->contacts[i].email, temp) == 0) {
                        printf("\033[1;31mThis email already exists for contact: %s\033[0m\n", addressBook->contacts[i].name);
                        isvalid = 0;
                        break;
                    }
                }
            }
            
            if(isvalid == 0){
                printf("\033[1;31mEnter a valid email id\033[0m\n");
            }
        }while(!isvalid);
        if(isvalid==1){
            strcpy(addressBook->contacts[idx].email,temp);
            editted=1;
        } 
    }
    if (option == 4){
        do{
            isvalid=1;
            printf("Enter the new name: ");
            scanf(" %[^\n]",temp);
            int length=strlen(temp);
            //checking to the length of string if all the entered characters are valid or not
            for(i=0;temp[i] != '\0';i++){
                if (!((temp[i]>='a'&& temp[i]<='z') 
                    ||(temp[i]>='A'&& temp[i]<='Z') 
                    || (temp[i] ==' '|| temp[i] =='.'))){
                    isvalid =0;
                    break;
                }

            }
            if (isvalid==0){
                printf("\033[1;31mInvalid contact name,Please enter valid name\033[0m\n");
            }
        }while(!isvalid);
        if(isvalid==1){
            strcpy(addressBook->contacts[idx].name,temp);
            editted=1;
        }
        do{
            isvalid=1;
            printf("Enter the new phone: ");
            scanf(" %[^\n]",temp);
            int length = strlen(temp);
            if(length !=10){
                isvalid=0;
            }
            else{
                for(i=0;i<length;i++){
                    if (!isdigit((unsigned char)temp[i] )){
                        isvalid=0;
                        break;
                    }
                }
            }
            //checking if number already exists
            if (isvalid) {
                for (i = 0; i < addressBook->contactCount; i++) {
                    if (strcmp(addressBook->contacts[i].phone, temp) == 0) {
                        printf("\033[1;31mThis phone number already exists for contact: %s\033[0m\n", addressBook->contacts[i].name);
                        isvalid = 0;
                        break;
                    }
                }
            }
            if(isvalid==0){
                printf("\033[1;31mPlease enter a valid contact number\033[0m\n");
            }
        }while(!isvalid);

        if(isvalid==1){
            strcpy(addressBook->contacts[idx].phone,temp);
            editted=1;
        }
        do{
            isvalid=1;
            printf("Enter the new email: ");
            scanf(" %[^\n]",temp);
            
            int length = strlen(temp);  
            
            // check for validations
            for(i = 0; temp[i] != '\0'; i++){
                if (!((temp[i] >= 'a' && temp[i] <= 'z') || 
                    (temp[i] >= '0' && temp[i] <= '9') || 
                    temp[i] == '_' || temp[i] == '@' || temp[i] == '.')) {
                    isvalid = 0;
                    break;
                }
                
                // Check if email starts with @
                if(temp[i] == '@' && i == 0){
                    isvalid = 0;
                    break;
                }    
                
                // Check if email ends with .
                if(temp[i] == '.' && temp[i+1] == '\0'){
                    isvalid = 0;
                    break;
                }
            }
            if(isvalid){
                int at = 0, dotafterat = 0, atpos = -1;
                
                // Check for @ and . present or not
                for(i = 0; temp[i] != '\0'; i++){
                    if(temp[i] == '@'){
                        if(at == 1){  // Multiple @ found
                            isvalid = 0;
                            break;
                        }
                        at = 1;
                        atpos = i;
                    }
                    
                    // Check if . is present after @
                    if(at && temp[i] == '.'){
                        dotafterat = 1;
                    }
                }
                if(!at || !dotafterat){
                    isvalid = 0;
                }
                
                // Check if email ends with ".com"
                if(isvalid && dotafterat){
                    if(length < 4){
                        isvalid = 0;
                    }
                    else{
                        int k = length - 4;
                        if(temp[k] != '.' || temp[k+1] != 'c'
                            || temp[k+2] != 'o' || temp[k+3] != 'm'){
                            isvalid = 0;
                        } 
                    }
                }
            }
            //checking if email already exists
            if (isvalid) {
                for (i = 0; i < addressBook->contactCount; i++) {
                    if (strcmp(addressBook->contacts[i].email, temp) == 0) {
                        printf("\033[1;31mThis email already exists for contact: %s\033[0m\n", addressBook->contacts[i].name);
                        isvalid = 0;
                        break;
                    }
                }
            }
            
            if(isvalid == 0){
                printf("\033[1;31mEnter a valid email id\033[0m\n");
            }
        }while(!isvalid);
        if(isvalid==1){
            strcpy(addressBook->contacts[idx].email,temp);
            editted=1;
        } 
    }
    if(editted==1){
        printf("Contact edited successfully!"); 
    }
    else{
        printf("No changes made.\n");
    }
}

void deleteContact(AddressBook *addressBook)
{
    if(addressBook->contactCount==0){
        printf("\033[1;31mNo contacts available to delete.\033[0m\n");
        return; 
    }
    char temp[50];
    int count=0;
    int choice,match[50];
    printf("Search the contact by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. email\n");
    printf("Enter the choice: ");
    if (scanf("%d", &choice) != 1) {
            printf("\033[1;31mPlease enter a valid choice\033[0m\n");
            while (getchar() != '\n'); 
            return;
        }
    if (choice<1 || choice>3){
        printf("\033[1;31mInvalid choice\033[0m\n");
        return;
    }
    int isvalid;
    do{
        isvalid=1;
        printf("Enter the value to be searched: "); 
        scanf(" %49[^\n]",temp);
        int length=strlen(temp);
        if(choice == 1){
            int i;
            //checking to the length of string if all the entered characters are valid or not
            if (length==0){
                isvalid=0;
            }
            for(i=0;temp[i] != '\0';i++){
                if (!((temp[i]>='a'&& temp[i]<='z') 
                    ||(temp[i]>='A'&& temp[i]<='Z') 
                    || (temp[i] ==' '|| temp[i] =='.'))){
                    isvalid =0;
                    break;
                }
            }
        }
        else if(choice==2){
            int i;
            if(length!=10){
                isvalid=0;
            }
            for(i=0;i<length;i++){
                if(!isdigit((unsigned char)temp[i])){
                    isvalid=0;
                    break;
                }
            }
        }
        else if (choice == 3) {  
            // Email validation
            int len = strlen(temp);
            int i;
            int at = 0, dotAfterAt = 0;
            for (i = 0; temp[i] != '\0'; i++) {
                if (!((temp[i] >= 'a' && temp[i] <= 'z') || 
                    (temp[i] >= '0' && temp[i] <= '9') || 
                    temp[i] == '_' || temp[i] == '@' || temp[i] == '.')) {
                    isvalid = 0;
                    break;
                }
                if (temp[i] == '@') {
                    if (at == 1) { 
                        isvalid = 0; 
                        break; 
                    } // multiple @
                    at = 1;
                }
                if (at && temp[i] == '.') dotAfterAt = 1;
            }
            if (!at || !dotAfterAt || temp[0] == '@' || temp[len - 1] == '.'){
                isvalid = 0;
            }
        }

        if (!isvalid) {
            printf("\033[1;31mInvalid input format.\033[0m\n");
        }
    }while(!isvalid);
    int index[50]; 
    for ( int i = 0; i < addressBook->contactCount; i++) {
        if (choice == 1) {
            int len = strlen(temp);
            for (int pos = 0; pos <= (int)strlen(addressBook->contacts[i].name) - len; pos++) {
                if (strncasecmp(&addressBook->contacts[i].name[pos], temp, len) == 0) {
                    index[count++] = i;
                    break; // found a match in this name
                }
            }
        }
        else if (choice == 2 && strcmp(addressBook->contacts[i].phone, temp) == 0) {
            index[count++] = i;
        }
        else if (choice == 3 && strcmp(addressBook->contacts[i].email, temp) == 0) {
            index[count++] = i;
        }
    }
    if (count == 0) {
        printf("\033[1;31mNo matching contact found.\033[0m\n");
        return;
    }

    int delIndex;
    if (count == 1) {
        delIndex = index[0];  // only one match is found delete directly
    }
    else {
        printf("\nMultiple contacts found:\n");
        for (int i = 0; i < count; i++) {
            int idx = index[i];
            printf("%d. Name: %s\n Phone: %s\n Email: %s\n",
                   i + 1,
                   addressBook->contacts[idx].name,
                   addressBook->contacts[idx].phone,
                   addressBook->contacts[idx].email);
        }
        printf("Enter the number to delete: ");
        int option;
        scanf("%d", &option);

        if (option < 1 || option > count) {
            printf("\033[1;31mInvalid option.\033[0m\n");
            return;
        }
        delIndex = index[option - 1];
    }

    // Shift contacts left to delete
    for (int i = delIndex; i < addressBook->contactCount - 1; i++) {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }
    
    addressBook->contactCount--;

    printf("Contact deleted successfully.\n");
} 