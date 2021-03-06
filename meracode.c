#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

int global_depth ;
int delete_option;
int bucket_size ;

struct node
{
	int *p;
	int ldepth;
	int number;
	int left;
	struct node *next;
};


int hash(int data)
{
	int hash_fun =  (int) pow((double) 2,global_depth);
	return data % hash_fun;
}
int search(int data,struct node **head)
{
	struct node *pass,*new;
	int i=0;
	pass = *head;
	new = *head;
	int hased_value = hash(data);
	while(hased_value != pass->number)
	{
		pass = pass->next;
	}
	for(i=0;i<bucket_size;i++)
	{
		if(data == pass->p[i])
		{
			return 1;
		}
	}
	return 0;
}
void delete_1 (int data , struct node **head)
{
	struct node *pass,*new;
	int i=0;
	pass = *head;
	new = *head;
	int hased_value = hash(data);
	while(hased_value != pass->number)
	{
		pass = pass->next;
	}
	while(1)
	{
		if(data == pass->p[i])
		{
			pass->p[i] = pass->p[bucket_size-(pass->left)-1];
			(pass->left)++;
			break;
		}
		i++;

	}
	while(new != NULL)
	{
		if(new->p == pass->p)
			new->left = pass->left;
		new = new->next;
	}
}

void delete_2(int data , struct node **head)
{
	struct node *pass,*new;
	int i=0;
	pass = *head;
	new = *head;
	int hased_value = hash(data);
	int image;
	int jump = (int) pow((double) 2,global_depth-1);
	while(hased_value != pass->number)
	{
		pass = pass->next;
	}
	while(1)
	{
		if(data == pass->p[i])
		{
			pass->p[i] = pass->p[bucket_size-(pass->left)-1];
			(pass->left)++;
			break;
		}
		i++;

	}
	while(new != NULL)
	{
		if(new->p == pass->p)
			new->left = pass->left;
		new = new->next;
	}
	new = *head; 	//reuse
	// merging two bucket
	if(pass->left == bucket_size)
	{
		if(pass->number < jump)
			image = pass->number + jump;
		else
			image = pass->number - jump;

		while(new->number != image)
		{
			new  = new->next;
		}

		free(pass->p);
		pass->p = new->p;
		(pass->ldepth)--;
		(new->ldepth)--;
		pass->left = new->left;
	}

}

void delete_3(int data , struct node **head)
{
	struct node *pass,*new,*fur;
	int i=0,flag=0,limit;
	pass = *head;
	new = *head;
	int hased_value = hash(data);
	int image;
	int jump = (int) pow((double) 2,global_depth-1);
	limit = (int) pow((double) 2,global_depth-1);
	while(hased_value != pass->number)
	{
		pass = pass->next;
	}
	while(i<bucket_size)
	{
		if(data == pass->p[i])
		{
			pass->p[i] = pass->p[bucket_size-(pass->left)-1];
			(pass->left)++;
			break;
		}
		i++;
	}
	if(i != bucket_size)
	{

		while(new != NULL)
		{
			if(new->p == pass->p)
				new->left = pass->left;
			new = new->next;
		}
		new = *head; 	//reuse
		// merging two bucket
		if(pass->left == bucket_size)
		{
			if(pass->number < jump)
				image = pass->number + jump;
			else
				image = pass->number - jump;

			while(new->number != image)
			{
				new  = new->next;
			}

			free(pass->p);
			pass->p = new->p;
			(pass->ldepth)--;
			(new->ldepth)--;
			pass->left = new->left;
		}

		pass = *head;


		while(pass != NULL)
		{
			if(pass->ldepth != global_depth-1)
			{
				flag = 1;
				break;
			}
			pass = pass->next;
		}
		pass = *head;	
		//printf("flag %d\n", flag);
		if(flag==0)
		{
			for(i=0;i<limit-1;i++)
				pass = pass->next;
			new = pass->next;
			pass->next = NULL;
			while(new != NULL)
			{
				fur = new->next;
				free(new);
				new = fur;

			}
			global_depth--;
		}

	}
}

void double_dir(struct node **head)
{
	int count = 1,i,j;
	struct node *pass,*new;
	pass = *head;
	new = *head;
	while(pass->next != NULL)
	{
		pass = pass->next;
		count++;
	}
	j = count;

	//  for least significant bit 
	for(i=0;i<count;i++)
	{
		pass->next = (struct node*) malloc(sizeof(struct node));		//copying data from upper dir
		pass = pass->next;
		pass->p = new->p;
		pass->ldepth = new->ldepth;
		pass->left = new->left;
		pass->number = j;
		j++;
		pass->next = NULL;
		new = new->next;
	}
	// global_depth++;

}

void add_bucket(int data,struct node **head,struct node **curr)
{
	struct node *add_buc,*pass;
	pass = *curr;
	add_buc = *head;
	int i,steps,hased_value;
	int trav,j=0;
	hased_value = hash(data);
	//steps = (int) pow((double) 2,global_depth-1)+pass->number;
	//printf("hased_value = %d\n",hased_value );

	while(add_buc->number != hased_value)
		add_buc = add_buc->next;

	add_buc->p =  (int*) malloc(bucket_size * sizeof(int)); // create new bucket
	add_buc->left = bucket_size;
	add_buc->ldepth++;
	pass->ldepth++;

	// copyng old data
	for(i=0;i<bucket_size;i++)
	{
		hased_value = hash(pass->p[i]);
		if(hased_value == pass->number)
		{
			pass->p[j] = pass->p[i];
			j++;
		}
		else
		{
			add_buc->p[bucket_size - add_buc->left] = pass->p[i];
			add_buc->left = (add_buc->left) -1;

		}
	}
	pass->left = bucket_size -j;

	// enter new data 
	hased_value = hash(data);
	if(hased_value == pass->number)
	{
		pass->p[bucket_size-pass->left] = data;
		pass->left = (pass->left)-1;
	}
	else
	{
		add_buc->p[bucket_size - add_buc->left] = data;
		add_buc->left = (add_buc->left) -1;
	}

}
void add_bucket2(int data,struct node **head,struct node **curr)
{
	struct node *add_buc,*pass;
	pass = *curr;
	add_buc = *head;
	int i,steps,hased_value;
	int trav,j=0;
	//hased_value = hash(data);
	steps = (int) pow((double) 2,global_depth-1)+pass->number;
	//printf("hased_value = %d\n",hased_value );
	for(i=0;i<steps;i++)
	{
		add_buc = add_buc->next;
	}
	// while(add_buc->number != hased_value)
	// 	add_buc = add_buc->next;

	add_buc->p =  (int*) malloc(bucket_size * sizeof(int)); // create new bucket
	add_buc->left = bucket_size;
	add_buc->ldepth++;
	pass->ldepth++;

	// copyng old data
	for(i=0;i<bucket_size;i++)
	{
		hased_value = hash(pass->p[i]);
		if(hased_value == pass->number)
		{
			pass->p[j] = pass->p[i];
			j++;
		}
		else
		{
			add_buc->p[bucket_size - add_buc->left] = pass->p[i];
			add_buc->left = (add_buc->left) -1;

		}
	}
	pass->left = bucket_size -j;

	// enter new data 
	hased_value = hash(data);
	if(hased_value == pass->number)
	{
		pass->p[bucket_size-pass->left] = data;
		pass->left = (pass->left)-1;
	}
	else
	{
		add_buc->p[bucket_size - add_buc->left] = data;
		add_buc->left = (add_buc->left) -1;
	}

}
void index_status(struct node **head)
{
	struct node *pass;
	pass = *head;
	int i ;
	while(pass != NULL)
	{
		if(pass->number<pow(2,pass->ldepth)){
			printf("%d\t %d\t %d\t\t",pass->number,pass->ldepth,pass->left );
			for(i=0;i<bucket_size - pass->left ; i++)
			{
				printf("%d\t", pass->p[i]);
			}
			printf("\n");
	}
		pass = pass->next;
	}
}
void insert_data(int data,struct node **head)
{
	int hased_value = hash(data);
	//printf(" hased_value : %d\n",hased_value );
	struct node *pass,*update_left;
	pass = *head;
	update_left = *head;
			// for distrubuting bucket

	while(hased_value != pass->number)
	{
		pass = pass->next;
	}
	//printf("number %d\n",pass->number );
	if(pass->left > 0)
	{
		pass->p[bucket_size - pass->left] = data;
		while(update_left != NULL)
		{
			if(update_left->p == pass->p)
				update_left->left = (update_left->left) -1;

			update_left = update_left->next;
		}


		//printf("in 1st %d\n",data );
	}
	else if(pass->left == 0)
	{

		if(pass->ldepth == global_depth)
		{
			double_dir(head);
			global_depth++;
			//index_status(head);
			add_bucket2(data,head,&pass);
			//printf("in 2st %d\n",data );
			
		}
		else if (pass->ldepth < global_depth)
		{
			if(pass->number >= (int) pow((double) 2,global_depth-1))
			{
				hased_value = data % (int) pow((double) 2,pass->ldepth);
				pass = *head;
				while(hased_value != pass->number)
				{
					pass = pass->next;
				}
			}
			//printf("in 3.1st %d\n",hased_value );
			add_bucket2(data,head,&pass);
			//printf("in 3st %d\n",data );
		}
	}
}


int main(int argc , char * argv[])
{
	char *p[100];
	int data,k=0;
	if(argc != 5)
	{
		printf("not full arguments\n");
		return 0;
	}
	global_depth = atoi(argv[1]);
	bucket_size = atoi(argv[2]);
	delete_option = atoi(argv[3]);

	// for global_depth = 0	
	struct node *pass;
	struct node *head = (struct node*) malloc(sizeof(struct node));
	head->ldepth = global_depth;
	head->number = 0;
	head->p = (int*) malloc(bucket_size * sizeof(int));
	head->left = bucket_size;
	head->next = NULL;

	int i;
	if(global_depth > 0)
	{
		for(i=0;i<global_depth;i++)
			double_dir(&head);
	}

	pass = head->next;
	while(pass != NULL)
	{
		pass->ldepth = global_depth;
		pass->p = (int*) malloc(bucket_size * sizeof(int));
		pass = pass->next;
	}



	char *line = NULL;
	size_t len =0;
	ssize_t read;
	FILE *fp = fopen(argv[4],"r");
	if(fp ==0)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) 
	{

        //printf("Retrieved line of length %zu :\n", read);
      	// printf("\t%s\n", line);
      	// if(!strcmp(line,"status\n"))
      	// {
      	// 	printf("dadadda\n");
      	// }
		p[0] = strtok(line, " ");
		i = 0;

		while(p[i] != NULL)
		{
			i++;
			p[i] = strtok(NULL," ");
		}

		//printf("%s %s\n",p[0],p[1]);
		if(!strcmp(p[0],"insert"))
		{
			//printf("1\n");
			insert_data(atoi(p[1]),&head);
		}
		else if (!strcmp(p[0],"delete"))
		{
			//printf("2\n");
			if(delete_option==0)
				delete_1(atoi(p[1]),&head);
			else if (delete_option==1)
				delete_2(atoi(p[1]),&head);
			else if(delete_option==2)
				delete_3(atoi(p[1]),&head);
		}
		else if(!strcmp(p[0],"status\n"))
		{
			//printf("hihi\n");
			index_status(&head);
			//printf("dadada\n");
		}

		k=0;
		while(p[k] != NULL)
		{
			p[k] = NULL;
			k++;
		}
		k =0;
        
    }
    fclose(fp);
    if(line)
    	free(line);
    //index_status(&head);
    //printf("%d  %d  %d\n",global_depth,delete_option,bucket_size );
	// printf("%d\n",bucket_size );
	/*
	insert_data(4,&head);

	insert_data(12,&head);

	insert_data(32,&head);
	insert_data(16,&head);
	insert_data(1,&head);
	insert_data(5,&head);
	insert_data(21,&head);
	insert_data(10,&head);
	// printf("%d\n",bucket_size );
	insert_data(15,&head);
	insert_data(7,&head);
	insert_data(19,&head);
		// index_status(&head);
	insert_data(20,&head);
	insert_data(13,&head);
		// index_status(&head);
	// insert_data(9,&head);
		// index_status(&head);

	delete_3(5,&head);
	delete_3(15,&head);
	delete_3(7,&head);
	delete_3(19,&head);
	delete_3(12,&head);
	// insert_data(4,&head);
	// insert_data(12,&head);
	// insert_data(16,&head);
	// insert_data(25,&head);
	// insert_data(32,&head);


	index_status(&head);
	// int k = search(13,&head);
	// printf("%d\n", k);
*/


	return 0;
}