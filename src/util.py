# simple function

def py_print_hello():
    print("Hello from util.py")


# Basic math functions


def py_add(a: int, b: int) -> int:
    return a + b


def py_multiply(a: float, b: float) -> float:
    return a * b


# string manipulation


def py_greet(name: str) -> str:
    return f"Hello, {name}!"


# list processing


def py_square_list(vec: list[int]) -> list[int]:
    return [x * x for x in vec]


# A simple calculator class


class PyCalculator:
    def __init__(self, initial_value: float = 0.0):
        self.value = initial_value

    def add(self, amount: float) -> None:
        self.value += amount

    def multiply(self, factor: float) -> None:
        self.value *= factor

    def get_value(self) -> float:
        return self.value

    def reset(self) -> None:
        self.value = 0.0


if __name__ == "__main__":
    
    py_print_hello()

    print(f"5 + 3 = {py_add(5, 3)}")
    print(f"2.5 * 4.0 = {py_multiply(2.5, 4.0)}")
    print(py_greet("World"))
    print(f"Square of [1, 2, 3, 4] = {py_square_list([1, 2, 3, 4])}")

    calc = PyCalculator(10.0)
    print(f"Initial calculator value: {calc.get_value()}")
    calc.add(5.0)
    print(f"After adding 5.0: {calc.get_value()}")
    calc.multiply(2.0)
    print(f"After multiplying by 2.0: {calc.get_value()}")
    calc.reset()
    print(f"After reset: {calc.get_value()}")
