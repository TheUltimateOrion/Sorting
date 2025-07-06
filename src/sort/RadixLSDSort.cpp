#include "sort/RadixLSDSort.h"

RadixLSDSort::RadixLSDSort(std::vector<int>& arr, int radix) : Sort(arr), radix(radix) {}

void RadixLSDSort::countSortByDigits(int exponent, int minValue)
{
	int bucketIndex;
    std::vector<int> buckets(radix);
    std::vector<int> output(elems.size());
    // Initialize bucket
    for (int i = 0; i < radix; i++) {
        buckets[i] = 0;
	}
    
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
		app->sortRenderer->update(output, i, i);
		if (wantClose || wantStop)
			return;
	}

    // Copy back
    for (int i = 0; i < elems.size(); i++) {
        elems[i] = output[i];
		app->sortRenderer->update(output, i, i);
		if (wantClose || wantStop)
			return;
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

// void RadixLSDSort::sort()
// {
// 	isSorting = true;

// 	int maxNum = elems[0];

// 	for (int i = 1; i < elems.size(); i++)
// 	{
// 		maxNum = std::max(maxNum, elems[i]);
// 	}

// 	int digitsCount = 0;
// 	while (maxNum > 0)
// 	{
// 		digitsCount++;
// 		maxNum /= 10;
// 	}

// 	for (int i = 0; i < digitsCount; i++)
// 	{
// 		int pwr = std::pow(10, i);
// 		std::vector<int> new_elems(elems.size());

// 		int count_array[10];
// 		memset(count_array, 0, sizeof(count_array));

// 		for (int j = 0; j < elems.size(); j++)
// 		{
// 			int num = (elems[j] / pwr) % 10;

// 			count_array[num]++;
// 		}

// 		for(int j = 1; j < 10; j++){
//             count_array[j] += count_array[j-1];
//         }

//         for(int j = elems.size() - 1; j >= 0; j--){
//             int num = (elems[j] / pwr) % 10;
//             new_elems[count_array[num] - 1] = elems[j];
//             count_array[num]--;
//         }

//         // Now, we are updating the array with the new array
//         for(int j = 0;j < elems.size(); j++) {
//             app->sortRenderer->render(elems, j, j);
//             if (wantClose || wantStop)
//                 return;
//             elems[j] = new_elems[j];
// 		}
// 	}

//     isSorting = false;
//     sorted = true;
// }