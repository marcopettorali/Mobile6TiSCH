import matplotlib.pyplot as plt


def avg(vals):
    return sum(vals) / len(vals)


def err(vals):
    if len(vals) == 1:
        return 0
    return 0.5 * abs(vals[0] - vals[1])


class SeriesPlotter:

    def __init__(self, type: str):
        self.type = type
        self.__series = []

    def add_series(self, series, **kwargs):
        series["kwargs"] = kwargs
        self.__series.append(series)

    def draw_all_old(self, ax: plt.Axes):
        if self.type == "line":
            for series in self.__series:
                ax.plot(series["xs"], series["ys"], **series["kwargs"])
        elif self.type == "errorbar":
            for series in self.__series:
                ax.errorbar(series["xs"], [avg(y) for y in series["ys"]], yerr=[err(y) for y in series["ys"]], **series["kwargs"])
        elif self.type == "bar":
            pass

    def draw_all(self, ax: plt.Axes):
        if self.type == "line":
            for series in self.__series:
                kwargs = {}
                if "kwargs" in series:
                    kwargs = series["kwargs"]
                    series.pop("kwargs")
                ax.plot(list(series.keys()), list(series.values()), **kwargs)
        elif self.type == "errorbar":
            for series in self.__series:
                kwargs = {}
                if "kwargs" in series:
                    kwargs = series["kwargs"]
                    series.pop("kwargs")
                ax.errorbar(list(series.keys()), [avg(y) for y in list(series.values())], yerr=[err(y) for y in list(series.values())], **kwargs)
        elif self.type == "bar":
            pass