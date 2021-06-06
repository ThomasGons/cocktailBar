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

class Specs:
    def __init__(self):
        self.alcohol = 0
        self.volume = 0
        self.sugar = 0
        self.price = 0
class Order:
    def __init__(self):
        self.content = []
        self.amount = []


#global vars:

person, turnover = 0, 0


def yaml(cocktail, stock):
    with open(r'cocktail.yml') as yml_cocktail, open(r'stock.yml') as yml_stock:
        loaded_cocktail = yml.load(yml_cocktail, Loader = yml.FullLoader)
        loaded_stock = yml.load(yml_stock, Loader = yml.FullLoader)
        return [[Cocktail.from_yaml(i) for i in loaded_cocktail],
        [Ingredient.from_yaml(i) for i in loaded_stock]]


def main():
    global person
    who = input("Who are you:\n \t- bartender ?\n \t- customer(default) ?\n")
    if who in "bartender":
        person = 1
        if input("Do you want to reset the cocktail card and the stock (yes/no) ?\n") in "yes":
            reset_yaml()
    cocktail, stock = yaml([], [])
    order = Order()
    while True:
        menu(cocktail, stock, order)
        if input("\nDo you want to make a cocktail (yes/no) ?\n") not in "yes":
            break

    if len(order.content) > 0: 
        # order_var(order)
        if not person:
            print("It will make you {}$\n".format(turnover))
    print("Have a nice day :)\n")
    return 0

def menu(cocktail, stock, order):
    choice = input("Do you want to make:\n\t- an alcoholic cocktail,\n\t- a non alcoholic cocktail,\n\t- your own\n" +
    ("\t- modify the stock\n" if person == 1 else ""))
    if choice in "alcoholic":
        displayCocktail(cocktail, stock, order, True)
    elif choice in "non alcoholic":
        displayCocktail(cocktail, stock, order, False)
    elif choice in "own":
        homemade(cocktail, stock, order)
    elif choice in "modify" and person == 1:
        stock_var(stock)
    else:
        print("Sorry, you've entered a wrong choice.\n")
        menu(cocktail, stock, order)

def displayCocktail(cocktail, stock, order, value):
    specs = []
    for i in range(len(cocktail)):
        specs.append(specificity(cocktail[i].ingredients))
        if cocktail[i].alcoholic == value and cocktail[i].name != None and availability(cocktail[i].ingredients, stock) == True:
            print("\n- {0}({1:.1f}°, {2:.1f}° g (sugar)), {3:.2f} cL, {4:.2f} $".format(cocktail[i].name,
					specs[i].alcohol, specs[i].sugar, specs[i].volume, specs[i].price))
    select(cocktail, stock, order)


def specificity(ingredients):
    specs = Specs()
    for i in range(len(ingredients)):
        specs.price += ingredients[i].price * ingredients[i].quantity * 0.001
        specs.volume += ingredients[i].quantity
        specs.sugar += ingredients[i].sugar
        if ingredients[i].alcohol > 0:
            specs.alcohol += ingredients[i].quantity * ingredients[i].alcohol * 0.01
    specs.price *= 1.1
    specs.alcohol *= 100 / specs.volume
    return specs

def availability(ingredients, stock):
    for i in range(len(ingredients)):
        for j in range(len(stock)):
            if ingredients[i].name in stock[j].name and ingredients[i].quantity > stock[j].quantity:
                return False
    return True

def select(cocktail, stock, order):
    selection = input("\n\nWhich cocktail do you want ?\n\n\t")
    for i in range(len(cocktail)):
        if selection in cocktail[i].name:
            if availability(cocktail[i].ingredients, stock) == False:
                print("\nSorry, I don't have enough ingredients to make a {}\n".format(cocktail[i].name))
                select(cocktail, stock, order)
            print("\nA {}, very good choice it's my favourite cocktail.\n".format(cocktail[i].name))
            if order.content == None:
                return
            for j in range(len(order.content)):
                if order.content[j].name in cocktail[i].name:
                    order.amount[j] += 1 
                    break
            if cocktail[i].name in order.content:
                order.content.append(cocktail[i])
                order.amount.append(1)
            quantityLess(cocktail[i].ingredients, stock)
            if person == 1:
                if input("Do you want to see and//or modify the stock (yes/no)") in "yes":
                    stock_var(stock)
            return
    print("\nExcuse me, I don't know this cocktail.\n") 
    select(cocktail, stock, order)

def quantityLess(ingredients, stock):
    for i in range(len(ingredients)):
        for j in range(len(stock)):
            if ingredients[i].name in stock[j].name:
                stock[j].quantity -= ingredients[i].quantity
            

main()