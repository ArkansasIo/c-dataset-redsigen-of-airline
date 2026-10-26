
/*database navigation control
  Name: RecordBase Database Program
  Author: apocalypse coder0
  Date Started: 2018 /5/2
  Date Stop: 2026/10/25
  Version 2.0
  Description: C++ Records Database System (redesigned from the airline
  program). Domain mapping used throughout the redesign:
      airline         -> catalog
      flight          -> dataset
      passenger       -> entry
      reservation     -> transaction
      seat assignment -> index key
      airline.txt     -> database.dat
*/
//TODO (#0#): Work on entry_information
//TODO (#1#): Work on Transaction
//TODO (#2#): Work on Index Key
//TODO (#3#): Work out the Bugs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <iostream>
#include <cctype>

struct dataset;  //Forward Declaration of Dataset structure

//Catalog List Structure And Function Declarations

struct catalog    //Structure for Catalog node, includes strings for Catalog name
{                //pointer locations to the next catalog and also to the dataset list for that catalog
	char *catalogname;
	struct catalog  *next;
	struct dataset *node1;
} *start1 = NULL;

catalog * create_catalog_node(catalog  *start1);  //Function Declarations
catalog * enter_catalog_info(catalog  *node);
catalog * delete_catalog_node(catalog  *start1);
void print_catalog_list(catalog  *start1);
catalog * enter_dataset_menu(catalog  *start1);
void write_to_file(catalog  *start1);
catalog * read_from_file();
catalog * catalog_menu(catalog  *start1);

//Dataset List Structure And Function Declarations

struct entry;  //Forward Declaration of entry structure

struct dataset   //Structure for dataset node
{
	char *dataset_id;
	char *date_of_dataset;
	char *source_table;
	char *target_table;
	char *start_time;
	char *end_time;
	int total_rows;
	int rows_indexed;

	struct dataset *next;
	struct entry *node2;
	struct transaction *node3;
} *start2 = NULL;

struct transaction;  //forward declaration used by struct dataset above

dataset* create_dataset_node(dataset *start2);  //Function Declarations
dataset* enter_dataset_info(dataset *node);
dataset* delete_dataset_node(dataset *start2);
void print_dataset_list(dataset *start2);
dataset* enter_entry_menu(dataset *start2);
dataset* enter_transaction_menu(dataset *start2);
dataset* dataset_menu(dataset *start2);

//Entry List Structure And Function Declarations

struct entry  //Entry Structure includes name of the entry and the pointer to the next
{             //entry node, structure declaration also declares start as the first node
	char *name_suffix;  //Entry Title Suffix
    char *name;  //Entry Full Name
    char *entry_id; //(unique key)
    char *address;  //Entry Address
    char *city;  //Entry City
    char *gender;  //Entry Gender
    char *data_of_birth;  //Entry Data of Birth
    char *weight;  //Entry Weight
	struct entry *next;     //and declares it NULL
} *start3 = NULL;

entry* create_entry_node(entry *start3);  //Function Declarations
entry* enter_entry_info(entry *node);
entry* delete_entry_node(entry *start3);
entry* entry_menu(entry *start3);
void print_entry_list(entry *node);

struct transaction  //Transaction Structure
{
    char *transaction_id; //(unique key)
    char *entry_id;  //Entry ID
    char *dataset_id;  //Dataset ID
    char *cost;  //amount
	struct transaction  *next;     // and declares it NULL
} *start4 = NULL;

transaction* create_transaction_node(transaction *start4);  //Function Declarations
transaction* enter_transaction_info(transaction *node);
transaction* delete_transaction_node(transaction *start4);
transaction* transaction_menu(transaction *start4);
void print_transaction_list(transaction *node);


 struct index_key  //Index Key Structure
{
   char index_key[80];   //key name/id
   char cost[80];
   int Available;

      struct index_key  *next;
} *start5 = NULL;

index_key* create_index_key_node(index_key *start5);
index_key* enter_index_key_info(index_key *node);
index_key* cancelled_index_key_node(index_key *start5);
index_key* index_key_menu(index_key *start5);
void print_index_key_list(index_key *node);


//------------------------------------------------------------------------------
//Member Functions To Manipulate The Catalog Linked List

catalog  * create_catalog_node(catalog   *start1)  //Creates a sorted list of catalogs (default entry sort condition by catalog name)
{
	if (start1 == NULL)
{
		start1 = (catalog   *) malloc(sizeof(catalog  ));
		start1 = enter_catalog_info(start1);
		start1->next = NULL;
}
	else
{
		catalog   *count_node1, *count_node2, *node;
		node = (catalog   *) malloc(sizeof(catalog  ));
		node = enter_catalog_info(node);
		for (count_node1 = start1;
        (count_node1 != NULL) && (strcmp(count_node1->catalogname, node->catalogname) <= 0);
         count_node2 = count_node1, count_node1 = count_node1->next);
		if(count_node1 == start1)
{
		node->next = start1;
		start1 = node;
}
	else if (count_node1 == NULL)
{
    	count_node2->next = node;
		node->next = NULL;
}
		else
{
		count_node2->next = node;
		node->next = count_node1;
}
}
	return start1;
}


catalog  * enter_catalog_info(catalog   *node)  //Function to enter the information about the catalog
{
	printf("\n");
	printf(" Welcome To RecordBase Database System\n");
	printf("Please enter the Catalog Name\n");
	printf("\n");
	fflush(stdin);
	node->catalogname = new char [80];
	fgets(node->catalogname,80,stdin);
	node->catalogname[strcspn(node->catalogname,"\n")] = 0;
	node->node1 = NULL;
	return node;
}

catalog  * delete_catalog_node(catalog   *start)  //Function to delete the name of a catalog
{
	if(start1 == NULL)
	{
		printf("\n");
		printf("Sorry, this Database is not operational at this moment\n");
}
	else
{
		char catalogname[80];
		printf("\n");
		printf("Please enter the name of the Catalog you want to delete\n");
		fflush(stdin);
		fgets(catalogname,80,stdin);
	catalogname[strcspn(catalogname,"\n")] = 0;
		catalog   *count_node1, *count_node2;
		catalog   *temp;
	for(count_node1 = start1;
    (count_node1 != NULL) && (strcmp(count_node1->catalogname, catalogname) != 0);
     count_node2 = count_node1, count_node1 = count_node1->next);
    	if(count_node1!= NULL)
{
		if (count_node1 == start1)
{
			temp = start1;
			start1 = start1->next;
			free(temp);
}
	else if(count_node1->next == NULL && count_node1 != start1)
{
			temp = count_node1;
			count_node2->next = NULL;
			free (temp);
	}
	else
	{
				temp = count_node1;
				count_node2->next = count_node1->next;
				free (temp);
}
}
	else
{
			printf("\n");
			printf("Sorry, this catalog is currently not in this database\n");
}
}
	return start1;
}


void print_catalog_list(catalog   *start) /*Function to print all the catalogs in the database*/
{
	if (start1 == NULL)
 {
		printf("\n");
		printf("Sorry, this Database is not operational at this moment\n");
  }
	else
{
		printf("\n");
		catalog   *count;
		for (count = start1; count != NULL; count = count->next)
		printf("%s\n", count->catalogname);
	}
}

catalog  * enter_dataset_menu(catalog   *start1)  /*Function to search for the catalog
                                              entered and enter its dataset list menu*/
{
	char catalogname[80];
	printf("\n");
	printf("Welcome to the Dataset List Menu\n");
	printf("Enter the Catalog name you wish to check\n");
	fflush(stdin);
	fgets(catalogname,80,stdin);
	catalogname[strcspn(catalogname,"\n")] = 0;
	catalog   *count;
	for(count = start1;
     (count != NULL) && (strcmp(count->catalogname, catalogname) != 0);
      count = count->next);
	if(count == NULL)
	{
		printf("\n");
		printf("Sorry, this catalog does not exist in this database\n");
		return start1;
	}
	else
	{
		printf("\n");
		count->node1 = dataset_menu(count->node1);
		return start1;
	}
}

void write_to_file(catalog   *start1)  //Write to file database.dat
{
	FILE *fp;
	catalog   *count1;
	dataset *count2;
	entry *count3;
	transaction *count4;
	index_key *count5;
    int length = 0;
	int catalog_count = 0;
	int entry_count = 0;
	int dataset_count = 0;
	int transaction_count = 0;
	int index_key_count = 0;
	fp = fopen("database.dat","wb");

	//Determine number of catalogs and write to file
	for( count1 = start1; count1 != NULL; count1 = count1->next) ++catalog_count;
	fwrite(&catalog_count,sizeof(int),1,fp);

	//Loop through and write catalogs data to file
	for( count1 = start1; count1 != NULL; count1 = count1->next)
{
		//Determine length of catalog name and write to file
		length = strlen(count1->catalogname);
		fwrite(&length,sizeof(int),1,fp);

		//Write catalog name to file
		fwrite(count1->catalogname,length,1,fp);
//------------------------------------------------------------------------------
		//Determine number of datasets for the catalog and write to file
		for( count2 = count1->node1; count2 != NULL; count2 = count2->next) ++dataset_count;
		fwrite(&dataset_count,sizeof(int),1,fp);

		//Loop through and write dataset data for the catalog to file
		for( count2 = count1->node1; count2 != NULL; count2 = count2->next)
{
			//Write each dataset string field, length-prefixed
			length = strlen(count2->dataset_id);
			fwrite(&length,sizeof(int),1,fp);
			fwrite(count2->dataset_id,length,1,fp);

            length = strlen(count2->date_of_dataset);
            fwrite(&length,sizeof(int),1,fp);
			fwrite(count2->date_of_dataset,length,1,fp);

			length = strlen(count2->source_table);
			fwrite(&length,sizeof(int),1,fp);
			fwrite(count2->source_table,length,1,fp);

			length = strlen(count2->target_table);
			fwrite(&length,sizeof(int),1,fp);
			fwrite(count2->target_table,length,1,fp);

			length = strlen(count2->start_time);
			fwrite(&length,sizeof(int),1,fp);
			fwrite(count2->start_time,length,1,fp);

            length = strlen(count2->end_time);
            fwrite(&length,sizeof(int),1,fp);
			fwrite(count2->end_time,length,1,fp);

			//Write total_rows / rows_indexed
			fwrite(&count2->total_rows,sizeof(int),1,fp);
			fwrite(&count2->rows_indexed,sizeof(int),1,fp);
//------------------------------------------------------------------------------
			//Determine number of entries for this dataset and write to file
			for( count3 = count2->node2; count3 != NULL;
             count3 = count3->next) ++entry_count;
			fwrite(&entry_count,sizeof(int),1,fp);

			//Loop through and write entry data for the dataset to file
			for( count3 = count2->node2; count3 != NULL; count3 = count3->next)
{
				length = strlen(count3->name_suffix);
				fwrite(&length,sizeof(int),1,fp);
				fwrite(count3->name_suffix,length,1,fp);

                length = strlen(count3->name);
				fwrite(&length,sizeof(int),1,fp);
				fwrite(count3->name,length,1,fp);

           		length = strlen(count3->entry_id);
				fwrite(&length,sizeof(int),1,fp);
                fwrite(count3->entry_id,length,1,fp);

                length = strlen(count3->address);
   	            fwrite(&length,sizeof(int),1,fp);
				fwrite(count3->address,length,1,fp);

                length = strlen(count3->city);
				fwrite(&length,sizeof(int),1,fp);
				fwrite(count3->city,length,1,fp);

				length = strlen(count3->gender);
                fwrite(&length,sizeof(int),1,fp);
				fwrite(count3->gender,length,1,fp);

				length = strlen(count3->data_of_birth);
                fwrite(&length,sizeof(int),1,fp);
				fwrite(count3->data_of_birth,length,1,fp);

				length = strlen(count3->weight);
                fwrite(&length,sizeof(int),1,fp);
				fwrite(count3->weight,length,1,fp);
//------------------------------------------------------------------------------
		    //Determine number of transactions for this dataset and write to file
			for( count4 = count2->node3; count4 != NULL;
             count4 = count4->next) ++transaction_count;
			fwrite(&transaction_count,sizeof(int),1,fp);

			//Loop through and write transaction data for the dataset to file
			for( count4 = count2->node3; count4 != NULL; count4 = count4->next)
{
               length = strlen(count4->transaction_id);
               fwrite(&length,sizeof(int),1,fp);
               fwrite(count4->transaction_id,length,1,fp);

  			   length = strlen(count4->entry_id);
			   fwrite(&length,sizeof(int),1,fp);
			   fwrite(count4->entry_id,length,1,fp);

               length = strlen(count4->dataset_id);
			   fwrite(&length,sizeof(int),1,fp);
			   fwrite(count4->dataset_id,length,1,fp);

               length = strlen(count4->cost);
   	           fwrite(&length,sizeof(int),1,fp);
               fwrite(count4->cost,length,1,fp);
}
 }
  }
 }
	fclose(fp);
}

catalog  * read_from_file()  //read from file database.dat
{
	FILE *fp;
	catalog   *start, *curr;
	int length = 0;
	int catalog_count= 0;
	int dataset_count = 0;
	int entry_count = 0;
	int transaction_count = 0;
	int index_key_count = 0;

	//Initialize pointers
	start = curr = NULL;

	//Open file for reading
	fp = fopen("database.dat","rb");
	if(fp == NULL)
		return NULL;

	while(fread(&catalog_count,sizeof(int),1,fp) == 1)
	{
		//Loop through "catalog_count" catalogs
		for( ; catalog_count > 0; --catalog_count )
		{
			//allocate room for a single catalog structure
			catalog *a = (catalog *)malloc(sizeof(catalog));
			a->next = NULL;
			a->node1 = NULL;

			//read catalog name length, then the name itself
			fread(&length,sizeof(int),1,fp);
			a->catalogname = (char *)malloc(length+1);
			fread(a->catalogname,length,1,fp);
			a->catalogname[length] = 0;

			//append catalog to the end of the catalog list
			if(start == NULL)
				start = curr = a;
			else
			{
				curr->next = a;
				curr = curr->next;
			}

			//Read in number of datasets stored for the catalog
			fread(&dataset_count,sizeof(int),1,fp);

			//Loop through "dataset_count" datasets
			dataset *lastdataset = NULL;
			for( ; dataset_count > 0; --dataset_count )
			{
				//allocate room for a single dataset structure
				dataset *f = (dataset *)malloc(sizeof(dataset));
				f->next = NULL;
				f->node2 = NULL;
				f->node3 = NULL;

				//read each string field of the dataset, length-prefixed
				fread(&length,sizeof(int),1,fp);
				f->dataset_id = (char *)malloc(length+1);
				fread(f->dataset_id,length,1,fp);
				f->dataset_id[length] = 0;

				fread(&length,sizeof(int),1,fp);
				f->date_of_dataset = (char *)malloc(length+1);
				fread(f->date_of_dataset,length,1,fp);
				f->date_of_dataset[length] = 0;

				fread(&length,sizeof(int),1,fp);
				f->source_table = (char *)malloc(length+1);
				fread(f->source_table,length,1,fp);
				f->source_table[length] = 0;

				fread(&length,sizeof(int),1,fp);
				f->target_table = (char *)malloc(length+1);
				fread(f->target_table,length,1,fp);
				f->target_table[length] = 0;

				fread(&length,sizeof(int),1,fp);
				f->start_time = (char *)malloc(length+1);
				fread(f->start_time,length,1,fp);
				f->start_time[length] = 0;

				fread(&length,sizeof(int),1,fp);
				f->end_time = (char *)malloc(length+1);
				fread(f->end_time,length,1,fp);
				f->end_time[length] = 0;

				fread(&f->total_rows,sizeof(int),1,fp);
				fread(&f->rows_indexed,sizeof(int),1,fp);

				//append dataset to the end of this catalog's dataset list
				if(a->node1 == NULL)
					a->node1 = f;
				else
					lastdataset->next = f;
				lastdataset = f;

				//Read in number of entries stored for that dataset
				fread(&entry_count,sizeof(int),1,fp);

				//Loop through "entry_count" entries for this dataset
				entry *lastentry = NULL;
				for( ; entry_count > 0; --entry_count )
				{
					//allocate room for a single entry structure
					entry *p = (entry *)malloc(sizeof(entry));
					p->next = NULL;

					//read each string field of the entry, length-prefixed
					fread(&length,sizeof(int),1,fp);
					p->name_suffix = (char *)malloc(length+1);
					fread(p->name_suffix,length,1,fp);
					p->name_suffix[length] = 0;

					fread(&length,sizeof(int),1,fp);
					p->name = (char *)malloc(length+1);
					fread(p->name,length,1,fp);
					p->name[length] = 0;

					fread(&length,sizeof(int),1,fp);
					p->entry_id = (char *)malloc(length+1);
					fread(p->entry_id,length,1,fp);
					p->entry_id[length] = 0;

					fread(&length,sizeof(int),1,fp);
					p->address = (char *)malloc(length+1);
					fread(p->address,length,1,fp);
					p->address[length] = 0;

					fread(&length,sizeof(int),1,fp);
					p->city = (char *)malloc(length+1);
					fread(p->city,length,1,fp);
					p->city[length] = 0;

					fread(&length,sizeof(int),1,fp);
					p->gender = (char *)malloc(length+1);
					fread(p->gender,length,1,fp);
					p->gender[length] = 0;

					fread(&length,sizeof(int),1,fp);
					p->data_of_birth = (char *)malloc(length+1);
					fread(p->data_of_birth,length,1,fp);
					p->data_of_birth[length] = 0;

					fread(&length,sizeof(int),1,fp);
					p->weight = (char *)malloc(length+1);
					fread(p->weight,length,1,fp);
					p->weight[length] = 0;

					//append entry to the end of this dataset's entry list
					if(f->node2 == NULL)
						f->node2 = p;
					else
						lastentry->next = p;
					lastentry = p;
				}

				//Read in number of transactions stored for that dataset
				fread(&transaction_count,sizeof(int),1,fp);

				//Loop through "transaction_count" transactions for this dataset
				transaction *lasttrans = NULL;
				for( ; transaction_count > 0; --transaction_count )
				{
					//allocate room for a single transaction structure
					transaction *r = (transaction *)malloc(sizeof(transaction));
					r->next = NULL;

					//read each string field of the transaction, length-prefixed
					fread(&length,sizeof(int),1,fp);
					r->transaction_id = (char *)malloc(length+1);
					fread(r->transaction_id,length,1,fp);
					r->transaction_id[length] = 0;

					fread(&length,sizeof(int),1,fp);
					r->entry_id = (char *)malloc(length+1);
					fread(r->entry_id,length,1,fp);
					r->entry_id[length] = 0;

					fread(&length,sizeof(int),1,fp);
					r->dataset_id = (char *)malloc(length+1);
					fread(r->dataset_id,length,1,fp);
					r->dataset_id[length] = 0;

					fread(&length,sizeof(int),1,fp);
					r->cost = (char *)malloc(length+1);
					fread(r->cost,length,1,fp);
					r->cost[length] = 0;

					//append transaction to the end of this dataset's transaction list
					if(f->node3 == NULL)
						f->node3 = r;
					else
						lasttrans->next = r;
					lasttrans = r;
				}

				//Read in number of index keys stored for that dataset
				fread(&index_key_count,sizeof(int),1,fp);
			}
		}
	}
	fclose(fp);
	return start;
}
catalog  * catalog_menu(catalog   *start)  //Menu Function which is handles the cataloglist
{
	int choice;
	printf("Welcome to the RecordBase Database System Main Menu\n");
	printf("Please enter your choice from the menu shown below\n");
	printf("1: Add a new catalog to this database\n");
	printf("2: Delete a catalog from the database\n");
	printf("3: Display the Catalog List in this Database\n");
    printf("4: Enter Dataset menu for a catalog\n");
	printf("5: Exit the program (saves to database.dat)\n");
	scanf ("%d", &choice);
	switch(choice)
	{
	case 1:
		{
			start1 = create_catalog_node(start1);
		} break;
	case 2:
		{
			start1 = delete_catalog_node(start1);
		} break;
	case 3:
		{
			print_catalog_list(start1);
		} break;
	case 4:
		{
			start1 = enter_dataset_menu(start1);
		} break;
	case 5:
		{
			write_to_file(start1);
			exit(1);
		} break;
	default:
		{
			start1 = catalog_menu(start1);
		} break;
	}
	printf("\n");
	start1 = catalog_menu(start1);
	return start1;
}
//--------------------------------------------------------------------------------
//Member Functions To Manipulate The Dataset Linked List

dataset* create_dataset_node(dataset *start2)  // Creates a sorted list of datasets (default entry sort condition by dataset id.)
{
    printf("\n");
	if (start2 == NULL)
	{
		start2 = (dataset *) malloc(sizeof(dataset));
		start2 = enter_dataset_info(start2);
		start2->next = NULL;
}
	else
{
		dataset *count_node1, *count_node2, *node;
		node = (dataset *) malloc(sizeof(dataset));
		node = enter_dataset_info(node);
	for (count_node1 = start2;
    (count_node1 != NULL) && (strcmp(count_node1->dataset_id, node->dataset_id) <= 0);
     count_node2 = count_node1, count_node1 = count_node1->next);
	if(count_node1 == start2)
	{
		node->next = start2;
		start2 = node;
	}
	else if (count_node1 == NULL)
	{
			count_node2->next = node;
			node->next = NULL;
	}
	else
	{
		count_node2->next = node;
		node->next = count_node1;
	}
	}
	return start2;
}

dataset* enter_dataset_info(dataset *node)  // Function to enter the information about the dataset
{
	printf(" Welcome to the Dataset List\n");
	printf("Please enter the Dataset ID\n"); //Dataset ID (unique key)
	       fflush(stdin);
	       node->dataset_id = new char [80];
fgets(node->dataset_id,80,stdin);
	       node->dataset_id[strcspn(node->dataset_id,"\n")] = 0;
	printf("Please enter the Date of Dataset\n");
           fflush(stdin);
	       node->date_of_dataset = new char [80];
fgets(node->date_of_dataset,80,stdin);
	       node->date_of_dataset[strcspn(node->date_of_dataset,"\n")] = 0;
	printf("Please enter the Source Table\n");
	       fflush(stdin);
	       node->source_table = new char [80];
fgets(node->source_table,80,stdin);
	       node->source_table[strcspn(node->source_table,"\n")] = 0;
	printf("Please enter the Target Table\n");
	       fflush(stdin);
	       node->target_table = new char [80];
fgets(node->target_table,80,stdin);
	       node->target_table[strcspn(node->target_table,"\n")] = 0;
	printf("Please enter the Start time\n");
	       fflush(stdin);
           node->start_time = new char [80];
fgets(node->start_time,80,stdin);
	       node->start_time[strcspn(node->start_time,"\n")] = 0;
    printf("Please enter the End time\n");
           fflush(stdin);
           node->end_time = new char [80];
fgets(node->end_time,80,stdin);
	       node->end_time[strcspn(node->end_time,"\n")] = 0;
    printf("Please enter the Total Rows\n");
           fflush(stdin);
           scanf("%d", &node->total_rows);
    printf("Please enter the Rows Indexed\n");
           fflush(stdin);
           scanf("%d", &node->rows_indexed);
	       node->node2 = NULL;

	return node;
}

dataset* delete_dataset_node(dataset *start2)  // Function to delete a dataset by id
{
	printf("\n");

	if(start2 == NULL)
	{
		printf("Sorry, no available Datasets today\n");
	}
	else
	{
		char dataset_id[80];
	printf("Please enter the dataset id you want to delete\n");
		fflush(stdin);
		fgets(dataset_id,80,stdin);
	dataset_id[strcspn(dataset_id,"\n")] = 0;
		dataset *count_node1, *count_node2;
		dataset *temp;
		for(count_node1 = start2;
        (count_node1 != NULL) && (strcmp(count_node1->dataset_id, dataset_id)!= 0);
         count_node2 = count_node1, count_node1 = count_node1->next);
	if(count_node1!= NULL)
	{
	if (count_node1 == start2)
	{
			temp = start2;
			start2 = start2->next;
			free(temp);
		}
		else if(count_node1->next == NULL && count_node1 != start2)
		{
				temp = count_node1;
				count_node2->next = NULL;
				free (temp);
		}
		else
		{
				temp = count_node1;
				count_node2->next = count_node1->next;
				free (temp);
		}
	}
		else
		{
			printf("\n");
			printf("Sorry, this id was not found in this list\n");
		}
	}
	return start2;
}

void print_dataset_list(dataset *start2)
{
	printf("\n");
	if (start2 == NULL)
	{
		printf("Sorry, the Dataset List is empty\n");
	}
	else
	{
		dataset *count;
		for (count = start2; count != NULL; count = count->next)
		{
			printf("Dataset ID : %s\n", count->dataset_id);
			printf("Date of Dataset : %s\n", count->date_of_dataset);
		    printf("Source Table : %s\n", count->source_table);
		    printf("Target Table : %s\n", count->target_table);
		    printf("Start Time : %s\n", count->start_time);
		    printf("End Time : %s\n", count->end_time);
		    printf("Total Rows : %d\n", count->total_rows);
		    printf("Rows Indexed : %d\n", count->rows_indexed);
			printf("\n");
		}
	}
}

dataset* enter_entry_menu(dataset *start2)  // Function to search for the dataset entered and enter its entry list menu
{
	printf("\n");
	char dataset_id[80];
	printf("Welcome to the entry list Menu\n");
	printf("Enter the Dataset ID you wish to check\n");
	fflush(stdin);
	fgets(dataset_id,80,stdin);
	dataset_id[strcspn(dataset_id,"\n")] = 0;
	dataset *count;
	for(count = start2; (count != NULL) && (strcmp(count->dataset_id, dataset_id) != 0);
     count = count->next);
	if(count == NULL)
	{
		printf("Sorry, this dataset is not available\n");
		return start2;
	}
	else
	{
		count->node2 = entry_menu(count->node2);
		return start2;
	}
}

dataset* dataset_menu(dataset *start2)  // Menu Function which handles the datasetlist for a catalog
{
	int choice;
	printf("Welcome to the Dataset menu\n");
	printf("Please enter your choice from the menu shown below\n");
	printf("1: Add a new Dataset to this Catalog\n");
	printf("2: Delete a Dataset from the Catalog\n");
	printf("3: Display the Dataset List for the Catalog\n");
    printf("4: Enter entry menu for a dataset\n");
    printf("5: Enter transaction menu for a dataset\n");
    printf("6: Enter index key menu for a dataset\n");
	printf("7: Return back to the Main Menu\n");
	scanf ("%d", &choice);
	switch(choice)
	{
	case 1:
	{
			start2 = create_dataset_node(start2);
	     } break;
	case 2:
	{
			start2 = delete_dataset_node(start2);
		} break;
	case 3:
	{
			print_dataset_list(start2);
		} break;
	case 4:
	{
			start2 = enter_entry_menu(start2);
		} break;
	case 7:
	{
			return start2;
		} break;
	default:
	{
			start2 = dataset_menu(start2);
		} break;
	}
	printf("\n");
	start2 = dataset_menu(start2);
	return start2;
}

//------------------------------------------------------------------------------
//Member Functions To Manipulate The Entry Linked List

entry* create_entry_node(entry *start3)  //Creates a sorted list of entries
{
    printf("\n");
	if (start3 == NULL)
	{
		start3 = (entry *) malloc(sizeof(entry));
		start3 = enter_entry_info(start3);
		start3->next = NULL;
	}
	else
	{
		entry *count_node1, *count_node2, *node;
		node = (entry *) malloc(sizeof(entry));
		node = enter_entry_info(node);
	for (count_node1 = start3; (count_node1 != NULL) && (strcmp(count_node1->name, node->name) <= 0); count_node2 = count_node1, count_node1 = count_node1->next);
		if(count_node1 == start3)
	{
			node->next = start3;
			start3 = node;
	}
	else if (count_node1 == NULL)
	{
		count_node2->next = node;
		node->next = NULL;
	}
	else
	{
		count_node2->next = node;
		node->next = count_node1;
	}
	}
	return start3;
}


entry* enter_entry_info(entry *node)  // Function to enter the name of the entry
{
	printf(" Welcome to the Entry List\n");
	printf("Please enter Entry Name Suffix\n");
	fflush(stdin);
	node->name_suffix = new char [6];
	fgets(node->name_suffix,6,stdin);
	node->name_suffix[strcspn(node->name_suffix,"\n")] = 0;
    printf("Please enter Entry Full Name\n");
	fflush(stdin);
	node->name = new char [80];
	fgets(node->name,80,stdin);
	node->name[strcspn(node->name,"\n")] = 0;
	printf("please enter Entry ID\n");
	fflush(stdin);
	node->entry_id = new char [10];
	fgets(node->entry_id,10,stdin);
	node->entry_id[strcspn(node->entry_id,"\n")] = 0;
    printf("Please enter Entry Address;\n");
    fflush(stdin);
    node->address = new char [80];
	fgets(node->address,80,stdin);
	node->address[strcspn(node->address,"\n")] = 0;
    printf("Please enter Entry City\n");
    fflush(stdin);
    node->city = new char [30];
	fgets(node->city,30,stdin);
	node->city[strcspn(node->city,"\n")] = 0;
    printf("Please enter Gender\n");
    fflush(stdin);
    node->gender = new char [10];
    fgets(node->gender,10,stdin);
	node->gender[strcspn(node->gender,"\n")] = 0;
    printf("Please enter Data Of Birth\n");
    fflush(stdin);
    node->data_of_birth = new char [10];
    fgets(node->data_of_birth,10,stdin);
	node->data_of_birth[strcspn(node->data_of_birth,"\n")] = 0;
    printf("Please enter Weight\n");
    fflush(stdin);
    node->weight = new char [10];
    fgets(node->weight,10,stdin);
	node->weight[strcspn(node->weight,"\n")] = 0;
	return node;
}

entry* delete_entry_node(entry *start3)  // Function to delete the name of an entry in the dataset
{
	printf("\n");
	if(start3 == NULL)
	{
		printf("\n");
		printf("Sorry, the entry list is empty\n");
	}
	else
	{
		char name[80];
		printf("Please enter the name of the entry you want to delete\n");
		fflush(stdin);
		fgets(name,80,stdin);
	name[strcspn(name,"\n")] = 0;
		entry *count_node1, *count_node2;
		entry *temp;
	for(count_node1 = start3; (count_node1 != NULL) && (strcmp(count_node1->name, name) != 0);
    count_node2 = count_node1, count_node1 = count_node1->next);
		if(count_node1!= NULL)
	{
		if (count_node1 == start3)
	{
				temp = start3;
				start3 = start3->next;
				free(temp);
	}
		else if(count_node1->next == NULL && count_node1 != start3)
	{
			temp = count_node1;
			count_node2->next = NULL;
			free (temp);
	}
	else
	{
			temp = count_node1;
			count_node2->next = count_node1->next;
			free (temp);
		}
	}
	else
	{
		printf("\n");
		printf("Sorry, this name was not found in this list\n");
	}
}
	return start3;
}

entry* entry_menu(entry *start3)  // Menu Function which is called from dataset menu
{
	printf("\n");
	int choice;
	printf("Welcome to the entry menu\n");
	printf("Please enter your choice from the menu shown below\n");
	printf("1: Add an entry to this dataset\n");
	printf("2: Delete an entry from this dataset\n");
	printf("3: Display the entry list for this dataset\n");
	printf("4: Return back to the Dataset Menu\n");
	scanf ("%d", &choice);
	switch(choice)
	{
	case 1:
		{
			start3 = create_entry_node(start3);
		} break;
	case 2:
		{
			start3 = delete_entry_node(start3);
		} break;
	case 3:
		{
			print_entry_list(start3);
		} break;
	case 4:
		{
			return start3;
		} break;
	default:
		{
			start3 = entry_menu(start3);
		} break;
	}
	start3 = entry_menu(start3);
	return start3;
}

void print_entry_list(entry *start3)
{
	printf("\n");
	if (start3 == NULL)
	{
		printf("Sorry, the entry list is empty\n");
	}
	else
	{
		entry *count;
		for (count = start3; count != NULL; count = count->next)
		{
			printf("Entry Name Suffix : %s\n", count->name_suffix);
			printf("Entry Name : %s\n", count->name);
			printf("EntryID : %s\n", count->entry_id);
			printf("Entry Address : %s\n", count->address);
			printf("Entry City : %s\n", count->city);
			printf("Entry Gender : %s\n", count->gender);
			printf("Entry Data Of Birth : %s\n", count->data_of_birth);
			printf("Entry Weight : %s\n", count->weight);
			printf("\n");
		}
	}
}
//------------------------------------------------------------------------------
//Member Functions To Manipulate The transaction Linked List

transaction* create_transaction_node(transaction *start4)  //Creates a sorted list of transactions
{
    printf("\n");
	if (start4 == NULL)
	{
		start4 = (transaction *) malloc(sizeof(transaction));
		start4 = enter_transaction_info(start4);
		start4->next = NULL;
	}
	else
	{
		transaction *count_node1, *count_node2, *node;
		node = (transaction *) malloc(sizeof(transaction));
		node = enter_transaction_info(node);
	for (count_node1 = start4;
    (count_node1 != NULL) && (strcmp(count_node1->transaction_id, node->transaction_id) <= 0);
     count_node2 = count_node1, count_node1 = count_node1->next);
		if(count_node1 == start4)
	{
			node->next = start4;
			start4 = node;
	}
	else if (count_node1 == NULL)
	{
		count_node2->next = node;
		node->next = NULL;
	}
	else
	{
		count_node2->next = node;
		node->next = count_node1;
	}
	}
	return start4;
}

transaction* enter_transaction_info(transaction *node)  //Function to enter a transaction
{
	printf(" Welcome to the Transaction List\n");
	printf("Please enter your Transaction ID\n");
	fflush(stdin);
	node->transaction_id = new char [80];
	fgets(node->transaction_id,80,stdin);
	node->transaction_id[strcspn(node->transaction_id,"\n")] = 0;
	printf("please enter your Entry ID\n");
	fflush(stdin);
	node->entry_id = new char [10];
	fgets(node->entry_id,10,stdin);
	node->entry_id[strcspn(node->entry_id,"\n")] = 0;
    printf("Please enter your Dataset ID;\n");
    fflush(stdin);
    node->dataset_id = new char [80];
	fgets(node->dataset_id,80,stdin);
	node->dataset_id[strcspn(node->dataset_id,"\n")] = 0;
    printf("Please enter your Amount\n");
    fflush(stdin);
    node->cost = new char [30];
	fgets(node->cost,30,stdin);
	node->cost[strcspn(node->cost,"\n")] = 0;
	return node;
}

transaction* delete_transaction_node(transaction *start4)  //Function to delete a transaction by id
{
	printf("\n");
	if(start4 == NULL)
	{
		printf("\n");
		printf("Sorry, the transaction list is empty\n");
	}
	else
	{
		char transaction_id[80];
		printf("Please enter the transaction id you want to delete\n");
		fflush(stdin);
		fgets(transaction_id,80,stdin);
	transaction_id[strcspn(transaction_id,"\n")] = 0;
		transaction *count_node1, *count_node2;
		transaction *temp;
	for(count_node1 = start4;
    (count_node1 != NULL) && (strcmp(count_node1->transaction_id, transaction_id) != 0);
     count_node2 = count_node1, count_node1 = count_node1->next);
		if(count_node1!= NULL)
	{
		if (count_node1 == start4)
	{
				temp = start4;
				start4 = start4->next;
				free(temp);
	}
		else if(count_node1->next == NULL && count_node1 != start4)
	{
			temp = count_node1;
			count_node2->next = NULL;
			free (temp);
	}
		else
	{
				temp = count_node1;
				count_node2->next = count_node1->next;
				free (temp);
		}
	}
	else
	{
		printf("\n");
		printf("Sorry, this name was not found in this list\n");
	}
}
	return start4;
}

transaction* transaction_menu(transaction *start4)  // Menu Function for transactions
{
	printf("\n");
	int choice;
	printf("Welcome to the Transaction menu\n");
	printf("Please enter your choice from the menu shown below\n");
	printf("1: Add a transaction to this dataset\n");
	printf("2: Delete a transaction from this dataset\n");
	printf("3: Display the transaction list for this dataset\n");
	printf("4: Return back to the entry Menu\n");
	scanf ("%d", &choice);
	switch(choice)
	{
	case 1:
		{
			start4 = create_transaction_node(start4);
		} break;
	case 2:
		{
			start4 = delete_transaction_node(start4);
		} break;
	case 3:
		{
			print_transaction_list(start4);
		} break;
	case 4:
		{
			return start4;
		} break;
	default:
		{
			start4 = transaction_menu(start4);
		} break;
	}
	start4 = transaction_menu(start4);
	return start4;
}

void print_transaction_list(transaction *start4)
{
	printf("\n");
	if (start4 == NULL)
	{
		printf("Sorry, the transaction list is empty\n");
	}
	else
	{
		transaction *count;
		for (count = start4; count != NULL; count = count->next)
		{
			printf("Transaction ID : %s\n", count->transaction_id);
			printf("EntryID : %s\n", count->entry_id);
			printf("Dataset ID : %s\n", count->dataset_id);
			printf("Amount : %s\n", count->cost);
			printf("\n");
		}
	}
}
//------------------------------------------------------------------------------
//Index Keys: total rows and rows indexed per dataset
index_key* create_index_key_node(index_key *start5)
{
    printf("\n");
	if (start5 == NULL)
	{
		start5 = (index_key *) malloc(sizeof(index_key));
		start5 = enter_index_key_info(start5);
		start5->next = NULL;
	}
	else
	{
		index_key *count_node1, *count_node2, *node;
		node = (index_key *) malloc(sizeof(index_key));
		node = enter_index_key_info(node);
	for (count_node1 = start5; (count_node1 != NULL) && (strcmp(count_node1->index_key, node->index_key) <= 0); count_node2 = count_node1, count_node1 = count_node1->next);
		if(count_node1 == start5)
	{
			node->next = start5;
			start5 = node;
	}
	else if (count_node1 == NULL)
	{
		count_node2->next = node;
		node->next = NULL;
	}
	else
	{
		count_node2->next = node;
		node->next = count_node1;
	}
	}
	return start5;
}

index_key* enter_index_key_info(index_key *node)
{
	printf(" Welcome to the Index Key List\n");
	printf("Please enter the index key\n");
	fflush(stdin);
	fgets(node->index_key,80,stdin);
	node->index_key[strcspn(node->index_key,"\n")] = 0;
	return node;
}

index_key* cancelled_index_key_node(index_key *start5)
{
	printf("\n");
	if(start5 == NULL)
	{
		printf("\n");
		printf("Sorry, the index key list is empty\n");
	}
	else
	{
		char key_name[80];
		printf("Please enter the index key you want to delete\n");
		fflush(stdin);
		fgets(key_name,80,stdin);
		key_name[strcspn(key_name,"\n")] = 0;
		index_key *count_node1, *count_node2;
		index_key *temp;
	for(count_node1 = start5;
     (count_node1 != NULL) && (strcmp(count_node1->index_key, key_name) != 0);
      count_node2 = count_node1, count_node1 = count_node1->next);
		if(count_node1!= NULL)
	{
		if (count_node1 == start5)
	{
			temp = start5;
			start5 = start5->next;
			free(temp);
	}
		else if(count_node1->next == NULL && count_node1 != start5)
	{
			temp = count_node1;
			count_node2->next = NULL;
			free (temp);
	}
		else
	{
			temp = count_node1;
			count_node2->next = count_node1->next;
			free (temp);
		}
	}
	else
	{
		printf("\n");
		printf("Sorry, this key was not found in this list\n");
	}
}
	return start5;
}

index_key* index_key_menu(index_key *start5)  // Menu Function for index keys
{
	printf("\n");
	int choice;
	printf("Welcome to the Index Key menu\n");
	printf("Please enter your choice from the menu shown below\n");
	printf("1: Add an index key to this dataset\n");
	printf("2: Cancel an index key from this dataset\n");
	printf("3: Move entry index key\n");
	printf("4: Display the index key list for this dataset\n");
	printf("5: Return back to the entry Menu\n");
	scanf ("%d", &choice);
	switch(choice)
	{
	case 1:
		{
			start5 = create_index_key_node(start5);
		} break;
	case 2:
		{
			start5 = cancelled_index_key_node(start5);
		} break;
	case 3:
		{
			print_index_key_list(start5);
		} break;
	case 4:
		{
			return start5;
		} break;
	default:
		{
			start5 = index_key_menu(start5);
		} break;
	}
	start5 = index_key_menu(start5);
	return start5;
}

void print_index_key_list(index_key *start5)
{
	printf("\n");
	if (start5 == NULL)
	{
		printf("Sorry, the index key list is empty\n");
	}
	else
	{
		index_key *count;
		for (count = start5; count != NULL; count = count->next)
		printf("Index Key : %s\n", count->index_key);
	}
}
//Main Function Calling The Menu Function

int main()
{
	start1 = read_from_file();
	start1 = catalog_menu(start1);
	return 0;
}

//------------------------------------------------------------------------------
/*
The RecordBase database system keeps track of catalogs, datasets,
entries and transactions in a records database.

Each dataset requires the following information:
Dataset ID (unique key), Source and Target Table, Date of Dataset,
Start and End Time, Total Rows, Rows Indexed and an array of entries
on the dataset stored by entry ID.

Each transaction requires the following information:
Transaction ID (unique key), Entry ID, Dataset ID, Amount

Each entry requires the following information:
Entry ID (unique key), Title, Full Name, Address, City

1)  Create structures for each of the above files and read the
information from separate data files.

2)  The database also wants the following set up:
a)  An input screen to add new datasets (does not add entries).
b)  An input screen to add new entries (does not add datasets).
c)  An input screen to add transactions (used to add entries to datasets).
d)  An output screen to show all current datasets, sorted by start time.
e)  An output screen to show all the entries on a selected dataset
    sorted by name.
f)  Add a save option to store the file.

Some other notes:
- Entries and datasets must already be in the system in order
to be added to a transaction.

- Use the unique ID's so you do not repeat data in the files.
*/
