#pragma once

class Sorting
{
public:
    template<typename T, typename U>
    static void CountSort(const std::vector<T>& sortingKeys, const std::vector<U>& sortingValues,
                          std::vector<T>& resultKeys, std::vector<U>& resultValues, int exp)
    {
        int i, count[10] = { 0 };

        for (i = 0; i < sortingKeys.size(); ++i)
            count[(sortingKeys[i] / exp) % 10]++;

        for (i = 1; i < 10; ++i)
            count[i] += count[i - 1];

        for (i = sortingKeys.size() - 1; i >= 0; --i)
        {
            resultKeys[count[(sortingKeys[i] / exp) % 10] - 1] = sortingKeys[i];
            resultValues[count[(sortingKeys[i] / exp) % 10] - 1] = sortingValues[i];
            count[(sortingKeys[i] / exp) % 10]--;
        }
    }

    template<typename T, typename U>
    static void RadixSort(std::vector<T>& sortingKeys, std::vector<U>& sortingValues)
    {
        if (sortingKeys.empty())
            return;

        const T m = (T)-1; // Max value (valid only for T unsigned types)

        std::vector<T> resultKeys;
        resultKeys.resize(sortingKeys.size());
        std::vector<U> resultValues;
        resultValues.resize(sortingValues.size());

        bool swap = false;
        for (long exp = 1; m / exp > 0; exp *= 10)
        {
            if (swap)
                CountSort(resultKeys, resultValues, sortingKeys, sortingValues, exp);
            else
                CountSort(sortingKeys, sortingValues, resultKeys, resultValues, exp);
            swap = !swap;
        }

        if (swap)
        {
            for (int i = 0; i < sortingKeys.size(); ++i)
            {
                sortingKeys[i] = resultKeys[i];
                sortingValues[i] = resultValues[i];
            }
        }
    }
};
