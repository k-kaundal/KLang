// utils.k - Utility functions module

export fn isEven(n) {
    return n % 2 == 0
}

export fn isOdd(n) {
    return n % 2 == 1
}

export fn max(a, b) {
    if (a > b) {
        return a
    }
    return b
}

export fn min(a, b) {
    if (a < b) {
        return a
    }
    return b
}
