// edge_cases.k - Test edge cases for module exports
export fn noParams() {
    return "no parameters"
}

export fn oneParam(x) {
    return "one param: " + x
}

export fn threeParams(a, b, c) {
    return a + b + c
}

export fn returnsFunction() {
    return (x) => {
        return x * 2
    }
}

export fn usesClosureVariable() {
    let count = 0
    return () => {
        count = count + 1
        return count
    }
}
