import sys
import time
import random
import math
from statistics import mean, median

IS_CI = "--ci" in sys.argv

def numeric_tests():
    nums = [random.randint(1, 1000) for _ in range(200 if IS_CI else 1000)]
    print("mean:", mean(nums))
    print("median:", median(nums))

    def is_prime(n):
        if n < 2:
            return False
        for i in range(2, int(math.sqrt(n)) + 1):
            if n % i == 0:
                return False
        return True

    limit = 100 if IS_CI else 200
    primes = [n for n in range(2, limit) if is_prime(n)]
    print("primes <=", limit, ":", len(primes))

def timing_tests():
    loops = 100_000 if IS_CI else 1_000_000
    start = time.perf_counter()
    s = sum(range(loops))
    elapsed = time.perf_counter() - start
    print("sum:", s)
    print("elapsed:", round(elapsed, 4), "sec")

def main():
    print("CI MODE:", IS_CI)
    numeric_tests()
    timing_tests()
    print("DONE")

if __name__ == "__main__":
    main()