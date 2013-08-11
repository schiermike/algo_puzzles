"""
There are K pegs. Each peg can hold discs in decreasing order of radius when looked from bottom to top of the peg. There
are N discs which have radius 1 to N; Given the initial configuration of the pegs and the final configuration of the
pegs, output the moves required to transform from the initial to final configuration. You are required to do the
transformations in minimal number of moves.

A move consists of picking the topmost disc of any one of the pegs and placing it on top of anyother peg.
At anypoint of time, the decreasing radius property of all the pegs must be maintained.

Constraints:
1<= N<=8
3<= K<=5


Input Format:
N K
2nd line contains N integers.
Each integer in the second line is in the range 1 to K where the i-th integer denotes the peg to which disc of radius
i is present in the initial configuration.
3rd line denotes the final configuration in a format similar to the initial configuration.


Output Format:
The first line contains M - The minimal number of moves required to complete the transformation.
The following M lines describe a move, by a peg number to pick from and a peg number to place on.
If there are more than one solutions, it's sufficient to output any one of them. You can assume, there is always a
solution with less than 7 moves and the initial confirguration will not be same as the final one.

Sample Input #00:


2 3
1 1
2 2
Sample Output #00:


3
1 3
1 2
3 2


Sample Input #01:

6 4
4 2 4 3 1 1
1 1 1 1 1 1
Sample Output #01:

5
3 1
4 3
4 1
2 1
3 1
"""
import copy


class Config(object):
    def __init__(self, parent, disk_move=None):
        self.disk_move = disk_move
        if not disk_move:  # root
            self.parent = None
            self.c = parent
        else:
            self.parent = parent
            self.c = copy.deepcopy(parent.c)
            disk = self.c[disk_move[0]].pop(0)
            self.c[disk_move[1]].insert(0, disk)

        self.h = hash(tuple([tuple(x) for x in self.c]))  # cache the hash

    def __eq__(self, other):
        return self.c == other.c

    def __hash__(self):
        return self.h

    def __str__(self):
        out = ''
        for stack in self.c:
            if stack:
                out += '%8s' % reduce(lambda x, y: x + str(y), stack, '')
            else:
                out += '        '
        return out


def convert(k, config):
    c = [[] for i in range(k)]
    for i in range(len(config)):
        pos = config[i]
        c[pos - 1].append(i)
    return c


def solve(k, init_config, end_config):
    init_config = Config(convert(k, init_config))
    end_config = Config(convert(k, end_config))

    end_config = find_way(init_config, end_config)

    # backtrack
    c = end_config
    output = ''
    i = 0
    while c.parent:
        i += 1
        output = '%d %d\n%s' % (c.disk_move[0] + 1, c.disk_move[1] + 1, output)
        c = c.parent
    output = '%d\n%s' % (i, output)
    print output


def find_way(init_config, end_config):
    discovered_configs = set()
    pending_configs = [init_config]
    while pending_configs:
        c = pending_configs.pop(0)
#        print '%5d %5d   | %s' % (len(discovered_configs), len(pending_configs), c)
        for cn in next_configs(c):
            if cn in discovered_configs:
                continue
            discovered_configs.add(cn)
            pending_configs.append(cn)
            if cn == end_config:
                return cn
    raise Exception('DOUGH!')


def next_configs(config):
    c = config.c
    new_configs = []
    for src_idx in range(len(c)):
        for dst_idx in range(len(c)):
            if src_idx == dst_idx:  # move does not make sense
                continue
            if not c[src_idx]:  # nothing to move
                continue
            if c[dst_idx] and c[src_idx][0] > c[dst_idx][0]:
                continue
            new_configs.append(Config(config, (src_idx, dst_idx)))
    return new_configs


# solve(4, [4, 2, 4, 3, 1, 1], [1, 1, 1, 1, 1, 1])
# solve(3, [1, 1], [2, 2])
solve(4, [4, 2, 4, 3, 1, 1], [4, 3, 2, 1, 2, 3])

