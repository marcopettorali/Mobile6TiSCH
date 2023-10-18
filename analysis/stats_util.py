import math
from scipy.stats import norm

CONFIDENCE = 0.95

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
    percentile = normal_dist_percentile(1-(1-CONFIDENCE)/2)
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