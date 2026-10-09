import random
import sys

i = int(sys.argv[1])
nums = list(range(i))
random.shuffle(nums)

for n  in nums:
    print(n)
