Sorting Algorithms
==================

The project attempts to sort 4 different data sets. Each data set consists of strings representing numbers, but each data set has certain specifications for how those strings are constructed. For instance, the first data set, T1, consists of 100,000 randomly generated elements with 20 digits followed by a decimal point, then 20 more digits. Leading and trailing 0s are removed from the numbers. The cases are as follows:

  T1: Described Above
  T2: Same as T1, except with 1,000,000 elements
  T3: Same as T2, except with 3 digits before and after the decimal
  T4: Same as T2, except close to sorted
  
In order to sort these data sets, the first few entries were considered. In this way, I attempt to figure out which of the above 4 cases the particular data set belongs to. The following sorting algorithms were used for each of the data sets.

Merge Sort
----------

Was used for T1 and T2. The comparator was overloaded and the default `list.sort` method was used.

Radix Sort
----------

Used for T3, since it's efficiency is measured by the amount of digits present.

Insertion Sort
--------------

Used for T4, particularly effective for close to sorted lists.
