from cs50 import get_float
from decimal import ROUND_DOWN


def main():

    owed = -1

    while owed < 0:
        owed = get_float("Change owed: ")

    quarters = calc_quarters(owed)
    owed = round(owed - quarters * 0.25, 2)

    dimes = calc_dimes(owed)
    owed = round(owed - dimes * 0.1, 2)

    nickels = calc_nickels(owed)
    owed = round(owed - nickels * 0.05, 2)

    pennies = calc_pennies(owed)
    owed = round(owed - pennies * 0.01, 2)

    no_coins = quarters + dimes + nickels + pennies
    print(f"{no_coins}")

def calc_quarters(owed):
    return int(owed / 0.25)

def calc_dimes(owed):
    return int(owed / 0.10)

def calc_nickels(owed):
    return int(owed / 0.05)

def calc_pennies(owed):
    return int(owed / 0.01)

if __name__ == "__main__":
    main()