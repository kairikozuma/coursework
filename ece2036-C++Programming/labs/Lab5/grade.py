def getResult(file):
	result = {}
	for line in file:
		curLine = line.strip().split(" ")
		pos = curLine[1]
		db = float(curLine[3])
		numWall= float(curLine[5])
		red= float(curLine[7])
		green= float(curLine[9])
		blue= float(curLine[11])
		result[pos] = ((db,red,green,blue),numWall)
	return result

def compareResult(solution,your_solution):
	numMatch = 0 
	for item,value in solution.items():
		if item in your_solution:
			your_value = your_solution[item]
			if value[1]==your_value[1] and len(filter(lambda x: abs(x[0]-x[1])<=1, zip(value[0],your_value[0]))) == len(value[0]):
				numMatch +=1
	print "correct number of matches are " + str(len(solution.keys()))
	print "your number of matches are " + str(numMatch)
	if numMatch == len(solution.keys()):
		print "Congratulations you pass"
	else:
		print "You did not pass"



if __name__ == "__main__":
	import sys
	solution = getResult(open(sys.argv[1],"r"))
	your_solution = getResult(open(sys.argv[2],"r"))
        compareResult(solution,your_solution)
