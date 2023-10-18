import math
from scipy.stats import norm

confidence = 0.95
def s_max(samples):
    max = samples[0]
    for i in range(1, len(samples)):
        if samples[i] > max:
            max = samples[i]
    return max
def s_min(samples):
    min = samples[0]
    for i in range(1, len(samples)):
        if samples[i] < min:
            min = samples[i]
    return min
def s_mean_value_and_variance(samples):
    mean = s_mean_value(samples)
    variance = 0
    for sample in samples:
        variance += (sample - mean) ** 2
    variance /= (len(samples) - 1)
    return mean, variance
def s_mean_value(samples):
    if len(samples) == 0:
        return 0
    mean = 0
    for sample in samples:
        mean += sample
    mean /= len(samples)
    return mean
def s_variance(samples):
    mean = s_mean_value(samples)
    variance = 0
    for sample in samples:
        variance += (sample - mean) ** 2
    variance /= (len(samples) - 1)
    return variance
def s_lorenz_curve_gap(samples):
    mean = s_mean_value(samples)
    gap = 0
    for sample in samples:
        gap += abs(sample - mean)
    gap /= len(samples)
    gap /= 2 * mean
    return gap

def normal_dist_percentile(p):
    percentile = norm.ppf(p)
    return percentile

def s_mean_confidence_interval(samples):
    mean = s_mean_value(samples)
    variance = s_variance(samples)
    percentile = normal_dist_percentile(1-(1-confidence)/2)
    radius = percentile * math.sqrt(variance/len(samples))
    return [mean-radius, mean+radius]

def s_percentile(samples, percentile):
    sorted_samples = samples[:]
    sorted_samples.sort()
    return sorted_samples[math.ceil(len(samples) * percentile) - 1]
def sample_autocorrelation(samples, lag):
    mean = s_mean_value(samples)
    variance = s_variance(samples)
    n = len(samples)
    value = 0

    for i in range(0, n - lag):
        value += ((samples[i] - mean) * (samples[i + lag] - mean))

    if value == 0:
        return 0

    value /= n
    value /= variance

    return value
class StatisticContainer:
    def __init__(self):
        self._samples = {}

    def insert(self, label, sample):
        if not label in self._samples:
            self._samples[label] = {
                "count": 1, "sum": sample, "avg": sample, "min": sample, "max": sample
            }
        else:
            self._samples[label]["count"] += 1
            self._samples[label]["sum"] += sample
            self._samples[label]["avg"] = self._samples[label]["sum"]/self._samples[label]["count"]
            self._samples[label]["min"] = sample if sample < self._samples[label]["min"] else self._samples[label]["min"]
            self._samples[label]["max"] = sample if sample > self._samples[label]["max"] else self._samples[label]["max"]
    
    def read(self):
        return self._samples
