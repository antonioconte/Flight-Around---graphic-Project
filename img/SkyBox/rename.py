import os
import sys


dir = sys.argv[1]
for f in os.listdir(dir):
	print(dir+"/"+f)
	if "UP" in f.upper():
		os.rename(dir+"/"+f,dir+"/top.png")
	if "FT" in f.upper():
		os.rename(dir+"/"+f,dir+"/right.png")
	if "BK" in f.upper():
		os.rename(dir+"/"+f,dir+"/left.png")
	if "RT" in f.upper():
		os.rename(dir+"/"+f,dir+"/front.png")
	if "DN" in f.upper():
		os.rename(dir+"/"+f,dir+"/bottom.png")
	if "LF" in f.upper():
		os.rename(dir+"/"+f,dir+"/back.png")
