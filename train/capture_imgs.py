import os
import time
import sys

# python3 capture_imgs.py wash 1 100 train

for i in range(int(sys.argv[2]), int(sys.argv[3])):
	print(i)
	os.system("wget -q -O data/{2}/{0}/{0}_{1}.jpg http://192.168.1.112/capture?_cb=1603311817835".format(sys.argv[1], i, sys.argv[4]))
	time.sleep(0.25)
