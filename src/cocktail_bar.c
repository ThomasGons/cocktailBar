/*!
\file cocktail_bar.c
\author Gons Thomas, Alzoubaidy Maxime, Laghzaoui Marwane
\version final
\date 21/05/2021
\brief file containing all the fonctions of the program
*/

#include"cocktail_bar.h"
#include"cyaml_conf.h"

/*! 
\fn int main()
\author Gons Thomas, Alzoubaidy Maxime, Laghzaoui Marwane
\version final
\date 21/05/2021
\brief The main fonction which contains the calls of the functions of the program
\return 0 if it worked properly
*/

int main()
{	
	char check[25], who[25], reset[25];	
	printf("Who are you:\n \t- bartender ?\n \t- customer(default) ?\n");
	scanf("%s", who); 
	person = (strstr("bartender", who))? 1: 0; // person is a global variable which indicates if it's the bartender(1) or a customer(0)	
	if (person == 1)
	{
		printf("Do you want to reset the cocktail card and the stock (yes/no) ?\n"); // replace the temporary content's files by the init content's files
		scanf("%s", reset);
		if (strstr("yes", reset)) 
			reset_yaml();
	}
	// declaration and initialization of cocktail and stock sequences 
	Cocktail* cocktail = NULL;
	Ingredient* stock = NULL;
	int nb_cocktail = yaml(&cocktail, &stock, "load", 0);  //loading of cocktails and stock's elements
	Order *order = malloc(sizeof(Order));  // Initialization of the order which contains cocktails and number of them
	order->content = malloc(sizeof(Cocktail)); // Initialization of the cocktail sequence at one cocktail
	order->amount = malloc(sizeof(int));
	order->size = 0;
	// while user want to choose a cocktail or modify the stock 
	do
	{
		menu(cocktail, stock, order, nb_cocktail);
		printf("\nDo you want to make a cocktail (yes/no) ?\n");
		scanf("%s", check);
	}while (strstr("yes", check));
	if (order->size > 0) // if order contains at least one element
	{
		order_var(order);
		(person != 1)? printf("It will make you %0.2f $\n", turnover): 0; // display of the final turnover
	}
	printf("Have a nice day :)\n");
	free(order->content);
	return 0;
}

/*!
\fn reset_yaml
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief replace the temporary content's file by the init content's
*/

void reset_yaml()
{
	Cocktail* init_cocktail;
	Ingredient* init_stock;
	cyaml_err_t  err_cocktail, err_stock;
	unsigned cocktail_count, stock_count;
	// loading of the init cocktails and stock with init yaml files
	err_cocktail = cyaml_load_file("config/init_cocktail.yml", &config, &cocktail_sequence_schema,
			(void **)&init_cocktail, &cocktail_count);

	err_stock = cyaml_load_file("config/init_stock.yml", &config, &stock_sequence_schema,
			(void **)&init_stock, &stock_count);
	// allow to raise error of loading and/or saving 
	if (err_cocktail != CYAML_OK || err_stock != CYAML_OK)
		exit(EXIT_FAILURE);
	/*saving data in the temporaries files allows to overwrite and
	thus reset temporaries files which can contains homemade cocktails added previously*/
	err_cocktail = cyaml_save_file("config/tmp_cocktail.yml", &config, &cocktail_sequence_schema,
			(void*)init_cocktail, __INIT_COCKTAIL);

	err_stock = cyaml_save_file("config/tmp_stock.yml", &config, &stock_sequence_schema,
			(void*)init_stock, STOCK);

	if (err_cocktail != CYAML_OK || err_stock != CYAML_OK)
		exit(EXIT_FAILURE);
}

/*!
\fn yaml ( Cocktail** cocktail, Ingredient** stock, char* mode )
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief load or save temporary content's file
\param *cocktail, table containing all our cocktails
\param *stock, table containing the stocks of each ingredient.
\param *mode allows to choose between saving a cocktail or create a cocktail
*/

int yaml(Cocktail** cocktail, Ingredient** stock, char* mode, int nb_cocktail)
{
	cyaml_err_t err_cocktail, err_stock;
	unsigned cocktail_count, stock_count; 
	// loading or saving structure's data according to the mode 
	err_cocktail = (!strcmp(mode, "load"))?cyaml_load_file("config/tmp_cocktail.yml", &config, &cocktail_sequence_schema,
			(void **)cocktail, &cocktail_count): cyaml_save_file("config/tmp_cocktail.yml", &config, &cocktail_sequence_schema,
				(void*)*cocktail, nb_cocktail);

	err_stock = (!strcmp(mode, "load"))?cyaml_load_file("config/tmp_stock.yml", &config, &stock_sequence_schema,
			(void **)stock, &stock_count): cyaml_save_file("config/tmp_stock.yml", &config, &stock_sequence_schema,
				(void*)*stock, STOCK);
	// allow to raise error of loading and/or saving 
	if (err_cocktail != CYAML_OK || err_stock != CYAML_OK)
		exit(EXIT_FAILURE);
	return (int)cocktail_count;
}

/*!
\fn menu ( Cocktail* cocktail, Ingredient* stock, Order* order, int nb_cocktail )
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief function allowing the user to choose the type of cocktail he wants to order
\param *cocktail, table containing all our cocktails
\param *stock, array of ingredients containing the stock of each ingredient.
\param *order, array containing the user's order
\param nb_cocktail indicates the number of existing cocktails
*/

void menu(Cocktail* cocktail, Ingredient* stock, Order* order, int nb_cocktail)
{
	char choice[25]; 
	printf("\nDo you want to make:\n\t- an alcoholic cocktail,\n\t- non_alcoholic cocktail,\n\t- your own.\n");
	(person == 1)? printf("\t- modify the stock\n"): 0;
	scanf("%s", choice);
	// simple menu in order to manage the entered choice by the user
	if (strstr("alcoholic", choice))
		display_cocktail(cocktail, stock, order, true, nb_cocktail);
	else if(strstr("non_alcoholic", choice))
		display_cocktail(cocktail, stock, order, false, nb_cocktail);
	else if(strstr("own", choice))
		homemade(cocktail, stock, order, nb_cocktail);
	// only the bartender can add or remove elements for the stock
	else if(strstr("modify", choice) && person == 1)
		stock_var(stock);
	else
	{
		printf("Sorry, you've entered a wrong choice.\n");
		menu(cocktail, stock, order, nb_cocktail);
	}
}	

/*!
\fn display_cocktail ( Cocktail* cocktail, Ingredient* stock, Order* order, bool value, int nb_cocktail )
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief function that displays all available cocktails according to the previous choice, alcoholic or non alcoholic
\param *cocktail, array containing all our cocktails
\param *stock, table containing the stocks of each ingredient.
\param *order array containing the user's order
\param value a boolean containing 1 or 0, 1 for cocktail with alcohol and 0 for cocktail without alcohol
\param nb_cocktail indicates the number of existing cocktails
*/

void display_cocktail(Cocktail* cocktail, Ingredient* stock, Order* order, bool value, int nb_cocktail)
{
	Specs* specs = malloc(nb_cocktail * sizeof(Specs));
	for (int i = 0; i < nb_cocktail; i++)
	{		
		// "value" refers to the absence (false) or presence (true) of alcohol in the cocktails
		specs[i] = specificity(cocktail[i].ingredient, cocktail[i].nb_ingredient);
		if (cocktail[i].alcoholic == value && cocktail[i].name != NULL && availability(cocktail[i], stock) == true)
			printf("\n- %s(%0.1f°, %0.1f g (sugar)), %0.2f cL, %0.2f $", cocktail[i].name,
					specs[i].alcoholic, specs[i].sugar, specs[i].volume, specs[i].price);
	}
	// after having display correct cocktails the user select his cocktail
	select_(cocktail, stock, order, nb_cocktail);
	free(specs);
}

 /*
 \fn Specs specificity ( Ingredient *ingredient, unsigned nb_ingredient )
 \author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
 \version final
 \date May 21, 2021
 \brief Function whose objective is to calculate the information of a cocktail via a list of ingredient given in parameter.
 \param *ingredient array containing a list of ingredients used to create a cocktail. This array contains the information for each ingredient.
 \param nb_ingredient number of ingredients that make up the ingredient array.
 \return specs the information of a cocktail, price, volume, sugar/alcohol rate.
 */

Specs specificity(Ingredient *ingredient, unsigned nb_ingredient)
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
	// we get some informations about the cocktail such as his price, volume, sugar, alcoholic
	return specs;
}

/*!
\fn select_ ( Cocktail* cocktail, Ingredient* stock, Order* order, int nb_cocktail )
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief allows the user to choose a cocktail which will be then put in his order.
\param *cocktail, table containing all our cocktails
\param *stock, table containing the stocks of each ingredient.
\param *order, array containing the user's order
\param nb_cocktail indicates the number of existing cocktails
*/

void select_(Cocktail* cocktail, Ingredient* stock, Order* order, int nb_cocktail)
{
	int j;
	char selection[50], test[25];
	printf("\n\nWhich cocktail do you want ?\n\n\t");
	scanf("%s", selection);
	for (int i = 0; i < nb_cocktail; i++)
	{
		if (strstr(cocktail[i].name, selection))
		{
			// if cocktail is not available we call recursively select_ until an available cocktail entered in
			if (availability(cocktail[i], stock) == false)
			{
				printf("\nSorry, I don't have enough ingredients to make a %s.\n", cocktail[i].name);
				select_(cocktail, stock, order, nb_cocktail);
			}
			printf("\nA %s, very good choice it's my favorite cocktail.\n", cocktail[i].name);
			if (order->content == NULL)
				return;
			for (j = 0 ; j < order->size; j++)
			{
				if (!strcmp(order->content[j].name, cocktail[i].name))
				{	
					order->amount[j]++;
					break;
				}		
			}
			if(j == order->size)
			{
				order->size++;
				order->amount = realloc(order->amount, order->size * sizeof(int));
				order->amount[order->size - 1] = 1;
				// resize content to put a new cocktail in the order
				order->content = realloc(order->content, order->size * sizeof(Cocktail));
				order->content[order->size - 1] = cocktail[i];
			}
			// remove ingredients used for the cocktail from the stock in their quantity
			quantity_Less(cocktail[i].ingredient, stock, cocktail[i].nb_ingredient);
			// only bartender can modify the stock
			if (person == 1)
			{
				printf("Do you want to see and/or modify the stock (yes/no)\n\n\t");
				scanf("%s", test);
				if (strstr("yes", test))
					// function allowing to modify the stock
					stock_var(stock);
			}
			return;
		}
	}
	printf("\nExcuse me, I don't know this cocktail.\n");
	select_(cocktail, stock, order, nb_cocktail);
}

/*!
\fn availability ( Cocktail cocktail, Ingredient* stock )
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief verifies if the chosen cocktail is available or not
\param cocktail it is the cocktail chosen in the function select_ or homemade.
\param *stock, ingredient array containing the stocks of each ingredient.
\return bool returns a boolean to know if a cocktail is available or not
*/

bool availability(Cocktail cocktail, Ingredient* stock)
{
	for (size_t i = 0; i < cocktail.nb_ingredient ; i++)
	{
		for (int j = 0; j < STOCK; j++)
		{
			// if at least one ingredient of the cocktail isn't in good quantities, it is unavailable     
			if (cocktail.ingredient[i].name == stock[j].name && cocktail.ingredient[i].quantity > stock[j].quantity)
				return false;
		}
	}
	return true;
} 

/*!
\fn homemade ( Cocktail* cocktail, Ingredient* stock, Order* order )
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief allows the user to create his cocktail according to a list and a quantity of available ingredients
\param *cocktail, table containing all our cocktails
\param *stock, table containing the stocks of each ingredient.
\param *order, table containing the user's order
*/

void homemade(Cocktail* cocktail, Ingredient* stock, Order* order, int nb_cocktail)
{
	int i, count = 0;
	float quantity;
	char choice[25], test[25];
	// array which will contain the ingredients of the homemade cocktail
	Ingredient* p_ingredient = malloc(STOCK * sizeof(Ingredient));
	printf("List of Ingredients : \n");
	for(i = 0; i < STOCK; i++)
	{
		// only display the available ingredient(s)
		(stock[i].quantity != 0)? printf("\n\t - %s : %0.2f $ (kg/L)",stock[i].name,stock[i].price): 1;
	}
	do
	{
		printf("\nWhat ingredient do you want to add?\n");
		scanf("%s", choice);
		for(i = 0; i < STOCK; i++)
		{
			if(!strcmp(stock[i].name, choice))
			{
				printf("What quantity of %s do you want ?\n", choice);
				scanf("%f", &quantity);
				// if there is not enough quantity of the ingredient
				if (quantity <= stock[i].quantity)
				{
					p_ingredient[count].name = malloc(25 * sizeof(char));
					strcpy(p_ingredient[count].name, choice);
					p_ingredient[count].quantity = quantity;
					p_ingredient[count].price = stock[i].price * p_ingredient[count].quantity * 0.001;
					count++;
				}
				else
					printf("Sorry, we don't have enough %s\n", choice);
				break;
			}
		}
		// if choosen element is not in the stock sequence
		(i == STOCK)? printf("Sorry I don't understand\n"): 0;
		printf("Do you want another ingredient (yes/no) ?\n");
		scanf("%s", test);
	}while(strstr("yes", test)); 
	// remove ingredients used for the homemade cocktail from the stock in their quantity
	quantity_Less(p_ingredient, stock, count);
	if (person == 1)
	{
		printf("\nDo you want to see and/or modify the stock (yes/no)\n\n\t");
		scanf("%s", test);
		if (strstr("yes", test))
			stock_var(stock);
	}
	printf("Do you want to add your cocktail to the list (yes/no)?\n");
	scanf("%s", test);
	// either the homemade cocktail is simply created or it is created and saved in the list of cocktails to be reused after
	if (strstr("yes", test))
		save_cocktail(cocktail, stock, p_ingredient, order, "save", nb_cocktail, count);
	else
		save_cocktail(cocktail, stock, p_ingredient, order, "create", nb_cocktail, count);
	for (i = 0; i < count; i++)
	{
		free(p_ingredient[i].name);
	}
	free(p_ingredient);
}

/*!
\fn save_cocktail ( Cocktail* cocktail, Ingredient* stock, Ingredient* p_ingredient, Order* order, char* mode, int size )
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief function allowing to save a cocktail created via the homemade function. The user can then save the cocktail for future use of the program or simply save the cocktail for the display of the order.
\param *cocktail, table containing all our cocktails
\param *stock, table containing the stocks of each ingredient.
\param *p_ingredient it is the cocktail created, this table contains all the ingredients chosen and a certain quantity
\param *order array containing the user's order
\param *mode allows to choose between creating and saving a cocktail or just creating a cocktail
\param size is the number of ingredients in the cocktail created.
*/

void save_cocktail(Cocktail* cocktail, Ingredient* stock, Ingredient* p_ingredient, Order* order, char* mode, int nb_cocktail, int size)
{
	// amongus is counter and also a very good easter egg <3
	int amongus = 0;
	Cocktail cocktail_spe;
	order->size++;
	order->amount = realloc(order->amount , order->size * sizeof(int));
	order->amount[order->size - 1] = 1;
	// resize content to put a new cocktail in the order
	order->content = realloc(order->content, order->size * sizeof(Cocktail));
	// initialization of the new cocktail cocktail_spe and last cocktail of the order
	cocktail_spe.name = malloc(25 * sizeof(char));
	order->content[order->size - 1].name = malloc(25 * sizeof(char));
	printf("Enter a name for your creation: ");
	scanf("%s", cocktail_spe.name);
	order->content[order->size - 1].name = cocktail_spe.name;
	cocktail_spe.nb_ingredient = size;
	order->content[order->size - 1].nb_ingredient = size;
	cocktail_spe.ingredient = malloc(size * sizeof(Ingredient));
	order->content[order->size - 1].ingredient = malloc(size * sizeof(Ingredient));	
	for (int i = 0; i < size; i++)
	{
		cocktail_spe.ingredient[i].name = p_ingredient[i].name;
		cocktail_spe.ingredient[i].quantity = p_ingredient[i].quantity;
		for (int j = 0; j < STOCK; j++)
		{
			if (!strcmp(cocktail_spe.ingredient[i].name, stock[j].name))
			{
				cocktail_spe.ingredient[i].alcohol = stock[j].alcohol;
				cocktail_spe.ingredient[i].sugar = stock[j].sugar;
				cocktail_spe.ingredient[i].price = stock[j].price;
				// if there is at least one ingredient which contains alcohol then cocktail is alcoholic
				if (cocktail_spe.ingredient[i].alcohol > 0)
					amongus = 1;
				break;
			}
		}
		order->content[order->size - 1].ingredient[i] = cocktail_spe.ingredient[i];
	}
	cocktail_spe.alcoholic = (amongus == 1)? true: false;
	order->content[order->size - 1].alcoholic = (amongus == 1)? true: false;
	if (!strcmp(mode, "save"))
	{
		// resize cocktail sequence to host the new cocktail 
		cocktail = realloc(cocktail, (nb_cocktail + 1) * sizeof(Cocktail));
		if (cocktail == NULL)
			return;
		// put the new cocktail in cocktail sequence
		cocktail[nb_cocktail].name = cocktail_spe.name;
		cocktail[nb_cocktail].alcoholic = cocktail_spe.alcoholic;
		cocktail[nb_cocktail].nb_ingredient = size;
		cocktail[nb_cocktail].ingredient = malloc(size * sizeof(Ingredient));
		for (int i = 0; i < size; i++)
		{
			cocktail[nb_cocktail].ingredient[i] = cocktail_spe.ingredient[i];
		}
		// save cocktail sequence with the new cocktail in order to add it to the temporary file cocktail
		yaml(&cocktail, &stock, "save", nb_cocktail + 1);
	}
	else
	{
		cyaml_free(&config, &cocktail_sequence_schema, cocktail, 0);
		cyaml_free(&config, &stock_sequence_schema, stock, 0);
	}
	free(cocktail_spe.name);
	free(cocktail_spe.ingredient);
}

/*!
\fn quantity_Less ( Ingredient* ingredient, Ingredient* stock, size_t nb_ingredient )
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief decreases the quantity of ingredients in our stock according to the cocktail sent in parameter
\param *ingredient array containing all the ingredients of the chosen cocktail
\param *stock, array of ingredients containing the stocks of each ingredient.
\param nb_ingredient number of ingredients of the chosen cocktail.
*/

void quantity_Less(Ingredient* ingredient, Ingredient* stock, size_t nb_ingredient)
{
    for (size_t i = 0; i < nb_ingredient; i++)
    {
		for(int j = 0 ; j < STOCK ; j++)
        {
            // simply remove quantity of used ingredients
			if(strstr(stock[j].name, ingredient[i].name))
				stock[j].quantity -= ingredient[i].quantity;
		}
	}
}

/*!
\fn stock_var ( Ingredient* stock )
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief allows the bartender to modify the ingredient quantities, he can then increase or decrease the available quantity of each ingredient.
\param *stock, ingredient table containing the stocks of each ingredient.
*/

void stock_var(Ingredient* stock)
{
	int i;
	float quantity;
	char test[25], ingredient[25], modify[25];
	printf("\n The stock contains:\n");
	for(i = 0; i < STOCK; i++)
	{
		printf("\t - %s (%0.3f kg/L): %0.2f $\n", stock[i].name, stock[i].quantity / 1000, stock[i].price);
	}
	do
	{
		printf("\nYou want to change the stock of which ingredient:\n\t");
		scanf("%s", ingredient);
		for (i = 0; i < STOCK; i++)
		{
			if (!strcmp(ingredient,stock[i].name))
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
		printf("\t - %s (%0.3f Kg/L): %0.2f $\n",stock[i].name, stock[i].quantity / 1000, stock[i].price);
	}
}

/*!
\fn order_var ( Order* order )
\author GonsThomas AlzoubaidyMaxime LaghzaouiMarwane
\version final
\date May 21, 2021
\brief Function whose objective is to display and manipulate the order, i.e.
to increase the number of cocktail to order or to decrease it as well as to cancel its order (by taking nothing for example)
\param *order Cocktail table containing the cocktails chosen by the user. This is the order.
\return Does not return anything, it is a void so we manipulate the order array directly.
*/

void order_var(Order* order)
{
	int  i, quantity;
	// creation a sequence of Specs to characterize cocktails in the order 
	Specs specs[order->size];
	printf("Here is your order:\n");
	for (i = 0; i < order->size; i++)
	{
		specs[i] = specificity(order->content[i].ingredient, order->content[i].nb_ingredient);
		// display some useful informations about the cocktails 
		printf("\t- %s(%d), %0.2f $\n", order->content[i].name, order->amount[i], order->amount[i] * specs[i].price);
	}
	char choice[25], change[25];   
	printf("Do you want to modify your order (yes/no)?\n");
	scanf("%s", choice);
	if (strstr("yes", choice))
	{
		do{
			printf("You want to change the amount of which cocktail ?\n");
			scanf("%s", choice);
			for(i = 0; i < order->size; i++)
			{
				if (strstr(order->content[i].name, choice))
				{
					printf("How many %s do you want to add(+) or to remove(-) ?\n", order->content[i].name);
					scanf("%d", &quantity);
					order->amount[i] = (order->amount[i] + quantity < 0)? 0: order->amount[i] + quantity;
					break;
				}
			}
			// if the entered cocktail is not recognized
			(i == order->size)? printf("Sorry, this cocktail is not part of your order\n"): 0;
			printf("Do you want to continue to modify your order (yes/no) ?\n");
			scanf("%s", change);
		}while(!strcmp(change, "yes"));
	}
	printf("Here is your order now:\n");
	for (i = 0; i < order->size; i++)
	{	
		// recall of specificity if a cocktail has been removed 
		specs[i] = specificity(order->content[i].ingredient, order->content[i].nb_ingredient);
		printf("\t- %s(%d), %0.2f $\n", order->content[i].name, order->amount[i], (specs[i].price * order->amount[i]));
		turnover += specs[i].price * order->amount[i];
	}
}

