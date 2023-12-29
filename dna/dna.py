import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py CSV-file txt-file")

    # TODO: Read database file into a variable
    database = sys.argv[1]

    subsequence = []
    persons = []
    with open(database) as file:
        reader = csv.reader(file)

        for row in reader:
            persons.append(row)

        subsequence = persons[0]
        del persons[0]
        # print(subsequence)
        # print(len(subsequence))
        # print(persons)
        # print(len(persons))

    # TODO: Read DNA sequence file into a variable
    sequence = open(sys.argv[2], "r")
    sequence = sequence.read()

    # TODO: Find longest match of each STR in DNA sequence
    dict = {}
    list = [0]

    for i in range(1, len(subsequence)):
        dict[subsequence[i]] = longest_match(sequence, subsequence[i])
    # print(dict)

    for subsequence in dict:
        list.append(dict[subsequence])

    # print(list)

    # TODO: Check database for matching profiles
    # compare list persons with list list
    # counter = 0
    found = False
    for person in persons:
        counter = 0
        if found == True:
            break
        for i in range(1, len(list)):
            if int(person[i]) == list[i]:
                counter += 1
                if counter == len(list)-1:
                    found = True
                    print(f"{person[0]}")
                    break

    if found == False:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
