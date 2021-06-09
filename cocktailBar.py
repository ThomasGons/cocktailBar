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
    def __iter__(self):
        for key in self.__dict__:
            yield key, getattr(self, key)
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
    def __iter__(self):
        for key in self.__dict__:
            if key == "ingredients":
                yield ("ingredients", [dict(i) for i in self.ingredients])
            else:
                yield key, getattr(self, key)

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

class NoAliasDumper(yml.SafeDumper):
    def ignore_aliases(self, data):
        return True

#global vars:

person, turnover = 0, 0


def yaml(cocktail, stock, mode):
    openMode = "r" if mode in "load" else "w"
    with open(r'config/tmpCocktail.yml', openMode) as ymlCocktail, open(r'config/tmpStock.yml', openMode) as ymlStock:
        if openMode == "r":
            loadedCocktail = yml.load(ymlCocktail, Loader = yml.FullLoader)
            loadedStock = yml.load(ymlStock, Loader = yml.FullLoader)
            return [[Cocktail.from_yaml(i) for i in loadedCocktail],
            [Ingredient.from_yaml(i) for i in loadedStock]]
        else:
            yml.dump([dict(c) for c in cocktail], ymlCocktail, Dumper = NoAliasDumper)
            yml.dump([s.__dict__ for s in stock], ymlStock, Dumper = NoAliasDumper)


def reset_yaml():
    with open(r'config/initCocktail.yml') as initYmlCocktail, open(r'config/initStock.yml') as initYmlStock:
        loadedInitCocktail = yml.load(initYmlCocktail, Loader = yml.FullLoader)
        loadedInitStock = yml.load(initYmlStock, Loader = yml.FullLoader)
    with open(r'config/tmpCocktail.yml', "w") as tmpYmlCocktail, open(r'config/tmpStock.yml', "w") as tmpYmlStock:
        yml.dump(loadedInitCocktail, tmpYmlCocktail)
        yml.dump(loadedInitStock, tmpYmlStock) 


def main():
    global person
    who = input("Who are you:\n \t- bartender ?\n \t- customer(default) ?\n")
    if who in "bartender":
        person = 1
        if input("Do you want to reset the cocktail card and the stock (yes/no) ?\n") in "yes":
            reset_yaml()
    cocktail, stock = yaml([], [], "load")
    order = Order()
    while True:
        menu(cocktail, stock, order)
        if input("\nDo you want to make a cocktail (yes/no) ?\n") not in "yes":
            break

    if len(order.content) > 0: 
        order_var(order)
        if not person:
            print("It will make you {:.3f}$\n".format(turnover))
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
            print("\t- {0}({1:.1f}°, {2:.1f}° g (sugar)), {3:.2f} cL, {4:.2f} $".format(cocktail[i].name,
					specs[i].alcohol, specs[i].sugar, specs[i].volume, specs[i].price, end=""))
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
            for j in range(len(order.content)):
                if order.content[j].name in cocktail[i].name:
                    order.amount[j] += 1 
                    break
            if cocktail[i].name not in order.content:
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
            

def homemade(cocktail, stock, order):
    p_ingredients = []
    print("List of ingredients:\n")
    for i in range(len(stock)):
        if stock[i].quantity > 0:
            print("\n\t- {0}: {1:.2f}$ (kg/L)".format(stock[i].name, stock[i].price), end="")
    while True:
        choice = input("\n\nWhat ingredient do you want to add ?\n")
        do = False
        for i in range(len(stock)):
            if choice == stock[i].name:
                quantity = int(input("\nWhat quantity of {} do you want ?\n".format(choice)))
                if quantity <= stock[i].quantity:
                    p_ingredients.append(Ingredient(choice, stock[i].alcohol, quantity, stock[i].sugar,
                    stock[i].price * quantity * 0.001))
                else:
                    print("Sorry, we don't have enough {}\n".format(choice))
                do = True
        if do == False:
            print("Sorry I don't understand\n")
        if input("Do you want another ingredient (yes/no) ?\n") not in "yes":
            break
    quantityLess(p_ingredients, stock)
    if person == 1:
        if input("\nDo you want to see and/or modify the stock (yes/no) ?\n\n\t") in "yes":
            stock_var(stock)
    if input("Do you want to add your cocktail to the list (yes/no) ?\n") in "yes":
        saveCocktail(cocktail, stock, p_ingredients, order, "save")
    else:
        saveCocktail(cocktail, stock, p_ingredients, order, "creation")


def stock_var(stock):
    print("\nThe stock contains:\n")
    for i in range(len(stock)):
        print("\t- {0} ({1:.3f} kg/L): {2:.2f}$\n".format(stock[i].name, stock[i].quantity * 0.001, stock[i].price))
    while True:
        ingredient = input("\nYou want to change the amount of which ingredient:\n\t")
        for i in range(len(stock)):
            if ingredient != stock[i].name :
                modify = input("\nYou want to:\n\t- add some {} (default)\n\t- remove some {}\n".format(stock[i].name,))
                while True:
                    quantity = input("\nWhich quantity?\n\t")
                    if quantity > stock[i].quantity and modify in "remove":
                        print("There is just {0:.3f} kg of {1} in the stock".format(stock[i].quantity * 0.001, stock[i].name))
                    else:
                        break
                
                stock[i].quantity -= quantity if modify in "remove" else stock[i].quantity + quantity
                quantity = 0
        test = input("\nDo you want to continue to modify the stock (yes/no) ?\n\t")
        if test in "yes":
            break
    print("\nNow the stock contains:\n")
    for i in range(len(stock)):
        print("\t- {0} ({1:.3f} kg/L): {2:.2f}$\n".format(stock[i].name, stock[i].quantity * 0.001, stock[i].price))


def saveCocktail(cocktail, stock, p_ingredients, order, mode):
    alcoholic = False
    name = input("Enter a name for you creation: ")
    for i in range(len(p_ingredients)):
        if p_ingredients[i].alcohol > 0:
            alcoholic = True
    cocktailSpe = Cocktail(name, alcoholic, p_ingredients)
    order.content.append(cocktailSpe)
    order.amount.append(1)
    if mode == "save":
        cocktail.append(cocktailSpe)
        yaml(cocktail, stock, "save")


def order_var(order):
    global turnover
    specs = [Specs() for i in range(len(order.content))]
    print("Here is your order")
    for i in range(len(order.content)):
        specs = specificity(order.content[i].ingredients)
        print("\t- {0}({1}), {2:.2f} $\n".format(order.content[i].name, order.amount[i], order.amount[i] * specs[i].price)) 
    if input("Do you want to modify your order (yes/no) ?\n") in "yes":
        while True:
            choice = input("You want to change the amount of which cocktail ?\n")
            do = False
            for i in range(len(order.content)):
                if choice in order.content[i].name:
                    quantity = int(input("How many {} do you want to add(+) or to remove(-) ?\n".format(choice)))
                    order.amount[i] = 0 if order.amount[i] + quantity < 0 else + quantity
                    do = True
            if do != True:
                print("Sorry this cocktail is not part of your order\n")
            if input("Do you want to continue to modify your order (yes/no) ?") not in "yes":
                break
    print("Here is your order now")
    for i in range(len(order.content)):
        specs[i] = specificity(order.content[i].ingredients)
        print("\t- {0}({1}), {2:.2f} $\n".format(order.content[i].name, order.amount[i], order.amount[i] * specs[i].price)) 
        turnover += specs[i].price * order.amount[i]

main()