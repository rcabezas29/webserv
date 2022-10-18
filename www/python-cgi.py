import sys

headers = \
"""
X-Powered-By: PHP/8.1.9
Content-type: text/html; charset=UTF-8
"""

response1 = \
"""
<html>
        <head>
                <title>PHP Test</title>
        </head>
        <body>
                <p>
"""
response2 = \
"""
</p> 
        </body>
</html>
"""

def text_file(file):
	with open(file, "r+") as r:
		content = r.readlines()
	return("".join(content))


if __name__ == "__main__":
	n = len(sys.argv)
	if n > 2:
		print("ERROR: Bad arguments")
	else:
		file = sys.argv[1]
		print(f"{headers}\n{response1}{text_file(file)}{response2}")