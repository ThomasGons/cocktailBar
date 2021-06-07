class Someone:
    def __init__(self, name, age):
        self.name = name
        self.age = age

david = Someone("David", 28)
print(david.__dict__)
