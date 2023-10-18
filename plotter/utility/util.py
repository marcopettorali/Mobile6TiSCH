def avg(vals):
    return sum(vals)/len(vals)

def err(vals):
    if len(vals) == 1:
        return 0
    return 0.5 * abs(vals[0]-vals[1])

def sum(vals):
    return [vals[0][0] + vals[1][0], vals[0][1] + vals[1][1]]