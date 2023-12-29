import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    stocks = db.execute("SELECT DISTINCT symbol FROM purchased WHERE user_id = ?", user_id)
    shares = db.execute("SELECT SUM(no_of_shares) AS sum, symbol FROM purchased WHERE user_id = ? GROUP BY symbol", user_id)

    list_stocks = []
    list_shares = []

    for i in shares:
        list_shares.append(i["sum"])
        list_stocks.append(i["symbol"])

    list_price = []
    for i in list_stocks:
        price = lookup(i)["price"]
        list_price.append(price)

    length = len(list_stocks)

    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cash[0]["cash"]

    summen = [a*b for a, b in zip(list_shares, list_price)]
    total = cash + sum(summen)
    total = usd(total)
    cash = usd(cash)

    list_price_usd = []

    for i in list_price:
        price_usd = usd(i)
        list_price_usd.append(price_usd)

    list_value = []
    for i in range(0, length):
        value = list_shares[i] * list_price[i]
        value = usd(value)
        list_value.append(value)

    return render_template("index.html", stocks=list_stocks, shares=list_shares, prices=list_price_usd, values=list_value, length=length, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    else:
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")  # number of stocks to be bought

        if not symbol:
            return apology("Please enter Symbol")

        elif lookup(symbol) == None:
            return apology("Symbol not existant")

        elif shares.isnumeric() == False:
            return apology("Must be positiv integer")

        # all entries correct:
        else:
            user_id = session["user_id"]
            price = lookup(symbol)["price"]
            cash_available = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
            cash_available = cash_available[0]["cash"]
            shares = int(shares)
            cash_needed = price * shares

            date_time = db.execute("SELECT CURRENT_TIMESTAMP")
            date_time = date_time[0]["CURRENT_TIMESTAMP"]

            # db.execute("CREATE TABLE purchased (user_id INTEGER NOT NULL, symbol TEXT NOT NULL, no_of_shares INTEGER NOT NULL, price_bought NUMERIC, FOREIGN KEY(user_id) REFERENCES users(id))")#; CREATE UNIQUE INDEX user_id ON purchased(user_id); CREATE INDEX symbol ON purchased(symbol)")

            if cash_needed > cash_available:
                return apology("Not enough cash available")

            else:
                db.execute("INSERT INTO purchased (user_id, symbol, no_of_shares, price_bought, date_time) VALUES (?,?,?,?,?)",
                            user_id, symbol, shares, price, date_time)
                db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_available-cash_needed, user_id)

                return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transactions = db.execute(
        "SELECT symbol, no_of_shares, price_bought, price_sold, date_time FROM purchased WHERE user_id = ? ORDER BY date_time", user_id)

    list_headers = ["symbol", "no_of_shares", "price_bought", "price_sold", "date_time"]
    for i in transactions:
        for header in list_headers:
            if i[header] == None:
                i[header] = "--"

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "GET":
        return render_template("quote.html")

    else:
        symbol = request.form.get("symbol")
        dict_lookup = lookup(symbol)

        if dict_lookup == None:
            return apology("Symbol not existant")

        else:
            company = dict_lookup["name"]
            price = dict_lookup["price"]
            price = usd(price)

            return render_template("quoted.html", company=company, price=price, symbol=symbol)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    username = request.form.get("username")
    password = request.form.get("password")
    dict_existing_users = db.execute("SELECT username FROM users")
    existing_users = []

    for i in dict_existing_users:
        existing_users.append(i["username"])

    print(existing_users)

    if request.method == "POST":
        if not username:
            return apology("Please provide username", 400)

        elif username in existing_users:
            return apology("Username already exists", 400)

        elif not password:
            return apology("Please provide password", 400)

        elif password != request.form.get("confirmation"):
            return apology("Passwords not identical", 400)

        else:
            hash = generate_password_hash(password, method="pbkdf2:sha256", salt_length=8)  # hash password here
            db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, hash)

            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]

    if request.method == "POST":

        requested_symbol = request.form.get("symbol")
        requested_shares = request.form.get("shares")
        available_shares = db.execute(
            "SELECT SUM(no_of_shares) AS sum FROM purchased WHERE user_id = ? AND symbol = ?", user_id, requested_symbol)
        available_shares = available_shares[0]["sum"]
        symbols = db.execute("SELECT DISTINCT symbol FROM purchased WHERE user_id = ?", user_id)
        list_symbols = []

        for i in range(0, len(symbols)):
            symbol = symbols[i]["symbol"]
            list_symbols.append(symbol)

        if request.form.get("symbol") not in list_symbols:
            return apology("Selected stocks not in possession")

        elif request.form.get("symbol") == None:
            return apology("Must select stock")

        elif request.form.get("shares").isnumeric() == False:
            return apology("Must be positive integer")

        elif int(requested_shares) > available_shares:
            return apology("Not enough shares available")

        else:
            #new_shares = available_shares - requested_shares
            price = lookup(requested_symbol)["price"]
            cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
            cash = cash[0]["cash"]
            new_cash = cash + (price * int(requested_shares))
            neg_requ_shares = - int(requested_shares)

            date_time = db.execute("SELECT CURRENT_TIMESTAMP")
            date_time = date_time[0]["CURRENT_TIMESTAMP"]

            db.execute("INSERT INTO purchased (user_id, symbol, no_of_shares, price_sold, date_time) VALUES (?,?,?,?,?)",
                        user_id, requested_symbol, neg_requ_shares, price, date_time)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)
            return redirect("/")

    else:
        symbols = db.execute("SELECT DISTINCT symbol FROM purchased WHERE user_id = ?", user_id)
        list_symbols = []

        for i in range(0, len(symbols)):
            symbol = symbols[i]["symbol"]
            list_symbols.append(symbol)

        return render_template("sell.html", symbols=list_symbols)


@app.route("/change_pw", methods=["GET", "POST"])
@login_required
def change_pw():
    """Change password"""

    user_id = session["user_id"]

    if request.method == "GET":
        return render_template("change_pw.html")

    else:
        current = request.form.get("current_pw")
        new = request.form.get("new_pw")
        confirm = request.form.get("confirm_pw")
        hash_new = generate_password_hash(new, method="pbkdf2:sha256", salt_length=8)
        hash_old = db.execute("SELECT hash FROM users WHERE id = ?", user_id)[0]["hash"]
        print(hash_old)

        if current == "" or new == "" or confirm == "":
            return apology("Please fill out all the fields.")

        elif new != confirm:
            return apology("New passwords do not match.")

        elif check_password_hash(hash_old, current) == False:
            return apology("Wrong password.")

        else:
            db.execute("UPDATE users SET hash = ? WHERE id = ?", hash_new, user_id)
            return redirect("/")