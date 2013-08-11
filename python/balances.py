"""
You have a room-full of balances and weights. Each balance weighs ten pounds and is considered perfectly balanced when the sum of weights on its left and right sides are exactly the same. You have placed some weights on some of the balances, and you have placed some of the balances on other balances. Given a description of how the balances are arranged and how much additional weight is on each balance, determine how to add weight to the balances so that they are all perfectly balanced.

There may be more than one way to balance everything, but always choose the way that places additional weight on the lowest balances.

The input file will begin with a single integer, N, specifying how many balances there are.
Balance 0 is specified by lines 1 and 2, balance 1 is specified by lines 3 and 4, etc...
Each pair of lines is formatted as follows:

WL <balances>
WR <balances>

WL and WR indicate the weight added to the left and right sides, respectively. <balances> is a space-delimited list of the other balance that are on that side of this balance. <balances> may contain zero or more elements.

Consider the following input:

4
0 1
0 2
0
0 3
3
0
0
0

Balance 0 has balance 1 on its left side and balance 2 on its right side
Balance 1 has balance 3 on its right side
Balance 2 has three pounds on its left side
Balance 3 has nothing on it

Since balance 3 has nothing on it it is already perfectly balanced, and weighs a total of 10 pounds.
Balance 2 has no other balance on it, so all we need to do is balance it by putting three pounds on its right side. Now it weighs a total of 16 pounds.
Balance 1 has balance three on its right side, which weighs 10 pounds, so we just put 10 pounds on its left side. Balance 1 weighs a total of 30 pounds.
Balance 0 has balance 1 on its left side (30 pounds), and balance 2 on its right side (16 pounds), we can balance it by adding 14 pounds to the right side.

The output should be N lines long, with the nth line listing the weight added to the nth balance, formatted as follows:

<index>: <weight added to left side> <weight added to right side>

So the output for this problem would be:

0: 0 14
1: 10 0
2: 0 3
3: 0 0
"""

import fileinput

nodes = {}
movements = {}
class Node(object):
    def __init__(self, idx):
        self.idx = idx
        self.wl = 0
        self.wr = 0
        self.cl = []
        self.cr = []
        self.parent = None

    def twl(self):
        return self.wl + sum((x.tw() for x in self.cl))

    def twr(self):
        return self.wr + sum((x.tw() for x in self.cr))

    def tw(self):
        return 10 + self.twl() + self.twr()

    def __repr__(self):
        return '%d: %d %s %d %s' % (self.idx, self.wl, [x.idx for x in self.cl], self.wr, [x.idx for x in self.cr])


def balance():
    for idx in nodes.keys():
        rec_balance(get_node(idx))


def rec_balance(n):
    if n.idx in movements:
        return
    for n2 in n.cl:
        rec_balance(n2)
    for n2 in n.cr:
        rec_balance(n2)

    wdiff = n.twl() - n.twr()
    if wdiff < 0:
        n.wl += abs(wdiff)
        movements[n.idx] = (abs(wdiff), 0)
    elif wdiff > 0:
        n.wr += wdiff
        movements[n.idx] = (0, wdiff)
    else:
        movements[n.idx] = (0, 0)

def get_node(idx):
    if idx not in nodes:
        nodes[idx] = Node(idx)
    return nodes[idx]


def init_tree(data):
    for idx in range(len(data)):
        nd = data[idx]
        n = get_node(idx)
        n.wl = nd[0][0]
        for idx2 in nd[0][1:]:
            n2 = get_node(idx2)
            n.cl.append(n2)
            n2.parent = n
        n.wr = nd[1][0]
        for idx2 in nd[1][1:]:
            n2 = get_node(idx2)
            n.cr.append(n2)
            n2.parent = n


def read():
    lines = [x for x in fileinput.input()]
    n = int(lines.pop(0))
    data = []
    for i in range(n):
        data.append([map(int, lines.pop(0).split()), map(int, lines.pop(0).split())])
    return data


def problem():
    init_tree(read())
    balance()
    for k in sorted(movements.keys()):
        print '%d: %d %d' % (k, movements[k][0], movements[k][1])


problem()