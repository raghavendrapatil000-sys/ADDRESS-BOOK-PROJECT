/*Name - Raghavendra.B.Patil
Date - 07-04-2026
<-----Address Book-------->*/


#include <stdio.h>          // Standard input-output functions
#include <stdlib.h>         // Standard library functions
#include <string.h>         // String handling functions
#include "contact.h"        // Contact structure definition
#include "file.h"           // File handling functions
#include "populate.h"       // Populate initial data

// ================= LIST CONTACTS =================
void listContacts(AddressBook *addressBook) 
{
    if(addressBook->contactCount == 0)   // Check if contact list is empty
    {
        printf("Contact not found!!\n"); // Print message if empty
        return;                          // Exit function
    }

    // Sorting (Bubble Sort)
    for(int i = 0; i < addressBook->contactCount - 1; i++)  // Outer loop
    {
        for(int j = 0; j < addressBook->contactCount - i - 1; j++) // Inner loop
        {
            if(strcmp(addressBook->contacts[j].name,
                      addressBook->contacts[j+1].name) > 0) // Compare names
            {
                Contact temp = addressBook->contacts[j];    // Swap contacts
                addressBook->contacts[j] = addressBook->contacts[j+1];
                addressBook->contacts[j+1] = temp;
            }
        }
    }

    printf("\n----------------------------------\n"); // Header line
    printf("        Contact List\n");               // Title
    printf("----------------------------------\n"); // Header line
    printf("No   Name           Phone        Email\n"); // Column titles

    for(int i = 0; i < addressBook->contactCount; i++) // Loop through contacts
    {
        printf("%d.  %-12s %-12s %-20s\n",
               i + 1,                                  // Serial number
               addressBook->contacts[i].name,          // Name
               addressBook->contacts[i].phone,         // Phone
               addressBook->contacts[i].email);        // Email
    }
}

// ================= INITIALIZE =================
void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;      // Initialize contact count
    loadContactsFromFile(addressBook);  // Load contacts from file
}

// ================= SAVE =================
void saveAndExit(AddressBook *addressBook)
{
    saveContactsToFile(addressBook);    // Save contacts to file
    exit(EXIT_SUCCESS);                 // Exit program
}

// ================= VALIDATION =================
int validate_name(char *name)
{
    for(int i = 0; name[i] != '\0'; i++) // Loop through each character
    {
        if(!(name[i] >= 'A' && name[i] <= 'Z' ||
             name[i] >= 'a' && name[i] <= 'z' ||
             name[i] == ' ' || name[i] == '.')) // Check valid characters
        {
            return 0;                   // Invalid name
        }
    }
    return 1;                           // Valid name
}

int validate_phone(AddressBook *addressBook, char *phone)
{
    if(strlen(phone) != 10)             // Check length
        return 0;

    if(phone[0] < '6' || phone[0] > '9') // Check starting digit
        return 0;

    for(int i = 0; phone[i]; i++)       // Check digits only
        if(phone[i] < '0' || phone[i] > '9')
            return 0;

    for(int i = 0; i < addressBook->contactCount; i++) // Check duplicate
        if(strcmp(addressBook->contacts[i].phone, phone) == 0)
            return 0;

    return 1;                           // Valid phone
}

int validate_email(AddressBook *addressBook, char *email)
{
    if(strchr(email,'@') == NULL || strstr(email,".com") == NULL) // Basic format check
        return 0;

    for(int i = 0; i < addressBook->contactCount; i++) // Check duplicate
        if(strcmp(addressBook->contacts[i].email, email) == 0)
            return 0;

    return 1;                           // Valid email
}

// ================= CREATE =================
void createContact(AddressBook *addressBook)
{
    char name[20], phone[11], email[30]; // Variables for input

    while(1)
    {
        printf("Enter Name: ");         // Prompt name
        scanf(" %[^\n]", name);         // Read name
        if(validate_name(name)) break;  // Validate
        printf("Invalid Name!\n");      // Error message
    }

    while(1)
    {
        printf("Enter Phone: ");        // Prompt phone
        scanf("%s", phone);             // Read phone
        if(validate_phone(addressBook, phone)) break; // Validate
        printf("Invalid Phone!\n");     // Error message
    }

    while(1)
    {
        printf("Enter Email: ");        // Prompt email
        scanf("%s", email);             // Read email
        if(validate_email(addressBook, email)) break; // Validate
        printf("Invalid Email!\n");     // Error message
    }

    int i = addressBook->contactCount;  // Get index

    strcpy(addressBook->contacts[i].name, name);   // Store name
    strcpy(addressBook->contacts[i].phone, phone); // Store phone
    strcpy(addressBook->contacts[i].email, email); // Store email

    addressBook->contactCount++;        // Increment count

    printf("Contact Added Successfully!\n"); // Success message
}

// ================= SEARCH HELPERS =================
int search_by_name(AddressBook *addressBook)
{
    char name[50];
    int foundIndexes[100];  // store matching indexes
    int count = 0;

    printf("Enter name: ");
    scanf(" %[^\n]", name);

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        if(strstr(addressBook->contacts[i].name, name))
        {
            printf("%d. %s %s %s\n", count + 1,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);

            foundIndexes[count++] = i;  // store index
        }
    }

    if(count == 0)
    {
        printf("Contact not found\n");
        return -1;
    }

    int choice;
    printf("Select contact (1-%d): ", count);
    scanf("%d", &choice);

    if(choice < 1 || choice > count)
    {
        printf("Invalid selection\n");
        return -1;
    }

    return foundIndexes[choice - 1]; // return actual index
}

int search_by_phone(AddressBook *addressBook)
{
    char phone[20];
    printf("Enter phone: ");           // Prompt
    scanf("%s", phone);               // Read input

    for(int i = 0; i < addressBook->contactCount; i++) // Loop
    {
        if(strcmp(addressBook->contacts[i].phone, phone) == 0)
            return i;                 // Match found
    }
    printf("Contact not found\n");    // Not found
    return -1;
}

int search_by_email(AddressBook *addressBook)
{
    char email[50];
    printf("Enter email: ");          // Prompt
    scanf("%s", email);              // Read input

    for(int i = 0; i < addressBook->contactCount; i++) // Loop
    {
        if(strcmp(addressBook->contacts[i].email, email) == 0)
            return i;                // Match found
    }
    printf("Contact not found\n");   // Not found
    return -1;
}

// ================= SAFE INPUT =================
int getChoice()
{
    int choice;

    while(1)
    {
        printf("Enter your choice: "); // Prompt

        if(scanf(" %d", &choice) != 1) // Check valid integer input
        {
            printf("INVALID CHOICE !!\n");
            while(getchar() != '\n');  // Clear buffer
            continue;
        }

        if(choice >= 1 && choice <= 3) // Valid range
            return choice;

        printf("INVALID CHOICE !!\n"); // Invalid choice
    }
}

// ================= SEARCH =================
void searchContact(AddressBook *addressBook)
{
    int choice;

    printf("\n1. Name\n2. Phone\n3. Email\n"); // Menu

    choice = getChoice(); // Get user choice

    if(choice == 1) search_by_name(addressBook); // Search by name
    else if(choice == 2) search_by_phone(addressBook); // Search by phone
    else search_by_email(addressBook); // Search by email
}

// ================= EDIT =================
void editContact(AddressBook *addressBook)
{
    int choice, index;

    printf("\n1. Name\n2. Phone\n3. Email\n"); // Menu

    choice = getChoice(); // Get choice

    if(choice == 1) index = search_by_name(addressBook); // Find by name
    else if(choice == 2) index = search_by_phone(addressBook); // Find by phone
    else index = search_by_email(addressBook); // Find by email

    if(index == -1) return; // Not found

    printf("\n1. Edit Name\n2. Edit Phone\n3. Edit Email\n"); // Edit menu

    choice = getChoice(); // Get choice

    if(choice == 1)
    {
        printf("Enter new name: ");
        scanf(" %[^\n]", addressBook->contacts[index].name); // Update name
    }
    else if(choice == 2)
    {
        printf("Enter new phone: ");
        scanf("%s", addressBook->contacts[index].phone); // Update phone
    }
    else
    {
        printf("Enter new email: ");
        scanf("%s", addressBook->contacts[index].email); // Update email
    }

    printf("Updated Successfully\n"); // Success message
}

// ================= DELETE =================
void deleteContact(AddressBook *addressBook)
{
    int choice, index;

    printf("\n1. Name\n2. Phone\n3. Email\n"); // Menu

    choice = getChoice(); // Get choice

    if(choice == 1) index = search_by_name(addressBook); // Find
    else if(choice == 2) index = search_by_phone(addressBook);
    else index = search_by_email(addressBook);

    if(index == -1) return; // Not found

    for(int i = index; i < addressBook->contactCount - 1; i++) // Shift elements
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }

    addressBook->contactCount--; // Reduce count

    printf("Contact Deleted Successfully\n"); // Success message
}