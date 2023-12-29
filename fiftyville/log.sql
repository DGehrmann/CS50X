-- Keep a log of any SQL queries you execute as you solve the mystery.

-- As suggested, start with crime scene reports to get general information on the reportet crime.
SELECT * FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28 AND street LIKE "Humphrey%";
-- Theft took place at 10:15am. 3 witnesses. Each witness mentions the bakery.

-- Find interview reports of the three witnesses mentioned in the crime scene report.
SELECT * FROM interviews WHERE year = 2021 AND month = 7 AND day = 28;
-- Witnesses are Ruth, Eugene and Raymond. Within 10 min of theft, thief got into car in the bakery parking lot and left.
-- Eugene knows the thief, doesn't recall his name. Before the theft, the thief was seen withdrawing money from the ATM on Leggett Street.
-- After thief left bakery, he called someone for less than a minute. Planned to take first flight out of fiftyville the next day. Accomplice was asked to buy plane ticket.

-- Check bakery security log in given time window. Returns license plates of the vehicles in question.
SELECT * FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25;
-- License plates with id 260 up to and including 267.

-- Check flights-table in combination with airport-table to find destination of the first flight that left from Fiftyville the day after the theft.
SELECT city FROM airports WHERE id = (SELECT destination_airport_id FROM flights WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville") AND day = 29 ORDER BY hour, minute LIMIT 1);
-- Destination: NYC.

-- Check people-table for persons name and passport number who's license plate is in the above and
-- who's phone no. as caller is in phone_calls-table on 2021/7/28 with duration < 60s.
SELECT name, passport_number FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25) AND phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60);

-- Check for the passengers passport numbers on the flight to NYC.
SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1);

-- Intersect the two above.
SELECT passport_number FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25) AND phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
INTERSECT
SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1);
-- Three people left as suspects.

-- Find person-IDs for the three people from the query above.
SELECT id FROM people WHERE passport_number IN (SELECT passport_number FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25) AND phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
INTERSECT
SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1));
-- Remaining suspects: Sofia (398010), Kelsey (560886), Bruce (686048)

-- Check ATM transactions and bank accounts for the three left suspects.
-- ATM transactions:
SELECT * FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location LIKE "Leggett%" AND transaction_type = "withdraw";
-- Acount numbers of the three suspects:
SELECT account_number, person_id FROM bank_accounts WHERE person_id IN (SELECT id FROM people WHERE passport_number IN (SELECT passport_number FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25) AND phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
INTERSECT
SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1)));
-- Turns out, only one of the three suspects has a bank account: account number 49610011, person_id 686048

-- Retrieve name for the remaining suspect:
SELECT name FROM people WHERE id = (SELECT person_id FROM bank_accounts WHERE person_id IN (SELECT id FROM people WHERE passport_number IN (SELECT passport_number FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25) AND phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
INTERSECT
SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1))));
-- Bruce!

-- Bruce's phone no.:
SELECT phone_number FROM people WHERE name = "Bruce";

-- Check phone_calls for call from Bruce to someone on given date.
SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller = (SELECT phone_number FROM people WHERE name = "Bruce");

-- Find corresponding name:
SELECT name FROM people WHERE phone_number = (SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller = (SELECT phone_number FROM people WHERE name = "Bruce"));
-- Robin!