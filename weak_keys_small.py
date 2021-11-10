from math import comb
from math import log

def combs(iterable, r):
    pool = tuple(iterable)
    n = len(pool)
    if r > n:
        return
    indices = list(range(r))
    yield list(pool[i] for i in indices)
    while True:
        for i in reversed(range(r)):
            if indices[i] != i + n - r:
                break
        else:
            return
        indices[i] += 1
        for j in range(i + 1, r):
            indices[j] = indices[j - 1] + 1
        yield list(pool[i] for i in indices)


def coeff_WK(num_index):
    I = list(combs([0, 1, 2, 3], num_index))
    KEYS = []
    for XX in I:
        for i in range(1 << 8):
            B = [int(y) for y in '{0:08b}'.format(i)]
            count = 0
            for x in XX:
                if B[x] != B[x + 4]:
                    count = count + 1
                else:
                    break
            if count == len(XX):
                KEYS.append(i)
    return KEYS


def coeff_sWK(num_index):
    I = list(combs([0, 1, 2, 3], num_index))
    KEYS = []
    for XX in I:
        for i in range(1 << 8):
            B = [int(y) for y in '{0:08b}'.format(i)]
            count = 0
            for x in XX:
                if B[x] == 0 and B[x + 4] == 1:
                    count = count + 1
                else:
                    break
            if count == len(XX):
                KEYS.append(i)
    return KEYS


def count_keys_WK_small(n, J, d):
    II = list(combs([i for i in range(n)], d))
    l = '{0:0' + str(2 * n) + 'b}'
    KEYS = []
    for XX in II:
        YY = list(set([i for i in range(n)]) - set(XX))
        for i in range(1 << (2 * n)):
            B = [int(z) for z in l.format(i)]
            count = 0
            for x in XX:
                if x in J:
                    if B[x] != B[x + n]:
                        count = count + 1
                else:
                    if B[x] == B[x + n]:
                        count = count + 1
            for y in YY:
                if y in J:
                    count = count + 1
                else:
                    if B[y] != B[y + n]:
                        count = count + 1
            if count == n:
                KEYS.append(i)

    return KEYS


def count_keys_sWK_small(n, J, d):
    II = list(combs([i for i in range(n)], d))
    l = '{0:0' + str(2 * n) + 'b}'
    KEYS = []
    for XX in II:
        YY = list(set([i for i in range(n)]) - set(XX))
        for i in range(1 << (2 * n)):
            B = [int(z) for z in l.format(i)]
            count = 0
            for x in XX:
                if x in J:
                    if (B[x] == 0) and (B[x + n]) == 1:
                        count = count + 1
                else:
                    if B[x] == 0 and B[x + n] == 0:
                        count = count + 1

            for y in YY:
                if y in J:
                    count = count + 1
                else:
                    if B[y] != B[y + n]:
                        count = count + 1
            if count == n:
                KEYS.append(i)

    return KEYS


def count_WK_eqn_20(n, d):
    l = n - d
    X = comb(l, d) * 256 + comb(l, d - 1) * 240 + comb(l, d - 2) * 176 + comb(l, d - 3) * 80 + comb(l, d - 4) * 16
    X = X * (1 << l)
    return X

def count_sWK_eqn_23(n, d):
    l = n - d
    X = comb(l, d) * 256 + comb(l, d - 1) * 2 * 175 + comb(l, d - 2) * 4 * 67 + comb(l, d - 3) * 8 * 13 + comb(l,
                                                                                                               d - 4) * 16
    X = X * (1 << (l - d))
    return X


print("Coefficients for WK^r[d] (Equation 20)")
for i in range(4):
    X = list(set(coeff_WK(i + 1)))
    print("alpha_{} = {}".format(i + 1, len(X)))

print("Coefficients for sWK^r[d'] (Equation 22)")
for i in range(4):
    X = list(set(coeff_sWK(i + 1)))
    print("beta_{} = {}".format(i + 1, len(X)))

print("Small parameters for verification")
print("Key size = 16")
print("d = d' = 4")
print("Fixed indices corresponding to {56, 57, 58, 59}: {1, 2, 3, 4}")

print("Estimated number of weak keys [WK] using Equation 20 = {}".format(count_WK_eqn_20(8, 4)))
Y1 = list(set(count_keys_WK_small(8, [1, 2, 3, 4], 4)))
print("Actual number of weak keys [WK] = {}".format(len(Y1)))

print("Estimated number of weak keys [sWK] using Equation 23 = {}".format(2*count_sWK_eqn_23(8, 4)))
Y2 = list(set(count_keys_sWK_small(8, [1, 2, 3, 4], 4)))
print("Actual number of weak keys [sWK] = {}".format(2*len(Y2)))


