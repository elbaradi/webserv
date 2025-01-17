file = "HTTPResponseMessageContentTypeMap.cpp"

with open(file, 'w') as f:
	f.write("/*\n *\tfile generated by contentTypeMapGenerator.py, to be copied into srcs/http\n *\tcontains static map<extension, content-type> declared in HTTPResponseMessage.h\n */\n\n")

f = open(file, 'a')
f.write("#include \"HTTPResponseMessage.hpp\"\n\n")
f.write("const std::map<std::string, const std::string> HTTPResponseMessage::contentTypes =\n{\n")

s = ""

for line in open("mime.types"):
    li = line.strip()
    if not li.startswith("#"):
		arr = line.rstrip().split()
		i = 0
		sizeofArr = len(arr)
		while i < sizeofArr:
			if i == 0:
				contentType = arr[i]
			else:
				extension = arr[i]
				s += "\t{ \"" + extension + "\", \"" + contentType + "\" },\n"
			i += 1
s = s[:-2]
s += "\n};\n"
f.write(s)
f.close()
			