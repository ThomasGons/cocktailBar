#ifndef __COCKTAIL_BAR_H__
#define __COCKTAIL_BAR_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<cyaml/cyaml.h>

#define COCKTAIL 19
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
	size_t nb_ingredient;
	Ingredient* ingredient;
}Cocktail;

typedef struct
{
	float volume, alcoholic, price, sugar;
}Specs;

typedef struct
{
	char* name;
	float quantity;
	float price;
}Stock;


static const cyaml_schema_field_t ingredient_fields_schema[] = {
	CYAML_FIELD_STRING_PTR("name", CYAML_FLAG_POINTER,
			Ingredient, name, 0, CYAML_UNLIMITED),
	CYAML_FIELD_FLOAT("quantity", CYAML_FLAG_DEFAULT,
			Ingredient, quantity),
	CYAML_FIELD_FLOAT("alcohol", CYAML_FLAG_DEFAULT,
			Ingredient, alcohol),
	CYAML_FIELD_FLOAT("sugar", CYAML_FLAG_DEFAULT,
			Ingredient, sugar),
	CYAML_FIELD_FLOAT("price", CYAML_FLAG_DEFAULT,
			Ingredient, price),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t ingredient_schema = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, Ingredient,
                        ingredient_fields_schema),
};

static const cyaml_schema_field_t cocktail_fields_schema[] = {
	CYAML_FIELD_STRING_PTR("name", CYAML_FLAG_POINTER,
			Cocktail, name, 0, CYAML_UNLIMITED),
	CYAML_FIELD_BOOL("alcoholic", CYAML_FLAG_DEFAULT,
			Cocktail, alcoholic),
	CYAML_FIELD_SEQUENCE_COUNT("ingredient", CYAML_FLAG_POINTER,
			Cocktail, ingredient, nb_ingredient,
			&ingredient_schema, 0, CYAML_UNLIMITED),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t cocktail_schema = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, Cocktail,
            cocktail_fields_schema),
};

static const cyaml_schema_value_t cocktail_sequence_schema = {
    CYAML_VALUE_SEQUENCE(CYAML_FLAG_POINTER, Cocktail,
        	&cocktail_schema, 0, CYAML_UNLIMITED),
};


static const cyaml_schema_field_t stock_fields_schema[] = {
	CYAML_FIELD_STRING_PTR("name", CYAML_FLAG_POINTER,
			Stock, name, 0, CYAML_UNLIMITED),
	CYAML_FIELD_FLOAT("quantity", CYAML_FLAG_DEFAULT,
			Stock, quantity),
	CYAML_FIELD_FLOAT("price", CYAML_FLAG_DEFAULT,
			Stock, price),
};

static const cyaml_schema_value_t stock_schema = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, Stock,
            stock_fields_schema),
};

static const cyaml_schema_value_t stock_sequence_schema = {
    CYAML_VALUE_SEQUENCE(CYAML_FLAG_POINTER, Stock,
        	&stock_schema, 0, CYAML_UNLIMITED),
};

static const cyaml_config_t config = {
	.log_level = CYAML_LOG_WARNING, 		// Logging errors and warnings only.
	.log_fn = cyaml_log,            		// Use the default logging function. 
	.mem_fn = cyaml_mem,            		// Use the default memory allocator. 
};

void yaml(Cocktail* cocktail, Stock* stock);
void menu(Cocktail* cocktail, Stock* stock);
void display_cocktail(Cocktail* cocktail, Stock* stock, bool value);
void select_(Cocktail* cocktail, Stock* stock, int* amount);
Specs specificity(Ingredient* ingredient, size_t nb_ingredient);
bool availability(Cocktail cocktail, Stock* stock);
void homemade(Stock* stock);
void quantity_Less(Ingredient *ingredient, Stock* stock, size_t nb_ingredient);
void stock_var(Stock* stock);

#endif
