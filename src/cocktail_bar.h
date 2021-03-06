#ifndef __COCKTAIL_BAR_H__
#define __COCKTAIL_BAR_H__

/*!
\file cocktail_bar.h
\author Gons Thomas, Alzoubaidy Maxime, Laghzaoui Marwane
\version final
\date 21/05/2021
\brief file containing all the structures
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<gtk/gtk.h>

#define __INIT_COCKTAIL 19
#define STOCK 35


float turnover = 0;
int person;

typedef struct
{
	float quantity, alcohol, sugar, price;
	char* name;
}Ingredient;

typedef struct
{	
	char* name;
	bool alcoholic;
	unsigned nb_ingredient;
	Ingredient* ingredient;
}Cocktail;

typedef struct
{
	float volume, alcoholic, price, sugar;
}Specs;

typedef struct
{
	Cocktail* content;
	int* amount;
	int size;
}Order;


void reset_yaml();
int yaml(Cocktail** cocktail, Ingredient** stock, char* mode, int nb_cocktail);
void menu(Cocktail* cocktail, Ingredient* stock, Order* order, int nb_cocktail);
void display_cocktail(Cocktail* cocktail, Ingredient* stock, Order* order, bool value, int nb_cocktail);
void select_(Cocktail* cocktail, Ingredient* stock, Order* order, int nb_cocktail);
Specs specificity(Ingredient* ingredient, unsigned nb_ingredient);
bool availability(Cocktail cocktail, Ingredient* stock);
void homemade(Cocktail* cocktail, Ingredient* stock, Order* order, int nb_cocktail);
void save_cocktail(Cocktail* cocktail, Ingredient* stock, Ingredient* p_ingredient, Order* order, char* mode, int nb_cocktail, int size);
void quantity_Less(Ingredient *ingredient, Ingredient* stock, size_t nb_ingredient);
void stock_var(Ingredient* stock);
void order_var(Order* order);

#endif
