from cs50 import get_string

word_counter = 1
sentence_counter = 0
letter_counter = 0

text = get_string("Text: ")

for c in text:
    if c == " ":
        word_counter += 1
    elif c in [".", "!", "?"]:
        sentence_counter += 1
    elif c in ["'", ",", ":", ";"]:
        continue
    else:
        letter_counter += 1

L = (letter_counter / word_counter) * 100
S = (sentence_counter / word_counter) * 100

grade = round(0.0588 * L - 0.296 * S - 15.8)

if grade >= 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print(f"Grade: {grade}")

# print(word_counter)
# print(sentence_counter)
# print(letter_counter)
# print(grade)