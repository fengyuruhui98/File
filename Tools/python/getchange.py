import requests
import re
import sys
import os
import time
	
def load_page(url):
	response = requests.get(url)
	data = response.content
	return data

def load_page2(url, urll):
	kv = {'user-agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36 Edg/107.0.1418.42', 'referer': urll}
	response = requests.get(url, headers = kv)
	data = response.content
	return data

def getImage(html, urll, nums):
	regx = r'https://[\S]*jpg'
	pattern = re.compile(regx)
	get_images = re.findall(pattern, repr(html))
	count = 1
	
	for img in get_images:
		if count <= 3:
			image = load_page2(img, urll)
			with open('./spider_picture/%s.jpg' % nums, 'wb') as fb:
				fb.write(image)
				nums += 1
				count += 1

if __name__ == "__main__":
	urll = sys.argv[1]
	num = int(sys.argv[2])
	dirs = sys.argv[1].split("/")
	print(dirs[4])
	nums = 0
	for i in range(1, num + 1):
		print(i)
		u = urll + "-" + str(i) + ".html"
		print(u)
		html = load_page(u)
		getImage(html, u, nums)
		nums += 3
		time.sleep(1)
	if not os.path.exists("./pic/" + str(dirs[4])):
		os.system("mkdir ./pic/" + str(dirs[4]))
	else: 
		print("warning: maybe already download before")
	os.system("mv ./spider_picture/*.jpg ./pic/" + str(dirs[4]))
