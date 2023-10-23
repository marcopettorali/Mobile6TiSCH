import json
import os
import fnmatch


class SeriesCollector:

    def __init__(self, data_folder: str):
        self.__inner_dict = {}
        self.__values = {}
        self.__data_folder = data_folder

    def __init__(self, data_folder, format_string, **kwargs):
        self.__inner_dict = {}
        self.__values = {}
        self.__data_folder = data_folder

        self.load(format_string, **kwargs)

    def __iter__(self):
        ret = []

        # iterate all the elements in the tree
        def iter_util(cursor, ret, path):

            if "layer" in cursor:
                for key in cursor:
                    if key != "layer":
                        iter_util(cursor[key], ret, path + [key])
            else:
                ret.append((path, cursor))

        iter_util(self.__inner_dict, ret, [])
        return iter(ret)

    def load(self, format_string: str, **kwargs):
        for m in kwargs:
            self.__values[m] = kwargs[m]

        self.__inner_dict = self.__load_file_recursive(format_string, **kwargs)
        print(self.__values)

    def __load_file_recursive(self, format_string, **kwargs):
        if "$" not in format_string:

            # list all files that match the format string in the data folder
            files = []

            for filename in os.listdir(self.__data_folder):
                relative_path = os.path.relpath(os.path.join(self.__data_folder, filename), self.__data_folder)

                # check if the filename matches the format string (json)
                if fnmatch.fnmatch(relative_path, format_string + ".json"):
                    # if it does, add it to the list
                    files.append(os.path.join(self.__data_folder, filename))

            # if there are no files raise an error
            if len(files) == 0:
                raise FileNotFoundError(f"No matches found for the format string {format_string}. Maybe the file {self.__data_folder}/{format_string}.json does not exist?")

            # if there are multiple files, raise an error
            if len(files) > 1:
                error = f"Multiple files ({len(files)}) match the format string {format_string}"
                for file in files:
                    error += f"\n\t{file}"
                raise FileExistsError(error)

            # open the file and return the json
            file = files[0]

            try:
                with open(file) as f:
                    return json.load(f)
            except:
                print(f"Cannot open file {self.__data_folder}/{format_string}.json")
                return

        keys = list(kwargs.keys())
        new_kwargs = kwargs.copy()
        new_kwargs.pop(keys[0])
        dict = {}
        for elem in kwargs[keys[0]]:
            dict["layer"] = keys[0]
            dict[elem] = self.__load_file_recursive(format_string.replace("$", str(elem), 1), **new_kwargs)
        return dict

    def get_values(self, index):
        return self.__values[index]

    def get(self, metric, **kwargs):
        cursor = self.__inner_dict
        while "layer" in cursor:
            if len(self.__values[cursor["layer"]]) == 1:
                cursor = cursor[self.__values[cursor["layer"]][0]]
            elif cursor["layer"] in kwargs:
                cursor = cursor[kwargs[cursor["layer"]]]
            else:
                raise KeyError(f"No leaf found for the selected query: kwargs = {kwargs}")
        return cursor[metric]

    def __get_util(self, x_name: str, x_val, z_name: str, z_val, **kwargs):
        cursor = self.__inner_dict
        while "layer" in cursor:
            if len(self.__values[cursor["layer"]]) == 1:
                cursor = cursor[self.__values[cursor["layer"]][0]]
            elif cursor["layer"] == x_name:
                cursor = cursor[x_val]
            elif cursor["layer"] == z_name:
                cursor = cursor[z_val]
            elif cursor["layer"] in kwargs:
                cursor = cursor[kwargs[cursor["layer"]]]
            else:
                raise KeyError(f"No leaf found for the selected query: {x_name} = {str(x_val)}, {z_name} = {str(z_val)}, kwargs = {kwargs}")
        return cursor

    def values_for_key(self, key):
        return self.__values[key]

    def add_values_at_query(self, y_name, y_val, **kwargs):
        cursor = self.__get_util("", None, "", None, **kwargs)
        cursor[y_name] = y_val

    def query_old(self, x_name: str, y_name, **kwargs):
        xs = self.__values[x_name]
        return {"xs": xs, "ys": [self.__get_util(x_name, x, "", None, **kwargs)[y_name] for x in xs]}

    def query(self, x_name: str, y_name, **kwargs):
        xs = self.__values[x_name]
        return {x: y for x, y in zip(xs, [self.__get_util(x_name, x, "", None, **kwargs)[y_name] for x in xs])}


def transform(series, func):
    # check if series is a dict or a list
    if isinstance(series, dict):
        for x in series:
            series[x] = [func(y) for y in series[x]]
        return series
    elif isinstance(series, list):
        ret = []
        for l in series:
            li = {}
            for x in series:
                li[x] = func(series[x])
            ret.append(li)
        return ret
