#ifndef __CYAML_CONF_H__
#define __CYAML_CONF_H

#include<cyaml/cyaml.h>

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


static const cyaml_schema_value_t stock_schema = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, Ingredient,
            ingredient_fields_schema),
};

static const cyaml_schema_value_t stock_sequence_schema = {
    CYAML_VALUE_SEQUENCE(CYAML_FLAG_POINTER, Ingredient,
        	&stock_schema, 0, CYAML_UNLIMITED),
};

static const cyaml_config_t config = {
	.log_level = CYAML_LOG_WARNING, 		// Logging errors and warnings only.
	.log_fn = cyaml_log,            		// Use the default logging function. 
	.mem_fn = cyaml_mem,            		// Use the default memory allocator. 
};

#endif
