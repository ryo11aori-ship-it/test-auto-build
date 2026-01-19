#!/usr/bin/env python3
import os
import sys
import time
import math
import json
import csv
import random
import threading
import asyncio
import subprocess
from pathlib import Path
from statistics import mean, median

# -----------------------------
# 基本情報
# -----------------------------
def basic_info():
    print("=== BASIC INFO ===")
    print("Python version:", sys.version)
    print("Executable:", sys.executable)
    print("Platform:", sys.platform)
    print("CWD:", os.getcwd())
    print("Args:", sys.argv)
    print()

# -----------------------------
# 数値計算・アルゴリズム
# -----------------------------
def numeric_tests():
    print("=== NUMERIC TESTS ===")

    nums = [random.randint(1, 1000) for _ in range(1000)]
    print("mean:", mean(nums))
    print("median:", median(nums))

    # 素数判定
    def is_prime(n):
        if n < 2:
            return False
        for i in range(2, int(math.sqrt(n)) + 1):
            if n % i == 0:
                return False
        return True

    primes = [n for n in range(2, 200) if is_prime(n)]
    print("primes <= 200:", len(primes))

    # 再帰（フィボナッチ）
    def fib(n):
        if n <= 1:
            return n
        return fib(n - 1) + fib(n - 2)

    print("fib(10):", fib(10))
    print()

# -----------------------------
# 文字列・Unicode
# -----------------------------
def string_tests():
    print("=== STRING TESTS ===")
    s = "Hello, 世界 🌍 Python 🐍"
    print("original:", s)
    print("upper:", s.upper())
    print("reversed:", s[::-1])
    print("length:", len(s))
    print()

# -----------------------------
# ファイル I/O
# -----------------------------
def file_tests():
    print("=== FILE TESTS ===")
    base = Path("test_output")
    base.mkdir(exist_ok=True)

    # text
    text_file = base / "sample.txt"
    text_file.write_text("Line1\nLine2\nこんにちは\n", encoding="utf-8")
    print("text read:", text_file.read_text(encoding="utf-8").strip())

    # JSON
    json_file = base / "data.json"
    data = {"a": 1, "b": [1, 2, 3], "msg": "hello"}
    json_file.write_text(json.dumps(data, ensure_ascii=False, indent=2), encoding="utf-8")
    loaded = json.loads(json_file.read_text(encoding="utf-8"))
    print("json loaded:", loaded)

    # CSV
    csv_file = base / "data.csv"
    with csv_file.open("w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["x", "y"])
        for i in range(5):
            w.writerow([i, i * i])

    with csv_file.open("r", encoding="utf-8") as f:
        rows = list(csv.reader(f))
    print("csv rows:", rows)

    print()

# -----------------------------
# 例外処理
# -----------------------------
def exception_tests():
    print("=== EXCEPTION TESTS ===")
    try:
        x = 1 / 0
    except ZeroDivisionError as e:
        print("caught:", repr(e))

    try:
        int("not_a_number")
    except ValueError as e:
        print("caught:", repr(e))

    print()

# -----------------------------
# マルチスレッド
# -----------------------------
def threading_tests():
    print("=== THREADING TESTS ===")
    results = []

    def worker(i):
        time.sleep(random.random() * 0.2)
        results.append(i * i)

    threads = []
    for i in range(5):
        t = threading.Thread(target=worker, args=(i,))
        t.start()
        threads.append(t)

    for t in threads:
        t.join()

    print("thread results:", results)
    print()

# -----------------------------
# 非同期処理
# -----------------------------
async def async_task(i):
    await asyncio.sleep(0.1)
    return i + 10

async def async_tests():
    print("=== ASYNCIO TESTS ===")
    tasks = [async_task(i) for i in range(5)]
    results = await asyncio.gather(*tasks)
    print("async results:", results)
    print()

# -----------------------------
# サブプロセス
# -----------------------------
def subprocess_tests():
    print("=== SUBPROCESS TESTS ===")
    try:
        proc = subprocess.run(
            [sys.executable, "-c", "print('hello from subprocess')"],
            capture_output=True,
            text=True,
            check=True
        )
        print("stdout:", proc.stdout.strip())
    except Exception as e:
        print("subprocess error:", e)
    print()

# -----------------------------
# パフォーマンス計測
# -----------------------------
def timing_tests():
    print("=== TIMING TESTS ===")
    start = time.perf_counter()
    s = 0
    for i in range(1_000_000):
        s += i
    elapsed = time.perf_counter() - start
    print("sum:", s)
    print("elapsed:", round(elapsed, 4), "sec")
    print()

# -----------------------------
# メイン
# -----------------------------
def main():
    print("=== PYTHON AUTOBUILD TOOL TEST ===")
    basic_info()
    numeric_tests()
    string_tests()
    file_tests()
    exception_tests()
    threading_tests()
    asyncio.run(async_tests())
    subprocess_tests()
    timing_tests()
    print("=== ALL TESTS COMPLETED ===")

if __name__ == "__main__":
    main()
