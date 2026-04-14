#include <stdio.h>
#include "contact.h"
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.txt","w");

    if(fptr == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    fprintf(fptr,"%d\n",addressBook->contactCount);

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fptr,"%s,%s,%s\n",
        addressBook->contacts[i].name,
        addressBook->contacts[i].phone,
        addressBook->contacts[i].email);
    }

    fclose(fptr);
}

void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.txt","r");

    if(fptr == NULL)
    {
        printf("File not found. Starting new address book.\n");
        return;
    }

    fscanf(fptr,"%d\n",&addressBook->contactCount);

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        fscanf(fptr,"%[^,],%[^,],%[^\n]\n",
        addressBook->contacts[i].name,
        addressBook->contacts[i].phone,
        addressBook->contacts[i].email);
    }

    fclose(fptr);
}