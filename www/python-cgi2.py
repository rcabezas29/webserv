import sys

headers = "X-Powered-By: Python/3.9\r\nContent-type: text/html; charset=UTF-8\r\n"

response = "<html>\n<head>\n<title>\nPython CGI Test\n</title>\n</head>\n<body>\n<p>Hello Python CGI</p>\n</body>\n</html>"

def text_file(file):
	with open(file, "r+") as r:
		content = r.readlines()
	return("".join(content))


if __name__ == "__main__":
	n = len(sys.argv)
	if n > 1:
		print("ERROR: Bad arguments")
	else:
		# file = sys.argv[1]
		# print(f"{headers}\n{response1}{text_file(file)}{response2}")
		print(f"{headers}\r\n{response}\r\n")