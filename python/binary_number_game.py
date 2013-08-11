"""
The beauty of a number X is the number of 1s in the binary representation of X.

Two players are plaing a game. There is number n written on a black board. The game is played as following:

Each time a player chooses an integer number (0 <= k) so that 2^k is less than n and (n-2^k) has as beautiful as n.
Next he removes n from blackboard and writes n-2^k instead.

The player that can not continue the game (there is no such k that satisfies the constrains) looses the game.

The First player starts the game and they play in turns alternatively. Knowing that both two players play optimally you
have to specify the winner.

Input:

First line of the Input contains an integer T, the number of testcase. 0 <= T <= 5.

Then follow T lines, each containing an integer n.

n more than 0 and less than 10^9 +1.

Output

For each testcase print "First Player" if first player can win the game and "Second Player" otherwise.

Sample Input

7
1
2
8
16
42
1000
123

Sample Output

Second Player
First Player
First Player
Second Player
Second Player
First Player
Second Player

Explanation

In the first example n is 1 and first player can't change it so the winner is the second player.

In the second example n is 2, so the first player subtracts 1 (2^0) from n and the second player looses the game.
"""


def solve(n):
    """
    Idea: Let the zeros bubble to the top. The strategy does not matter as the game's result is predetermined by the
    initial number (stupid game).
    """
    x = [int(x) for x in (bin(n)[2:])]
    move_count = 0
    while 0 in x:
        for i in range(len(x) - 1, 0, -1):
            if not x[i] and x[i - 1]:
                x[i] = 1
                if i == 1:
                    x.pop(0)
                else:
                    x[i - 1] = 0
                move_count += 1
    return '%s Player' % ('First' if move_count % 2 else 'Second')

assert 'Second Player' == solve(1)
assert 'First Player' == solve(2)
assert 'First Player' == solve(8)
assert 'Second Player' == solve(16)
assert 'Second Player' == solve(42)
assert 'First Player' == solve(1000)
assert 'Second Player' == solve(123)