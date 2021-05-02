#include"cocktail_bar.h"

int main()
{	
    Cocktail* cocktail = NULL;
	Stock* stock = NULL;
	yaml(cocktail, stock);

	char check[25], who[25];	
	printf("Who are you:\n \t- bartender ?\n \t- customer(default) ?\n");
	scanf("%s", who);
	person = (strstr("bartender", who))? 1: 0;
	do
	{
		menu(cocktail, stock);
		printf("\nDo you want to make a cocktail (yes/no) ?\n");
		scanf("%s", check);
	}while (strstr("yes", check));
	(person != 1)? printf("It will make you %0.2f €\n", turnover): 0;
	printf("Have a nice day :)\n");
	
	cyaml_free(&config, &stock_sequence_schema, stock, 0);
	cyaml_free(&config, &cocktail_sequence_schema, cocktail, 0);
	return 0;
}

void yaml(Cocktail* cocktail, Stock* stock)
{
	cyaml_err_t err_cocktail, err_stock;
	unsigned cocktail_count, stock_count;
	err_cocktail = cyaml_load_file("cocktail.yml", &config, &cocktail_sequence_schema,
			(void **)&cocktail, &cocktail_count);

	err_stock = cyaml_load_file("stock.yml", &config, &stock_sequence_schema,
			(void **)&stock, &stock_count);

	if (err_cocktail != CYAML_OK || err_stock != CYAML_OK)
		exit(EXIT_FAILURE);
}

void menu(Cocktail* cocktail, Stock* stock)
{
	char choice[25]; 
	printf("\nDo you want to make:\n\t- an alcoholic cocktail,\n\t- non_alcoholic cocktail,\n\t- your own.\n");
	(person == 1)? printf("\t- modify the stock\n"): 0;
	scanf("%s", choice);
	//non_alcoholic sub_string of alcoholic
	if (strstr("alcoholic", choice))
		display_cocktail(cocktail, stock, true);
	else if(strstr("non_alcoholic", choice))
		display_cocktail(cocktail, stock, false);
	else if(strstr("own", choice))
		homemade(stock);
	else if(strstr("modify", choice))
		stock_var(stock);
	else
	{
		printf("Sorry, you've entered a wrong choice.\n");
		menu(cocktail, stock);
	}
}	

//We display all the cocktails alcoholic or non-acloholic among the card according to the customer choice
void display_cocktail(Cocktail* cocktail, Stock* stock, bool value)
{
	int amount[2];
	Specs* specs = malloc(COCKTAIL * sizeof(Specs));
	for (int i = 0; i < COCKTAIL; i++)
	{		
		//value refers to the absence (false) or presence (true) of alcohol in the cocktails
		specs[i] = specificity(cocktail[i].ingredient, cocktail[i].nb_ingredient);
		if (cocktail[i].alcoholic == value && cocktail[i].name != NULL && availability(cocktail[i], stock) == true)
			printf("\n- %s(%0.1f°, %0.1f g (sugar)), %0.2f cL, %0.2f €", cocktail[i].name,
					specs[i].alcoholic, specs[i].sugar, specs[i].volume, specs[i].price);
	}
	select_(cocktail, stock, amount);
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
void select_(Cocktail* cocktail, Stock* stock, int* amount)
{
	char selection[50], test[25];
	printf("\n\nWhich cocktail do you want ?\n\n\t");
	scanf("%s", selection);
	for (int i = 0; i < COCKTAIL; i++)
	{
		if (strstr(cocktail[i].name, selection))
		{
			if (availability(cocktail[i], stock) == false)
			{
				printf("\nSorry, I don't have enough ingredients to make a %s.\n", cocktail[i].name);
				select_(cocktail, stock, amount);
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
	select_(cocktail, stock, amount);
}

//We see if the bartender can create the cocktail among the available ingredients in the stock
bool availability(Cocktail cocktail, Stock* stock)
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

void homemade(Stock* stock)
{
	int i, amount, count = 0;
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
				if(strstr(stock[i].name, choice))
				{
					check = false;
					break;
				}
			}	
			if (check == true)
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
	printf("\nHow many of your marvellous cocktail do you want ?\n");
	scanf("%d", &amount);
	turnover += specs.price * amount;
}

void quantity_Less(Ingredient* ingredient, Stock* stock, size_t nb_ingredient)
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

void stock_var(Stock* stock)
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

