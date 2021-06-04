import yaml as yml

class Ingredient:
    def __init__(self, name, alcohol, quantity, sugar, price):
        self.name = name
        self.alcohol = alcohol
        self.quantity = quantity
        self.sugar = sugar
        self.price = price
    @classmethod
    def from_yaml(cls, y):
        return cls(
            y['name'],
            y['alcohol'],
            y['quantity'],
            y['sugar'],
            y['price']
        )
class Cocktail:
    def __init__(self, name, alcoholic, *ingredients):
        self.name = name
        self.alcoholic = alcoholic
        self.ingredients = ingredients
    @classmethod
    def from_yaml(cls, y):
        return cls(
            y['name'],
            y['alcoholic'],
            *[Ingredient.from_yaml(i) for i in y['ingredient']] 
        )

class Order:
    def __init__(self):
        self.content = []
        self.amount = []
        self.size = 0


#global vars:

person, turnover = 0, 0


def yaml(cocktail, stock):
    with open(r'cocktail.yml') as yml_cocktail, open(r'stock.yml') as yml_stock:
        loaded_cocktail = yml.load(yml_cocktail, Loader = yml.FullLoader)
        loaded_stock = yml.load(yml_stock, Loader = yml.FullLoader)
        for i in range(len(loaded_cocktail)):
            cocktail.append(Cocktail.from_yaml(loaded_cocktail[i]))
        for i in range(len(loaded_stock)):
            stock.append(Ingredient.from_yaml(loaded_stock[i]))

    return len(loaded_cocktail)


def main():
    who = input("Who are you:\n \t- bartender ?\n \t- customer(default) ?\n")
    person = 1 if who in "bartender" else 0
    if person == 1:
        reset = input("Do you want to reset the cocktail card and the stock (yes/no) ?\n")
        if reset in "yes": 
            reset_yaml()
    cocktail, stock = [], []
    nb_cocktail = yaml(cocktail, stock)
    order = [Order()]
    while True:
        # menu(cocktail, stock, order, nb_cocktail)
        if input("\nDo you want to make a cocktail (yes/no) ?\n") not in "yes":
            break

    if len(order) > 0: 
        # order_var(order)
        if not person:
            print("It will make you {}$\n".format(turnover))
    print("Have a nice day :)\n")
    return 0

main()