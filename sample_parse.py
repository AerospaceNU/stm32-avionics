millis = 15


def hm_millis():
    # Fake millis
    return millis


data_dict = {"pos_z": 600, "acc_z": 68, "vel_z": 300}

# Just a few sample variables, real would have more
variables = {"pos_z": 1, "acc_z": 2, "vel_z": 3}

# Sample of possible events
# (Occurred, timestamp)
events = {
    "launch": {"past": True, "timestamp": 10},
    "apogee": {"past": False, "timestamp": 0},
    "touchdown": {"past": False, "timestamp": 0},
}

# List of expressions by id, whether they have been satisifed, and when they were satisifed
# (if they were)
expr_dict = {}


operations = {
    # Simple operations
    "<": lambda x, y: evaluate(x) < evaluate(y),
    ">": lambda x, y: evaluate(x) > evaluate(y),
    "==": lambda x, y: evaluate(x) == evaluate(y),
    "+": lambda x, y: evaluate(x) + evaluate(y),
    "-": lambda x, y: evaluate(x) - evaluate(y),
    "*": lambda x, y: evaluate(x) * evaluate(y),
    "/": lambda x, y: evaluate(x) // evaluate(y),
    "and": lambda x, y: evaluate(x) and evaluate(y),
    "or": lambda x, y: evaluate(x) or evaluate(y),
    # Some more complex operations
    "after": lambda delay, event: get_event(event)["past"]
    and hm_millis() - get_event(event)["timestamp"] > evaluate(delay),
    "for": lambda expr, timestamp: evaluate(expr) and hm_millis() - expr["timestamp"] > evaluate(timestamp),
}

# English equivalents of operations
op_lang = {
    "<": lambda x, y: f"{(x)} < {(y)}",
    ">": lambda x, y: f"{(x)} > {(y)}",
    "==": lambda x, y: f"{(x)} == {(y)}",
    "+": lambda x, y: f"{(x)} + {(y)}",
    "-": lambda x, y: f"{(x)} - {(y)}",
    "*": lambda x, y: f"{(x)} * {(y)}",
    "/": lambda x, y: f"{(x)} / {(y)}",
    "and": lambda x, y: f"({x}) and ({y})",
    "or": lambda x, y: f"({x}) or ({y})",
    "after": lambda delay, event: f"{delay} ms after {event}",
    "for": lambda expr, timestamp: f"{lang(expr)} for {timestamp} ms",
}


def get_args(expr):
    args = []
    num_paren = 0
    start_loc = 0
    for i, char in enumerate(expr):
        if char == " " and num_paren == 0 and start_loc != i:
            args.append(expr[start_loc:i])
            start_loc = i + 1
        if char == "(":
            if num_paren == 0:
                start_loc = i
            num_paren += 1
        if char == ")":
            num_paren -= 1
            if num_paren == 0:
                args.append(expr[start_loc:i + 1])
                start_loc = i + 1
    if start_loc != len(expr):
        args.append(expr[start_loc:])
    return args


def evaluate(expr):
    if expr in variables:
        return data_dict[expr]
    if expr in events:
        return expr
    if type(expr) == int or type(expr) == float or type(expr) == bool:
        return expr
    # This is cursed
    try:
        if str(int(expr)) == expr:
            return int(expr)
        elif str(float(expr)) == expr:
            return float(expr)
    except ValueError:
        pass
    # Otherwise it's some expression enclosed by ()
    expr = expr[1:-1]
    args = [arg.strip() for arg in get_args(expr)]
    evaluate(args[0])
    evaluate(args[2])
    res = operations[args[1]](args[0], args[2])

    if expr not in expr_dict:
        expr_dict[expr] = {'past': res, 'timestamp': hm_millis()}
    elif res and not expr_dict[expr]['past']:
        expr_dict[expr] = {'past': res, 'timestamp': hm_millis()}
    return res


def get_event(expr):
    # This is really hacky because I wasn't consistent with having () around things or not here
    if expr[1:-1] in events:
        return events[expr[1:-1]]
    if expr[1:-1] in expr_dict:
        return expr_dict[expr[1:-1]]
    if expr in events:
        return events[expr]
    if expr in expr_dict:
        return expr_dict[expr]
    else:
        # Not a thing that is an event with things
        print("none")
        return None


def lang(expr):
    if expr in variables:
        return expr
    if expr in events:
        return expr
    if type(expr) == int or type(expr) == float or type(expr) == bool:
        return str(expr)
    # This is cursed
    try:
        if str(int(expr)) == expr:
            return expr
        elif str(float(expr)) == expr:
            return expr
    except ValueError:
        pass
    # Otherwise it's some expression enclosed by ()
    expr = expr[1:-1]
    args = [arg.strip() for arg in get_args(expr)]
    return op_lang[args[1]](lang(args[0]), lang(args[2]))


exprs = [
    "((pos_z > 100) and (((vel_z > 305) or (acc_z == 71)) and ((5 + 5) after launch)))",
    "((pos_z < 605) or (acc_z > 74))",
    "((1 + 1) after (pos_z > 602))",
    "(3 + (2 * 8))"
]


for _ in range(8):
    for key, value in data_dict.items():
        data_dict[key] = value + 1
    millis = millis + 1
    print(f"Incrementing data to: {data_dict}, millis to {millis}")
    for expr in exprs:
        print(f"{expr}: {evaluate(expr)}")

for expr in exprs:
    print(f"{expr} in english: {lang(expr)}")
