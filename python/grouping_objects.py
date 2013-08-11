"""
Question 1 / 1 (Grouping Objects)
There are N objects kept in a row. The ith object is at position x_i. You want to partition them into K groups.
You want to move all objects belonging to the same group to the same position. Objects in two different groups may be
placed at the same position. What is the minimum total amount by which you need to move the objects to accomplish this?

Input:
The first line contains the number of test cases T. T test cases follow. The first line contains N and K. The next line
 contains N space seperated integers, denoting the original positions x_i of the objects.

Output:
Output T lines, containing the total minimum amount by which the objects should be moved.

Constraints:
1 <= T <= 1000
1 <= K <= N <= 200
0 <= x_i <= 1000

Sample Input:
3
3 3
1 1 3
3 2
1 2 4
4 2
1 2 5 7

Sample Output:
0
1
3

Explanation:

For the first case, there is no need to move any object.
For the second case, group objects 1 and 2 together by moving the first object to position 2.
For the third case, group objects 1 and 2 together by moving the first object to position 2 and group objects 3 and 4
together by moving object 3 to position 7. Thus the answer is 1 + 2 = 3.
"""


def solve(k, x):
    groups = [[i] for i in sorted(x)]

    while k < len(groups):
        # determine cheapest merge option for existing groups
        min_cost = None
        min_idx = None
        for idx in range(len(groups) - 1):
            cost = merge_cost(groups[idx] + groups[idx + 1])
            if min_cost is None or cost < min_cost:
                min_cost = cost
                min_idx = idx

        # then merge the two groups
        groups[min_idx] += groups.pop(min_idx + 1)
        print groups

    # calculate and return group merge costs
    return reduce(lambda s, x: s + merge_cost(x), groups, 0)


def merge_cost(group):
    # merge cost is the cost of moving all items to the median (not the middle)
    median_pos = group[len(group) / 2]
    return reduce(lambda s, x: s + abs(x - median_pos), group, 0)


# TEST CASES
assert 0 == solve(3, [1, 1, 3])
assert 1 == solve(2, [1, 2, 4])
assert 3 == solve(2, [1, 2, 5, 7])
assert 0 == solve(6, [0, 17, 22, 24, 33, 36])
assert 2 == solve(5, [0, 17, 22, 24, 33, 36])
assert 5 == solve(4, [0, 17, 22, 24, 33, 36])
assert 10 == solve(3, [0, 17, 22, 24, 33, 36])
assert 32 == solve(2, [0, 17, 22, 24, 33, 36])
assert 54 == solve(1, [0, 17, 22, 24, 33, 36])
assert 0 == solve(4, [3, 3, 3, 3])
assert 0 == solve(3, [3, 3, 3, 3])
assert 0 == solve(2, [3, 3, 3, 3])
assert 0 == solve(1, [3, 3, 3, 3])