#include "sort/RadixLSDSort.h"

RadixLSDSort::RadixLSDSort(std::vector<int>& arr, int radix) : Sort(arr), radix(radix) {}

void RadixLSDSort::countSortByDigits(int exponent, int minValue)
{
	int bucketIndex;
    std::vector<int> buckets(radix);
    std::vector<int> output(elems.size());

    // Initialize bucket
    std::fill(buckets.begin(), buckets.end(), 0);
    
    // Count frequencies
    for (int i = 0; i < elems.size(); i++) {
        bucketIndex = (int)(((elems[i] - minValue) / exponent) % radix);
        buckets[bucketIndex]++;
	}
    
    // Compute cumulates
    for (int i = 1; i < radix; i++) {
        buckets[i] += buckets[i - 1];
	}

    // Move records
    for (int i = elems.size() - 1; i >= 0; i--) {
        bucketIndex = (int)(((elems[i] - minValue) / exponent) % radix);
        output[--buckets[bucketIndex]] = elems[i];
		this->first = i;
		this->second = bucketIndex;
		if (wantClose || wantStop)
			return;
	}

    // Copy back
    for (int i = 0; i < elems.size(); i++) {
        elems[i] = output[i];
		this->first = i;
        this->second = ((output[i] - minValue) / exponent) % radix;
		// this->second = bucketIndex;
        HIGH_RES_WAIT(1.f / Sort::speed);
		if (wantClose || wantStop) return;
    }
}

void RadixLSDSort::sort()
{
	isSorting = true;
	int minValue = elems[0];
    int maxValue = elems[0];
    for (int i = 1; i < elems.size(); i++) {
        if (elems[i] < minValue) {
            minValue = elems[i];
        } else if (elems[i] > maxValue) {
                maxValue = elems[i];
        }
    }

    // Perform counting sort on each exponent/digit, starting at the least
    // significant digit
    int exponent = 1;
    while ((maxValue - minValue) / exponent >= 1) {
        countSortByDigits(exponent, minValue);
		if (wantClose || wantStop)
			return;
        exponent *= radix;
    }
	isSorting = false;
	sorted = true;
}