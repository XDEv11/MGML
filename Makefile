SOURCE = MGML.cpp
PYWRAP = pywrap.cpp
OBJ = MGML.o
PYSO = MGML$(shell python3-config --extension-suffix)
CPPTEST = test_MGML
PYTEST = test_MGML.py

all: cpp py

cpp: $(OBJ)
	g++ -Wall -std=c++17 $(CPPTEST).cpp $(OBJ) -o $(CPPTEST)
	@echo "Testing in C++..."
	@./$(CPPTEST)

py: $(PYSO)
	@echo "Testing in Python..."
	@python test_MGML.py 1>test.log
	@cat test.log

$(OBJ): $(SOURCE)
	g++ -O3 -Wall -std=c++17 -c $(SOURCE) -o $(OBJ)

$(PYSO): $(PYWRAP)
	g++ -O3 -Wall -std=c++17 -shared -fPIC $(PYWRAP) $(OBJ) -o $(PYSO) \
		$(shell python3 -m pybind11 --includes) $(shell python3-config --includes --ldflags)

clean:
	rm -f $(OBJ) $(CPPTEST) $(PYSO)

.PHONY: all cpp py clean
