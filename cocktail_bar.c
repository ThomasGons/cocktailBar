#include"cocktail_bar.h"

int main()
{	
	Cocktail* cocktail = NULL;
	Ingredient* stock = NULL;
	yaml(&cocktail, &stock, "load");
	int nb_cocktail = count_cocktail(cocktail);
	char check[25], who[25];	
	printf("Who are you:\n \t- bartender ?\n \t- customer(default) ?\n");
	scanf("%s", who);
	person = (strstr("bartender", who))? 1: 0;
	do
	{
		menu(cocktail, stock, nb_cocktail);
		printf("\nDo you want to make a cocktail (yes/no) ?\n");
		scanf("%s", check);
	}while (strstr("yes", check));
	(person != 1)? printf("It will make you %0.2f €\n", turnover): 0;
	printf("Have a nice day :)\n");
	
	cyaml_free(&config, &stock_sequence_schema, stock, 0);
	cyaml_free(&config, &cocktail_sequence_schema, cocktail, 0);
	return 0;
}


int count_cocktail(Cocktail* cocktail)
{
	int i = 0, nb_cocktail = 0;
	while(cocktail[i].name != NULL)
	{
		nb_cocktail++;
		i++;
	}
	return nb_cocktail;
}

void reset_yaml()
{
	Cocktail* init_cocktail;
	Ingredient* init_stock;
	cyaml_err_t  err_cocktail, err_stock;
	unsigned cocktail_count, stock_count;
	err_cocktail = cyaml_load_file("init_cocktail.yml", &config, &cocktail_sequence_schema,
			(void **)&init_cocktail, &cocktail_count);

	err_stock = cyaml_load_file("init_stock.yml", &config, &stock_sequence_schema,
			(void **)&init_stock, &stock_count);

	if (err_cocktail != CYAML_OK || err_stock != CYAML_OK)
		exit(EXIT_FAILURE);

	err_cocktail = cyaml_save_file("tmp_cocktail.yml", &config, &cocktail_sequence_schema,
			(void*)init_cocktail, __INIT_COCKTAIL);

	err_stock = cyaml_save_file("tmp_stock.yml", &config, &stock_sequence_schema,
			(void*)init_stock, STOCK);

	if (err_cocktail != CYAML_OK || err_stock != CYAML_OK)
		exit(EXIT_FAILURE);
}

void yaml(Cocktail** cocktail, Ingredient** stock, char* mode)
{
	cyaml_err_t err_cocktail, err_stock;
	unsigned cocktail_count, stock_count;
	err_cocktail = (!strcmp(mode, "load"))?cyaml_load_file("tmp_cocktail.yml", &config, &cocktail_sequence_schema,
			(void **)cocktail, &cocktail_count): cyaml_save_file("tmp_coktail.yml", &config, &cocktail_sequence_schema,
				(void*)*cocktail, count_cocktail(*cocktail));

	err_stock = (!strcmp(mode, "load"))?cyaml_load_file("tmp_stock.yml", &config, &stock_sequence_schema,
			(void **)stock, &stock_count): cyaml_save_file("tmp_stock.yml", &config, &stock_sequence_schema,
				(void*)*stock, STOCK);
	if (err_cocktail != CYAML_OK || err_stock != CYAML_OK)
		exit(EXIT_FAILURE);
}

void menu(Cocktail* cocktail, Ingredient* stock, int nb_cocktail)
{
	char choice[25]; 
	printf("\nDo you want to make:\n\t- an alcoholic cocktail,\n\t- non_alcoholic cocktail,\n\t- your own.\n");
	(person == 1)? printf("\t- modify the stock\n"): 0;
	scanf("%s", choice);
	//non_alcoholic sub_string of alcoholic
	if (strstr("alcoholic", choice))
		display_cocktail(cocktail, stock, true, nb_cocktail);
	else if(strstr("non_alcoholic", choice))
		display_cocktail(cocktail, stock, false, nb_cocktail);
	else if(strstr("own", choice))
		homemade(cocktail, stock);
	else if(strstr("modify", choice))
		stock_var(stock);
	else
	{
		printf("Sorry, you've entered a wrong choice.\n");
		menu(cocktail, stock, nb_cocktail);
	}
}	

//We display all the cocktails alcoholic or non-acloholic among the card according to the customer choice
void display_cocktail(Cocktail* cocktail, Ingredient* stock, bool value, int nb_cocktail)
{
	int amount[2];
	Specs* specs = malloc(nb_cocktail * sizeof(Specs));
	for (int i = 0; i < nb_cocktail; i++)
	{		
		//value refers to the absence (false) or presence (true) of alcohol in the cocktails
		specs[i] = specificity(cocktail[i].ingredient, cocktail[i].nb_ingredient);
		if (cocktail[i].alcoholic == value && cocktail[i].name != NULL && availability(cocktail[i], stock) == true)
			printf("\n- %s(%0.1f°, %0.1f g (sugar)), %0.2f cL, %0.2f €", cocktail[i].name,
					specs[i].alcoholic, specs[i].sugar, specs[i].volume, specs[i].price);
	}
	select_(cocktail, stock, amount, nb_cocktail);
	turnover += specs[amount[0]].price * amount[1];
	free(specs);
}

Specs specificity(Ingredient *ingredient, size_t nb_ingredient)
{
	float price = 0.0, volume = 0.0, sugar = 0.0, alcohol_volume = 0.0;
	for (size_t i = 0; i < nb_ingredient; i++)
	{
		price += ingredient[i].price * ingredient[i].quantity * 0.001;
		volume += ingredient[i].quantity;
		sugar += ingredient[i].sugar;
		if (ingredient[i].alcohol > 0)
			alcohol_volume += ingredient[i].quantity * ingredient[i].alcohol * 0.01;
	}
	Specs specs;
	specs.price = 1.1 * price;
	specs.volume = 0.1 * volume;
	specs.sugar = sugar;
	specs.alcoholic = alcohol_volume * 100 / volume;
	return specs;
}

//We get the cocktail chosen by the customer 
void select_(Cocktail* cocktail, Ingredient* stock, int* amount, int nb_cocktail)
{
	char selection[50], test[25];
	printf("\n\nWhich cocktail do you want ?\n\n\t");
	scanf("%s", selection);
	for (int i = 0; i < nb_cocktail; i++)
	{
		if (strstr(cocktail[i].name, selection))
		{
			if (availability(cocktail[i], stock) == false)
			{
				printf("\nSorry, I don't have enough ingredients to make a %s.\n", cocktail[i].name);
				select_(cocktail, stock, amount, nb_cocktail);
			}
			printf("\nA %s, very good choice it's my favorite cocktail.\n", cocktail[i].name);
			printf("How many %s do you want?\n", cocktail[i].name);
			scanf("%d", &amount[1]);
			amount[0] = i;
			if (person == 0)
				quantity_Less(cocktail[i].ingredient, stock, cocktail[i].nb_ingredient);
			else
			{
				printf("Do you want to see and/or modify the stock (yes/no)\n\n\t");
				scanf("%s", test);
				if (strstr("yes", test))
					stock_var(stock);
			}
			return;
		}
	}
	printf("\nExcuse me, I don't know this cocktail.\n");
	select_(cocktail, stock, amount, nb_cocktail);
}

//We see if the bartender can create the cocktail among the available ingredients in the stock
bool availability(Cocktail cocktail, Ingredient* stock)
{
	for (size_t i = 0; i < cocktail.nb_ingredient ; i++)
	{
		for (int j = 0; j < STOCK; j++)
		{
			if (cocktail.ingredient[i].name == stock[j].name && cocktail.ingredient[i].quantity > stock[j].quantity)
				return false;
		}
	}
	return true;
} 

void homemade(Cocktail* cocktail, Ingredient* stock)
{
	int i, count = 0;
	bool check = true;
	float quantity;
	char choice[25], test[25];
	Ingredient* p_ingredient = malloc(STOCK * sizeof(Ingredient));
	printf("List of Ingredients : \n");
	for(i = 0; i < STOCK; i++)
	{
		(stock[i].quantity != 0 )? printf("\n\t - %s : %0.2f € (kg/L)",stock[i].name,stock[i].price): 1;
	}
	do
	{
		while(check == true)
		{
			printf("\nWhat ingredient do you want to add?\n");
			scanf("%s", choice);
			for(i = 0; i < STOCK; i++)
			{
				if(!strcmp(stock[i].name, choice))
				{
					check = false;
					break;
				}
			}	
			if (check == false)
			{
				p_ingredient[count].price = stock[i].price;
				printf("What quantity of %s do you want ?\n", choice);
				scanf("%f", &quantity);
				if (quantity <= stock[i].quantity)
				{
					p_ingredient[count].name = malloc(strlen(choice) * sizeof(char));
					strcpy(p_ingredient[count].name, choice);
					p_ingredient[count].quantity = quantity;
					count++;
				}
				else
					printf("Sorry, we don't have enough %s\n", choice);
			}
			else
				printf("Sorry, I don't understand\n");
		}
		printf("Do you want another ingredient (yes/no) ?\n");
		scanf("%s", test);
	}while(strstr("yes", test));
	if (person == 0)
		quantity_Less(p_ingredient, stock, count);
	else
	{
		printf("\nDo you want to see and/or modify the stock (yes/no)\n\n\t");
		scanf("%s", test);
		if (strstr("yes", test))
			stock_var(stock);
	}
	Specs specs = specificity(p_ingredient, count);
	int amount;
	printf("\nHow many of your marvellous cocktail do you want ?\n");
	scanf("%d", &amount);
	turnover += specs.price * amount;
	printf("Do you want to add your cocktail to the list (yes/no)?\n");
	scanf("%s", test);
	if (strstr("yes", test))
		save_cocktail(cocktail, stock, p_ingredient, count);
}

void save_cocktail(Cocktail* cocktail, Ingredient* stock, Ingredient* p_ingredient, int size)
{
	int amongus = 0, nb_cocktail = count_cocktail(cocktail);
	if (realloc(cocktail, (nb_cocktail + 1) * sizeof(Cocktail)) == NULL)
		return;
	printf("Enter a name for your creation: ");
	scanf("%s", cocktail[nb_cocktail].name);
	printf("\nname: %s", cocktail[nb_cocktail].name);
	cocktail[nb_cocktail].ingredient = malloc(size * sizeof(Ingredient));
	for (int i = 0; i < size; i++)
	{
		cocktail[nb_cocktail].ingredient[i].name = p_ingredient[i].name;
		cocktail[nb_cocktail].ingredient[i].quantity = p_ingredient[i].quantity;	
		for (int j = 0; j < STOCK; j++)
		{
			if (cocktail[nb_cocktail].ingredient[i].name == stock[j].name)
			{
				cocktail[nb_cocktail].ingredient[i].alcohol = stock[j].alcohol;
				cocktail[nb_cocktail].ingredient[i].sugar = stock[j].sugar;
				cocktail[nb_cocktail].ingredient[i].price = stock[j].price * cocktail[nb_cocktail].ingredient[i].quantity * 0.001;
				if (cocktail[nb_cocktail].ingredient[i].alcohol > 0)
					amongus = 1;
				printf("\n\t- name: %s, \n\t- quantity: %f, \n\t- alcohol: %f, \n\t- sugar: %f, \n\tprice: %f", cocktail[nb_cocktail].ingredient[i].name,
						cocktail[nb_cocktail].ingredient[i].quantity, cocktail[nb_cocktail].ingredient[i].alcohol, cocktail[nb_cocktail].ingredient[i].sugar,
						cocktail[nb_cocktail].ingredient[i].price);
				break;
			}
		}
	}
	cocktail[nb_cocktail].alcoholic = (amongus == 1)? true: false;
	(cocktail[nb_cocktail].alcoholic == true)? printf("\nalcoholic: true"): printf("\nalcoholic: false");
	yaml(&cocktail, &stock, "save");
}

void quantity_Less(Ingredient* ingredient, Ingredient* stock, size_t nb_ingredient)
{
    for (size_t i = 0; i < nb_ingredient; i++)
    {
		for(int j = 0 ; j < STOCK ; j++)
        {
            if(strstr(stock[j].name, ingredient[i].name))
            {
				stock[j].quantity -= ingredient[i].quantity;
			}
		}
	}
}

void stock_var(Ingredient* stock)
{
	int i;
	float quantity;
	char test[25], ingredient[25], modify[25];
	printf("\n The stock contains:\n");
	for(i = 0; i < STOCK; i++)
	{
		printf("\t - %s (%0.3f kg/L): %0.2f €\n",stock[i].name, stock[i].quantity / 1000, stock[i].price);
	}
	do
	{
		printf("\nYou want to change the stock of which ingredient:\n\t");
		scanf("%s", ingredient);
		for (i = 0; i < STOCK; i++)
		{
			if (strstr(stock[i].name, ingredient))
			{
				printf("\nYou want to: \n\t- add some %s (default) ;\n\t- remove some %s;\n", stock[i].name, stock[i].name);
				scanf("%s", modify);
				do
				{
					printf("\nWhich quantity ?\n\t");
					scanf("%f", &quantity);
					if (quantity > stock[i].quantity && strstr("remove", modify))
						printf("There is just %0.3f Kg %s in the stock", stock[i].quantity * 0.001, stock[i].name);
				}while(quantity > stock[i].quantity && strstr("remove", modify));
				stock[i].quantity = (strstr("remove", modify))? stock[i].quantity - quantity: stock[i].quantity + quantity;
				quantity = 0.0;
			}
		}
		printf("\nDo you want to continue to modify the stock (yes/no)\n\t");
		scanf("%s", test);
	}while(strstr("yes", test));
	printf("\nNow the stock contains:\n");
	for(i = 0; i < STOCK; i++)
	{
		printf("\t - %s (%0.3f Kg/L): %0.2f €\n",stock[i].name, stock[i].quantity / 1000, stock[i].price);
	}
}

