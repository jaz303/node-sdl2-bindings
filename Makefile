src/constants.cc: data/constants.js
	node $< > $@

todo:
	grep -nr "TODO:" src
